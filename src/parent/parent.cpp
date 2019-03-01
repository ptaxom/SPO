#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

int main(int argc, char **argv)
{

	time_t ltime;
	pid_t pid = fork();

	int st;

	switch(pid)
	{
		case -1: {
			std::cout << "Error 2" << std::endl;
			break;
		}

		case 0: {
			if(execlp("./child",NULL) == -1)
				std::cout << "Error 1" << std::endl;
			break;
		}

		default: {
			for(int i = 0; i < 5; i++)
			{
				std::cout << "parent" << std::endl;
				sleep(1);
				if(waitpid(pid,&st,WNOHANG)>0)
					break;	                                
			}
			std::cout << "The child process was terminated.\n\n";
			if (WIFEXITED(st))
			{
				std::cout << "1";
			}
		}
	}
	return 0;
} 
