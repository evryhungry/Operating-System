========================================
Operating Systems Homework #1
Student Name: Kim Dohyun
Student ID: 21800063
========================================

1. Program Description
------------------------
This program implements a word search tool using inter-process communication (IPC) via a pipe.

- A child process executes a shell command.
- A parent process reads the child’s output through a pipe.
- The parent searches for lines containing a specific word.
- Matching lines are printed with:
  - Green line numbers
  - Red-highlighted search word

2. Compilation
------------------------
Target system: Ubuntu 24.04 LTS  
To compile the program, simply run:
make

- make or make all
    Compiles the source code (wspipe.c) and generates the executable file named 'wspipe'.
    Uses gcc with flags: -Wall -Wextra -g

- make clean
    Removes the compiled executable file 'wspipe' to clean the working directory.
    Equivalent to: rm -f wspipe

3. Creative Service: --save Option
------------------------
This program includes a creative feature worth 1 point: the --save option.

Description:
When the user appends --save to the command-line arguments, matched lines are also saved to a file named result.txt.

Example usage:
./wspipe "curl -s https://man7.org/linux/man-pages/man2/fork.2.html" child --save

Saved output (result.txt):
[5] The child process is created using fork().
[12] The child inherits file descriptors from the parent.

Implementation details:
- --save is detected using a manual my_strcmp function.
- A flag save_enabled controls saving behavior.
- Matching lines are written to result.txt using write() (not fprintf() or snprintf()).
- Buffer overflow is prevented using bounds checks.

Output file: result.txt  

5. Submission Contents
------------------------
The zip file should include the following:

- wspipe.c
- Makefile
- README.md
========================================