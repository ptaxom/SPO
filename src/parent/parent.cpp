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


		pid_t pid = fork();
		int st;
		switch (pid)
		{
		case -1:
		{
			std::cout << "Error 2" << std::endl;
			break;
		}

		case 0:
		{
			if (execlp("./child", NULL) == -1)
				std::cout << "Error 1" << std::endl;
			break;
		}
		default:
			{
			for(int i = 0; i < 50; i++)
			{
				sleep(1);
				std::cout << i << std::endl;
				if(waitpid(pid,&st,WNOHANG)>0)
					break;                                
			}			
			std::cout << "The child process was terminated." << std::endl;
			if (WIFEXITED(st))
			{
				std::cout <<  ((int)WEXITSTATUS(st)) << std::endl;
			}
			}
		}
	
	return 0;
}
