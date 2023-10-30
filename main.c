/**
 * The purpose of the program is for me to practice writing functional and clean C code.
 *
 * Currently the generator will create a random maze of sizes specified in the common.h header file.
 * The maze is split into square tiles, which forms a grid.
 * There should only be one legal path from start to end in the generated maze.
 * In the future, the generator will include a solver feature,
 *      so that when it's given a maze to solve, it will find the shortest (or only) path to the end.
 *
 * If you encounter any errors in my code, you are encouraged to attempt to fix it yourself.
 * If you can't be asked, you can e-mail me the problem, and I'll try to publish a fixed version.
 *
 *
 * Author: som38
 * Email: som38@aber.ac.uk
 * Version: 1.1
 * Last modified: 2023-10-30
 * License: GNU GPLv3 (https://www.gnu.org/licenses/gpl-3.0.html)
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "common.h"
#include "maze_API.h"

int MAZE_WIDTH = 8;
int MAZE_HEIGHT = 8;
int RANDOM_BRANCH_LIMIT = 20;

unsigned int seed;

/*
 * The function allows the user to pass keyed arguments for width, height, and seed.
 * Making these keyed means the user themselves decide what values to set, and what should be left as default.
 */
void readParameters(int argc, char* argv[]) {
    for (int i = 1; i < argc - 1; i++) {
        if (strcmp(argv[i], "-w") == 0) {
            sscanf(argv[++i], "%d", &MAZE_WIDTH);

            #if INCLUDE_MAX_SIZE == 1
            if (MAZE_WIDTH >= MAZE_MAX_SIZE) {
                MAZE_WIDTH = MAZE_MAX_SIZE;
            }
            #endif
            #if PRINT_PARAMETER_SETUP == 1
            printf("Set width equal to %d\n", MAZE_WIDTH);
            #endif
        }
        else if (strcmp(argv[i], "-h") == 0) {
            sscanf(argv[++i], "%d", &MAZE_HEIGHT);

            #if INCLUDE_MAX_SIZE
            if (MAZE_HEIGHT >= MAZE_MAX_SIZE) {
                MAZE_HEIGHT = MAZE_MAX_SIZE;
            }
            #endif
            #if PRINT_PARAMETER_SETUP == 1
            printf("Set height equal to %d\n", MAZE_HEIGHT);
            #endif
        }
        else if (strcmp(argv[i], "-s") == 0) {
            sscanf(argv[++i], "%d", &seed);

            #if PRINT_PARAMETER_SETUP == 1
            printf("Set seed equal to %d\n", seed);
            #endif
        }
        else if (strcmp(argv[i], "-bl") == 0) {
            sscanf(argv[++i], "%d", &RANDOM_BRANCH_LIMIT);
        }
    }
}


int main(int argc, char* argv[]) {
    #if DEBUG_LEVEL >= 2
    setvbuf(stdout, NULL, _IONBF, 0);
    #endif

    seed = time(0);
    readParameters(argc, argv);

    srand(seed);
    mazeInit();
    populateMaze();
    printMaze();

    return 0;
}
