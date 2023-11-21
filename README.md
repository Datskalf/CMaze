# CMaze

## Larger mazes
If you wish to make a maze that leads to a stack overflow, there are a couple hotfixes until I get around ot fixing this issue.
If you're using CMake, simply increasing the value in line 16 will increase the stack size used during compilation.
Otherwise, if the program is compiled on Linux, setting the "LINUX_INCREASE_STACK_SIZE" flag to 1 will increase the stack size to the value defined by the "LINUX_STACK_SIZE" statement.
