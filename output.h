/**
 * @author Datskalf
 * @version 1.1
 * @date 2023-11-09
 */

#ifndef MAZEGENERATOR_OUTPUT_H
#define MAZEGENERATOR_OUTPUT_H

enum colours {
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN
};

void set_stream(FILE* stream);
void open_file(char* fp);
void fPrintMaze();
void cfprintf(FILE* stream, enum colours colour, char* stringToColour);

#endif
