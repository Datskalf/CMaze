/**
 * Header file for the internals of the maze state.
 *
 * @author Datskalf
 * @version 1.0
 * @date 2023-11-07
 */

#ifndef MAZEGENERATOR_MAZE_DATA_H
#define MAZEGENERATOR_MAZE_DATA_H

enum Direction {
    NORTH = 0,
    EAST = 1,
    SOUTH = 2,
    WEST = 3
};
enum State {
    OFF = 0,
    ON = 1
};


void setTileWall(int x, int y, enum Direction direction, enum State state);
void setAllTileWalls(int x, int y, enum State hasNorth, enum State hasEast, enum State hasSouth, enum State hasWest);

void setStartTile(int x, int y);
void setEndTile(int x, int y);

int getWall(int x, int y, enum Direction direction);
int getWalls(int x, int y);
int getWallCount(int x, int y);

int getUnvisitedNeighbors(int x, int y);
int getRandomBranchPoint(int* coordArr);

void setCanBranch(int x, int y, enum State state);
int getCanBranch(int x, int y);

void setCanVisit(int x, int y, enum State state);
int getCanVisit(int x, int y);

int isStartTile(int x, int y);
int isEndTile(int x, int y);

#endif
