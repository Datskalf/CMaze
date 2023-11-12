/**
 * This file utilizes a lot of bit-masking. I used the resource below to learn shorthands for extracting each bit value.
 * https://www.learn-c.org/en/Bitmasks
 *
 * TODO:
 * - Could start random branch from a random point, visited or not. If unvisited, make sure it ends on the path.
 * - WIP: Could precalculate the eligibility for branching, reducing repeated calculations.
 *
 * @author Datskalf
 * @version 1.1
 * @date 2023-10-30
 */

#include <stdio.h>
#include "common.h"
#include "maze.h"
#include "maze_data.h"
#include "maze_API.h"
#include "rng.h"
#include "output.h"

/**
 * Creates a blank maze with all walls filled in.
 * Once the blank is created, path generation is run.
 */
void populateMaze() {


    generatePaths();
}

/**
 * Starting from the given coordinates, creates a random path until the head
 * cannot go anywhere, or hits the end tile.
 *
 * @param x The 0-indexed column of the tile.
 * @param y The 0-indexed row of the tile.
 */
void createPathSegment(int x, int y) {
    int validPaths;

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

        unsigned char unvisitedNeighbors = getUnvisitedNeighbors(x, y);
        int count = (
                (unvisitedNeighbors >> NORTH) & 1
                + (unvisitedNeighbors >> EAST) & 1
                + (unvisitedNeighbors >> SOUTH) & 1
                + (unvisitedNeighbors >> WEST) & 1
                );
        if (count == 2 || count == 3) {
            
        }

        // If head is on the end tile, exit
        if (isEndTile(x, y)) {
            return;
        }
    }

    if (print_all_branches >= 1)
        fPrintMaze();
}

/**
 * Initially, creates a path from the start tile.
 * After this path, will create branches for as long as there exists valid branch points.
 *
 * A path is deemed finished once it either hits a dead end or the end tile.
 */
void generatePaths() {
    int startX = 0, startY = 0;
    int endX = mazeWidth-1, endY = mazeHeight-1;

    // Define the start and end tile locations
    setStartTile(startX, startY);
    setEndTile(endX, endY);


#if PRINT_BRANCHES >= 1
    static int iterationCount = 0;
    printf("Branch iteration no %d", iterationCount++);
    #if PRINT_BRANCHES >= 2
        printf(": x=%d, y=%d", startX, startY);
    #endif
    printf("\n");
#endif
    createPathSegment(startX, startY);

    // loop for as long as there are valid branch points
    int randTileCoord[2];
    while (getRandomBranchPoint(randTileCoord)) {
#if PRINT_BRANCHES >= 1
        printf("Branch iteration no %d", iterationCount++);
        #if PRINT_BRANCHES >= 2
            printf(": x=%d, y=%d", randTileCoord[0], randTileCoord[1]);
        #endif
        printf("\n");
#endif
        createPathSegment(randTileCoord[0], randTileCoord[1]);
    }
}