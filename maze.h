/**
 * @Author Datskalf
 * @version 1.1
 * @date 2023-10-29
 */

#ifndef MAZEGENERATOR_MAZE_H
#define MAZEGENERATOR_MAZE_H


//void setTileWall(int x, int y, enum Direction direction, enum State state);
//void setAllTileWalls(int x, int y, enum State hasNorth, enum State hasEast, enum State hasSouth, enum State hasWest);
void createPathSegment(int x, int y);
void generatePaths();
//int getUnvisitedNeighbors(int x, int y);
//int getWalls(int x, int y);
void printRow(int rowNumber);
//int getWall(int x, int y, enum Direction direction);
//int getWallCount(int x, int y);

#endif
