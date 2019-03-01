#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <complex>
#include <string>
#include <string.h>
#include <vector>
#include <stdlib.h>

std::vector<std::complex<double>> points = 
{
{-1.5, 1},  {0, 0},
{0, 1},     {1.5, 0},
{-1.5, 0},  {0, -1},
{0, 0},    {1.5, 1}};


int width = 600;
int height = 400;

std::vector<int> offsets = {0, 0, width, 0, 0, height, width, height};

template <class T>
char* str_cast(T val)	
{
	std::string str = std::to_string(val);
	char* cArr = (char*)malloc(str.length()+1);
	cArr[str.length()] = '\0';
	for(int i = 0; i < str.length(); i++)
		cArr[i] = str[i];
	return cArr;
}


void run_process(int num)
{
	char **args = (char**)malloc(sizeof(char*) * 8);
	args[0] = str_cast(offsets[2 * num]);
	args[1] = str_cast(offsets[2 * num + 1]);
	args[2] = str_cast(width);
	args[3] = str_cast(height);
	args[4] = str_cast(points[2 * num].real());
	args[5] = str_cast(points[2 * num].imag());
	args[6] = str_cast(points[2 * num + 1].real());
	args[7] = str_cast(points[2 * num + 1].imag());
	execvp("./child", args);
}

int main(int argc, char **argv)
{
	double x0 = -1.5, y0 = 1, x1 = 1.5, y1 = -1;
	std::vector<int> order = {3, 2, 1, 0};

	int threads = 0;
	for(int i = 0; i < 4; i++)
	{

			pid_t cur_pid = fork();
			if (cur_pid == 0){
				run_process(order[i]);

			}
				
	}
	int status;
	waitpid(0, &status, WNOHANG);

	return 0;
}
