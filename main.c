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
 * License: GNU GPLv3 (https://www.gnu.org/licenses/gpl-3.0.html)
 *
 *
 * @author Datskalf
 * @version 1.2
 * @date 2023-11-09
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "common.h"
#include "maze_API.h"
#include "output.h"

int mazeWidth = 8;
int mazeHeight = 8;
int randomBranchLimit = 20;
int use_colours = 0;
int print_all_branches = 0;

unsigned int seed;

/**
 * The function allows the user to pass keyed arguments for width, height, and seed.
 * Making these keyed means the user themselves decide what values to set, and what should be left as default.<br/><br/>
 * The valid keys include:
 * <ul>
 *  <li>[-w, --width]: Sets the width of the maze in tiles.</li>
 *  <li>[-h, --height]: Sets the height of the maze in tiles.</li>
 *  <li>[-s, --seed]: Sets the seed used for the RNG generation.</li>
 *  <li>[-o, --output]: Sets the stream or file to write the resulting maze to.</li>
 *  <li>[-bl, --branch-limit]: Sets the branch limit used for maze generation.</li>
 *  <li>[-c, -colour]: Enables coloured output.</li>
 * </ul>
 *
 * @param argc An integer defining the item count of argv.
 * @param argv An array of char* containing the arguments passed to the program.
 */
void readParameters(int argc, char* argv[]) {
    char outfile[50];
    for (int i = 1; i < argc; i++) {

        // Sets the width of the maze.
        if ((strcmp(argv[i], "-w") == 0 || strcmp(argv[i], "--width") == 0) && i+1 < argc) {
            char readVal[10];
            sscanf(argv[++i], "%s", readVal);
            mazeWidth = strtol(readVal, NULL, 10);

            #if INCLUDE_MAX_SIZE == 1
            if (mazeWidth >= MAZE_MAX_SIZE) {
                mazeWidth = MAZE_MAX_SIZE;
            }
            #endif

            #if PRINT_PARAMETER_SETUP >= 1
            cfprintf(stdout, GREEN, "Setup: ");
            printf("Set width equal to %d\n", mazeWidth);
            #endif
        }


        // Sets the height of the maze.
        else if ((strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--height") == 0) && i+1 < argc) {
            char readVal[10];
            sscanf(argv[++i], "%s", readVal);
            mazeHeight = strtol(readVal, NULL, 10);

            #if INCLUDE_MAX_SIZE
            if (mazeHeight >= MAZE_MAX_SIZE) {
                mazeHeight = MAZE_MAX_SIZE;
            }
            #endif

            #if PRINT_PARAMETER_SETUP >= 1
            cfprintf(stdout, GREEN, "Setup: ");
            printf("Set height equal to %d\n", mazeHeight);
            #endif
        }


        // Sets the seed for the RNG module.
        else if ((strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--seed") == 0) && i+1 < argc) {
            char readVal[10];
            sscanf(argv[++i], "%s", readVal);
            seed = strtol(readVal, NULL, 10);

            #if PRINT_PARAMETER_SETUP >= 1
            cfprintf(stdout, GREEN, "Setup: ");
            printf("Set seed equal to %d\n", seed);
            #endif
        }


        // Sets the branch limit of the maze generator.
        else if ((strcmp(argv[i], "-bl") == 0 || strcmp(argv[i], "--branch-limit") == 0) && i+1 < argc) {
            char readVal[10];
            sscanf(argv[++i], "%s", readVal);
            randomBranchLimit = strtol(readVal, NULL, 10);

            #if PRINT_PARAMETER_SETUP >= 1
            cfprintf(stdout, GREEN, "Setup: ");
            printf("Set branch limit equal to %d\n", randomBranchLimit);
            #endif
        }


        // Sets the output stream to the specified stream, or filepath if the stream is unrecognized
        else if ((strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0) && i+1 < argc) {
            sscanf(argv[++i], "%s", outfile);

            if (strcmp(outfile, "stdout") == 0) set_stream(stdout);
            else if (strcmp(outfile, "stderr") == 0) set_stream(stderr);
            else if (strcmp(outfile, "stdin") == 0) set_stream(stdin);
            else open_file(outfile);

            #if PRINT_PARAMETER_SETUP >= 1
            cfprintf(stdout, GREEN, "Setup: ");
            printf("Set output stream to %s\n", outfile);
            #endif
        }


        // Enables ANSI coloured output to the console
        else if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--colour") == 0) {
            use_colours = 1;

            #if PRINT_PARAMETER_SETUP
            cfprintf(stdout, GREEN, "Setup: ");
            printf("Enabled colour mode\n");
            #endif
        }


        // Prints all maze branch iterations to the output file.
        else if (strcmp(argv[i], "-pab") == 0 || strcmp(argv[i], "--print-all-branches") == 0) {
            print_all_branches = 1;

            #if PRINT_PARAMETER_SETUP
            cfprintf(stdout, GREEN, "Setup: ");
            printf("Printing all branch iterations\n");
            #endif
        }
    }
}

/**
 * Program main entry point.
 *
 * @param argc An integer defining the item count of argv.
 * @param argv An array of char* containing the arguments passed to the program.
 * @return Exit code of the program.
 */
int main(int argc, char* argv[]) {
#if DEBUG_LEVEL >= 1
    setvbuf(stdout, NULL, _IONBF, 0);
#endif

    set_stream(stdout);
    seed = time(0);
    readParameters(argc, argv);

    srand(seed);
    mazeInit();
    populateMaze();
    fPrintMaze();

    return 0;
}
