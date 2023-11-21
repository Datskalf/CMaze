/**
 * A general header file containing the project-global values.
 *
 * @author Datskalf
 * @version 1.1
 * @date 2023-11-09
 */

#ifndef MAZEGENERATOR_COMMON_H
#define MAZEGENERATOR_COMMON_H

#define LINUX_INCREASE_STACK_SIZE 0
#define LINUX_STACK_SIZE 16

#define PRINT_BRANCHES 2
#define DEBUG_LEVEL 1
#define PRINT_PARAMETER_SETUP 1
#define INCLUDE_MAX_SIZE 0
#define MAZE_MAX_SIZE 400

#define WALL_SYMBOL "X"
#define FREE_SYMBOL " "
#define START_SYMBOL "S"
#define END_SYMBOL "E"

#pragma region ANSI COLOURS
#define ANSI_COLOR_RED      "\x1b[31m"
#define ANSI_COLOR_GREEN    "\x1b[32m"
#define ANSI_COLOR_YELLOW   "\x1b[33m"
#define ANSI_COLOR_BLUE     "\x1b[34m"
#define ANSI_COLOR_MAGENTA  "\x1b[35m"
#define ANSI_COLOR_CYAN     "\x1b[36m"
#define ANSI_COLOR_RESET    "\x1b[0m"

#pragma endregion

extern int mazeWidth;
extern int mazeHeight;
extern int randomBranchLimit;
extern int use_colours;
extern int print_all_branches;

#endif
