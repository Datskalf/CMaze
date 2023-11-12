/**
 * This file controls the array containing the maze state. This file should only be interacted with
 * by the maze.c file, as anything else might mess with the state incorrectly.
 *
 * @author Datskalf
 * @version 1.0
 * @date 2023-11-02
 */

#include <stdlib.h>
#include "common.h"
#include "maze_API.h"
#include "maze_data.h"
#include "rng.h"

unsigned char* startTile;
unsigned char* endTile;
unsigned char** mazeState;

/**
 * Assigns the required memory for the maze state array.
 */
void mazeInit() {
    mazeState = (unsigned char**) malloc(mazeWidth * sizeof(unsigned char*));

    for (int i = 0; i < mazeWidth; i++) {
        mazeState[i] = (unsigned char*) malloc(mazeHeight * sizeof(unsigned char));
    }

    for (int x = 0; x < mazeWidth; x++) {
        for (int y = 0; y < mazeHeight; y++) {
            setAllTileWalls(x, y, ON, ON, ON, ON);
        }
    }
}

/**
 * Finds the tile as the given coordinates, and sets the state of the wall in the given direction.
 *
 * @param x The 0-indexed column of the tile.
 * @param y The 0-indexed row of the tile.
 * @param direction Which wall is being set.
 * @param state What state the wall should assume.
 */
void setTileWall(int x, int y, enum Direction direction, enum State state) {
    int stateNorm = (state > 0);
    if (x < 0 || x >= mazeWidth || y < 0 || y >= mazeHeight) {
        return;
    }

    mazeState[x][y] &= ~(1 << direction); // Clear the wall state
    mazeState[x][y] |= stateNorm << direction; // Set the wall state equal to the parameter
}

/**
 * Finds the tile at the given coordinates, and sets each wall according to the given states.
 *
 * @param x The 0-indexed column of the tile.
 * @param y The 0-indexed row of the tile.
 * @param hasNorth State of the wall above the tile.
 * @param hasEast State of the wall to the right of the tile.
 * @param hasSouth State of the wall below the tile.
 * @param hasWest State of the wall to the left of the tile.
 */
void setAllTileWalls(int x, int y, enum State hasNorth, enum State hasEast, enum State hasSouth, enum State hasWest) {
    setTileWall(x, y, NORTH, hasNorth);
    setTileWall(x, y, EAST, hasEast);
    setTileWall(x, y, SOUTH, hasSouth);
    setTileWall(x, y, WEST, hasWest);
}

void setStartTile(int x, int y) {
    startTile = &mazeState[x][y];
}

void setEndTile(int x, int y) {
    endTile = &mazeState[x][y];
}

/**
 * Takes the tile at the given coordinates, and returns the state of the wall in the given direction.
 *
 * @param x The 0-indexed column of the tile.
 * @param y The 0-indexed row of the tile.
 * @param direction The direction from the tile to the wall.
 * @return The state the wall currently holds.
 */
int getWall(int x, int y, enum Direction direction) {
    return 1 & (mazeState[x][y] >> direction);
}

/**
 * Takes the tile at the given coordinates, and returns how many of the walls are set.
 * @param x The 0-indexed column of the tile.
 * @param y The 0-indexed row of the tile.
 * @return The total wall count.
 */
int getWallCount(int x, int y) {
    int count = 0;
    count += getWall(x, y, NORTH);
    count += getWall(x, y, EAST);
    count += getWall(x, y, SOUTH);
    count += getWall(x, y, WEST);
    return count;
}

/**
 * Creates and returns a bitmask of each unvisited tile orthogonal to the tile at the provided coordinates.
 * A tile is deemed unvisited if all 4 of its walls are ON.
 *
 * @param x The 0-indexed column of the tile.
 * @param y The 0-indexed row of the tile.
 * @return A bitmask of which walls are unvisited.
 */
int getUnvisitedNeighbors(int x, int y) {
    int result = 0;

    //*
    if (y > 0 && getWalls(x, y-1) == 15) result |= (1 << NORTH);
    if (x+1 < mazeWidth && getWalls(x+1, y) == 15) result |= (1 << EAST);
    if (y+1 < mazeHeight && getWalls(x, y+1) == 15) result |= (1 << SOUTH);
    if (x > 0 && getWalls(x-1, y) == 15) result |= (1 << WEST);
     //*/

    return result;
}

/**
 * Creates and returns a bitmask of each wall connected to the tile at the given coordinates.
 *
 * @param x The 0-indexed column of the tile.
 * @param y The 0-indexed row of the tile.
 * @return The count of unvisited tiles.
 */
int getWalls(int x, int y) {
    int result = 0;
    result |= getWall(x, y, NORTH) << NORTH;
    result |= getWall(x, y, EAST) << EAST;
    result |= getWall(x, y, SOUTH) << SOUTH;
    result |= getWall(x, y, WEST) << WEST;
    return result;
}

/**
 * Collects all tiles in the maze which have exactly 2 walls.
 * The function will then pick one at random, store the coordinates in the passed array, and return success or failure.
 *
 * Returns a 1 if there exists at least one branch point, and a 0 if not.
 *
 * @param coordArr Pointer to the selected branch point.
 * @return A boolean value stating whether a valid branch point was found or not.
 */
int getRandomBranchPoint(int* coordArr) {
    static int earliestX = 0, earliestY = 0;

    int count = 0;
    int coords[mazeWidth*mazeHeight][2];
    int keepLocating = 1;

    for (int x = earliestX; x < mazeWidth && keepLocating; x++) {
        int y = 0;
        if (x == earliestX) y = earliestY;
        for (; y < mazeHeight && keepLocating; y++) {

            if (isStartTile(x, y)) {
                continue;
            }

            int wallCount = getWallCount(x, y);
            if (getUnvisitedNeighbors(x, y) && (wallCount == 2 || wallCount == 3)) {
                if (count == 0) {
                    earliestX = x;
                    earliestY = y;
                }
                coords[count][0] = x;
                coords[count][1] = y;
                count++;
            }

            keepLocating = (count < randomBranchLimit);
        }
    }

    // If there are any valid new branch points, pick a random one, and store it in the passed array.
    if (count) {
        int randChoice = randInt(count);
        coordArr[0] = coords[randChoice][0];
        coordArr[1] = coords[randChoice][1];
        return 1;
    }
    return 0;
}

int isStartTile(int x, int y) {
    return &mazeState[x][y] == startTile;
}

int isEndTile(int x, int y) {
    return &mazeState[x][y] == endTile;
}