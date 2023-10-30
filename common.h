/**
 * Created: 2023-10-27 by som38
 * Last modified: 2023-10-30 by som38
 * Version: 1.0
 */

#ifndef MAZEGENERATOR_COMMON_H
#define MAZEGENERATOR_COMMON_H

/*
 * DEBUG_LEVEL defines how much output should be sent to the console.
 * 1: Normal level, only what the user needs to run the program
 * 2: Quick debug, includes each run of the path creation process
 * 3: Deeper debug, includes each coordinate of path creation
 * 4: Includes RNG results and wall scans
 */
#define DEBUG_LEVEL 1
#define PRINT_PARAMETER_SETUP 0
#define INCLUDE_MAX_SIZE 0

#define MAZE_MAX_SIZE 400

/*
 * MAZE_HEIGHT defines how many tile nodes should be stacked vertically
 * MAZE_WIDTH defines how many tile nodes should be stacked horizontally
 */
//#define MAZE_HEIGHT 30
//#define MAZE_WIDTH 30

extern int MAZE_WIDTH;
extern int MAZE_HEIGHT;
extern int RANDOM_BRANCH_LIMIT;

#endif
