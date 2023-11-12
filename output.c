/**
 * Controls where the maze gets outputted. Can output both to a filepath or stdout (or stderr, if you're mad).
 *
 * @author Datskalf
 * @version 1.1
 * @date 2023-11-09
 */

#include <stdio.h>
#include "common.h"
#include "maze_data.h"
#include "output.h"

FILE* outfile;

void set_stream(FILE* stream) {
    outfile = stream;
}

void open_file(char* fp) {
    set_stream(fopen(fp, "w"));
}



/**
 * Print out the specified tile row as well as the wall row above it.
 *
 * @param rowNumber The row to print out.
 */
void fPrintRow(int rowNumber) {
    for (int x = 0; x < mazeWidth; x++) {
        fprintf(outfile, WALL_SYMBOL);
        fprintf(outfile, getWall(x, rowNumber, NORTH) ? WALL_SYMBOL : FREE_SYMBOL);
    }
    fprintf(outfile, WALL_SYMBOL);
    fprintf(outfile, "\n");

    for (int x = 0; x < mazeWidth; x++) {
        fprintf(outfile, getWall(x, rowNumber, WEST) ? WALL_SYMBOL : FREE_SYMBOL);
        if (isStartTile(x, rowNumber)) fprintf(outfile, START_SYMBOL);
        else if (isEndTile(x, rowNumber)) fprintf(outfile, END_SYMBOL);
        else fprintf(outfile, FREE_SYMBOL);
    }
    fprintf(outfile, getWall(mazeWidth-1, rowNumber, EAST) ? WALL_SYMBOL : FREE_SYMBOL);
    fprintf(outfile, "\n");
}

/**
 * Print each row after each other, followed by the last wall row.
 */
void fPrintMaze() {
    for (int y = 0; y < mazeHeight; y++) {
        fPrintRow(y);
    }

    for (int x = 0; x < mazeWidth; x++) {
        fprintf(outfile, WALL_SYMBOL);
        fprintf(outfile, getWall(x, mazeHeight-1, SOUTH) ? WALL_SYMBOL : FREE_SYMBOL);
    }
    fprintf(outfile, WALL_SYMBOL);
    fprintf(outfile, "\n");
}

void cfprintf(FILE* stream, enum colours colour, char* stringToColour) {
    if (use_colours) {
        switch (colour) {
            case RED:
                fprintf(stream, "%s", ANSI_COLOR_RED);
                break;
            case GREEN:
                fprintf(stream, "%s", ANSI_COLOR_GREEN);
                break;
            case YELLOW:
                fprintf(stream, "%s", ANSI_COLOR_YELLOW);
                break;
            case BLUE:
                fprintf(stream, "%s", ANSI_COLOR_BLUE);
                break;
            case MAGENTA:
                fprintf(stream, "%s", ANSI_COLOR_MAGENTA);
                break;
            case CYAN:
                fprintf(stream, "%s", ANSI_COLOR_CYAN);
                break;
        }
    }

    fprintf(stream, "%s", stringToColour);

    if (use_colours) {
        fprintf(stream, "%s", ANSI_COLOR_RESET);
    }
}