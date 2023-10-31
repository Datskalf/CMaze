/**
 * This file utilizes a lot of bit-masking. I used the resource below to learn shorthands for extracting each bit value.
 * https://www.learn-c.org/en/Bitmasks
 *
 * Created: 2023-10-25 by som38
 * Last modified: 2023-10-30 by som38
 * Version: 1.1
 */

#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "maze.h"
#include "maze_API.h"
#include "rng.h"

#define WALL_SYMBOL "X"
#define FREE_SYMBOL "-"
#define START_SYMBOL "S"
#define END_SYMBOL "E"

#pragma region Enumerators
/*
 * The Direction enumerator makes tile indexing more intuitive.
 */
enum Direction {
    NORTH = 0,
    EAST = 1,
    SOUTH = 2,
    WEST = 3
};

/*
 * The State enumerator makes wall state indexing more intuitive.
 */
enum State {
    ON = 1,
    OFF = 0
};

#pragma endregion

#pragma region Internal globals

unsigned char* startTile;
unsigned char* endTile;
unsigned char** mazeState;

void mazeInit() {
    mazeState = (unsigned char**) malloc(MAZE_WIDTH * sizeof(unsigned char*));

    for (int i = 0; i < MAZE_WIDTH; i++) {
        mazeState[i] = (unsigned char*) malloc(MAZE_HEIGHT * sizeof(unsigned char));
    }
}

#pragma endregion

#pragma region Tile control API
#pragma region Setters

/*
 * Gets a tile, and sets one single wall state
 */
void setTileWall(int x, int y, enum Direction direction, enum State state) {
    int stateNorm = (state > 0);
    if (x < 0 || x >= MAZE_WIDTH || y < 0 || y >= MAZE_HEIGHT) {
        return;
    }

    mazeState[x][y] &= ~(1 << direction); // Clear the wall state
    mazeState[x][y] |= stateNorm << direction; // Set the wall state equal to the parameter
}

/*
 * Set the tile walls for the given tile to the passed states.
 */
void setAllTileWalls(int x, int y, enum State hasNorth, enum State hasEast, enum State hasSouth, enum State hasWest) {
    setTileWall(x, y, NORTH, hasNorth);
    setTileWall(x, y, EAST, hasEast);
    setTileWall(x, y, SOUTH, hasSouth);
    setTileWall(x, y, WEST, hasWest);
}

#pragma endregion
#pragma region Getters

int getWall(int x, int y, enum Direction direction) {
    return 1 & (mazeState[x][y] >> direction);
}

int getWallCount(int x, int y) {
    int count = 0;
    count += getWall(x, y, NORTH);
    count += getWall(x, y, EAST);
    count += getWall(x, y, SOUTH);
    count += getWall(x, y, WEST);
    return count;
}

/*
 * Creates and returns a bitmap of each unvisited tile orthogonal to the tile at the provided coordinates.
 * A tile is deemed unvisited if all 4 of its walls are ON.
 */
int getUnvisitedNeighbors(int x, int y) {
    int result = 0;
    if (y > 0 && getWalls(x, y-1) == 15) result |= (1 << NORTH);
    if (x+1 < MAZE_WIDTH && getWalls(x+1, y) == 15) result |= (1 << EAST);
    if (y+1 < MAZE_HEIGHT && getWalls(x, y+1) == 15) result |= (1 << SOUTH);
    if (x > 0 && getWalls(x-1, y) == 15) result |= (1 << WEST);
    return result;
}

/*
 * Creates and returns a bitmap of each wall connected to the tile at the given coordinates.
 */
int getWalls(int x, int y) {
    int result = 0;
    result |= getWall(x, y, NORTH) << NORTH;
    result |= getWall(x, y, EAST) << EAST;
    result |= getWall(x, y, SOUTH) << SOUTH;
    result |= getWall(x, y, WEST) << WEST;

#if DEBUG_LEVEL >= 4
    printf("Walls at coordinate %d, %d: %d", x, y, result);
#endif
    return result;
}

/*
 * Collects all tiles in the maze which have exactly 2 walls.
 * The function will then pick one at random, store the coordinates in the passed array, and return success or failure.
 *
 * Returns a 1 if there exists at least one branch point, and a 0 if not.
 */
int getRandomBranchPoint(int* coordArr) {
    int result = 0;
    int count = 0;
    int coords[MAZE_WIDTH*MAZE_HEIGHT][2];
    int keepLocating = 1;

    // For each tile, if the tile is
    //   a. visited
    //   b. neighbours an unvisited tile
    //   c. has no more than two connections
    // store the tile as a valid branch point.
    for (int x = 0; x < MAZE_WIDTH && keepLocating; x++) {
        for (int y = 0; y < MAZE_HEIGHT && keepLocating; y++) {
            if (x + y == 0) {
                continue;
            }
            int wallCount = getWallCount(x, y);
            if (getUnvisitedNeighbors(x, y) && (wallCount == 2 || wallCount == 3)) {
                coords[count][0] = x;
                coords[count][1] = y;
                count++;
                result = 1;
            }

            keepLocating = (count < RANDOM_BRANCH_LIMIT);
        }
    }

    // If there are any valid new branch points, pick a random one, and store it in the passed array.
    if (count) {
        #if DEBUG_LEVEL >= 3
        printf("Branches found: %d\n", count);
        #endif
        int randChoice = randInt(count);
        coordArr[0] = coords[randChoice][0];
        coordArr[1] = coords[randChoice][1];
    }

    return result;
}

#pragma endregion
#pragma endregion

#pragma region Maze generation

/*
 * Creates a blank maze with all walls filled in.
 * Once the blank is created, path generation is run.
 */
void populateMaze() {
    for (int x = 0; x < MAZE_WIDTH; x++) {
        for (int y = 0; y < MAZE_HEIGHT; y++) {
            setAllTileWalls(x, y, ON, ON, ON, ON);
        }
    }

    generatePaths();
}


#pragma region Path Generation

/*
 * Starting from the given coordinates, creates a random path until the head
 * cannot go anywhere, or hits the end tile.
 */
void createPathSegment(int x, int y) {
    int validPaths;

    #if DEBUG_LEVEL >= 3
    printf("Segment start location: %d, %d", x, y);
    #endif

    // Continue until the head doesn't have an unvisited tile next to it or the head is on the end.
    while ((validPaths = getUnvisitedNeighbors(x, y))) {

        // Sum up how many legal paths there are
        int pathOptions = ((validPaths >> NORTH) & 1)
                        + ((validPaths >> EAST)  & 1)
                        + ((validPaths >> SOUTH) & 1)
                        + ((validPaths >> WEST)  & 1);

        // Create an array to hold each possible movement, then select one at random
        int paths[pathOptions];
        for (int i = 0, n = 0; i < 4; i++) {
            if ((validPaths >> i)&1) {
                paths[n++] = i;
            }
        }
        int randDir = paths[randInt(pathOptions)];


        // Set the wall states for the current and next tile
        switch (randDir) {
            case 0:
                setTileWall(x, y--, NORTH, OFF);
                setTileWall(x, y, SOUTH, OFF);
                break;
            case 1:
                setTileWall(x++, y, EAST, OFF);
                setTileWall(x, y, WEST, OFF);
                break;
            case 2:
                setTileWall(x, y++, SOUTH, OFF);
                setTileWall(x, y, NORTH, OFF);
                break;
            case 3:
                setTileWall(x--, y, WEST, OFF);
                setTileWall(x, y, EAST, OFF);
                break;
            default:
                break;
        }



        // If head is on the end tile, exit
        if (&mazeState[x][y] == endTile) {
            #if DEBUG_LEVEL >= 3
            printf("Segment end location: %d, %d", x, y);
            #endif
            return;
        }
    }

    #if DEBUG_LEVEL >= 3
    printf("Segment end location: %d, %d", x, y);
    #endif
}

/*
 * Initially, creates a path from the start tile.
 * After this path, will create branches for as long as there exists valid branch points.
 *
 * A path is deemed finished once it either hits a dead end or the end tile.
 */
void generatePaths() {
    int startX = 0, startY = 0;
    int endX = MAZE_WIDTH-1, endY = MAZE_HEIGHT-1;

    // Define the start and end tile locations
    startTile = &mazeState[startX][startY];
    endTile = &mazeState[endX][endY];

    createPathSegment(startX, startY);

    #if DEBUG_LEVEL == 2
    int currentBranchIndex = 0;
    printf("Current branch: %d\n", currentBranchIndex++);
    #elif DEBUG_LEVEL >= 3
    printMaze();
    printf("\n");
    #endif

    // loop for as long as there are valid branch points
    int randTileCoord[2];
    while (getRandomBranchPoint(randTileCoord)) {
        createPathSegment(randTileCoord[0], randTileCoord[1]);

        #if DEBUG_LEVEL == 2
        printf("Current branch: %d\n", currentBranchIndex++);
        #elif DEBUG_LEVEL >= 3
        printMaze();
        printf("\n");
        #endif
    }
}

#pragma endregion
#pragma endregion

#pragma region Console output

/*
 * Print out the specified tile row as well as the wall row above it.
 */
void printRow(int rowNumber) {
    for (int x = 0; x < MAZE_WIDTH; x++) {
        printf(WALL_SYMBOL);
        printf(getWall(x, rowNumber, NORTH) ? WALL_SYMBOL : FREE_SYMBOL);
    }
    printf(WALL_SYMBOL);
    printf("\n");

    for (int x = 0; x < MAZE_WIDTH; x++) {
        printf(getWall(x, rowNumber, WEST) ? WALL_SYMBOL : FREE_SYMBOL);
        if (&mazeState[x][rowNumber] == startTile) printf(START_SYMBOL);
        else if (&mazeState[x][rowNumber] == endTile) printf(END_SYMBOL);
        else printf(FREE_SYMBOL);
    }
    printf(getWall(MAZE_WIDTH-1, rowNumber, EAST) ? WALL_SYMBOL : FREE_SYMBOL);
    printf("\n");
}

/*
 * Print each row after each other, followed by the last wall row.
 */
void printMaze() {
    for (int y = 0; y < MAZE_HEIGHT; y++) {
        printRow(y);
    }

    for (int x = 0; x < MAZE_WIDTH; x++) {
        printf(WALL_SYMBOL);
        printf(getWall(x, MAZE_HEIGHT-1, SOUTH) ? WALL_SYMBOL : FREE_SYMBOL);
    }
    printf(WALL_SYMBOL);
    //printf("\n");
}

#pragma endregion