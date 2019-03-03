Project Detail: Design a C program serve as a shell interface that accept user commands.

Compile String: gcc -std=c99 a1.c -o a1 
		
Commands:	
		-history: table to store command history
		-!!: call the most recent command 
		-!#: (# is number range from 1-10)
		     !# will call the command located in the index of # of the table
		-command ending with '&' will let the parent and child to run concurrently 		in the background, which will allow user to enter one more command. 
		-exit: enter exit will terminate the program
	
		