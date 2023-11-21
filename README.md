# CMaze

## Larger mazes
If you wish to make a maze larger than approximately 500 by 500, You will likely hit a stack overflow.
Until I figure out where the memory problem is, You may try one of the hotfixes below:

- If you're using CMake, simply increasing the value in line 16 will increase the stack size used during compilation.
- Otherwise, if the program is compiled on Linux, setting the "LINUX_INCREASE_STACK_SIZE" flag to 1 will increase the stack size to the value defined by the "LINUX_STACK_SIZE" statement (in MB). (both flags are in common.h)
