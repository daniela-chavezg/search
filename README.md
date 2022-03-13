CS-332: Hw3

Purpose: 

Implemented a program called search that traverses a directory and prints out its contents accordingly. The program also supports three command line options, -S, -s, -f. The command line options work seperately and in any combination. 

For Hw3 we were tasked with adding a new functionality to this program. The new functionality performs a UNIX command for each file and ignores directories. This -e option also works in combination with any of the -S, -s, -f, options.
Fork/exec/wait were used to create a new process and execute the UNIX command.

Note: 

This program implements every requirement except the output format of the directory.
I used my own Hw2 for this program.

How to compile: 

'gcc -o search search.c' or 'make'

To execute (any combination of options):

./search -s (file size in bytes) -f (string pattern) -e ("UNIX command")

Example: ./search -s 0 -f README -e "ls -l"

Author:
Daniela Chavez

Credits:
Dr. Unan Lectures
cppreference.com
linux manual
provided sample code