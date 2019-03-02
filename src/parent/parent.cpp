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

int width = 1000;
int height;
double x_start = -1.5, y_start = 1, x_end = 1.5, y_end = -1;
int parts = 2;

std::vector<std::complex<double>> points;

std::vector<int> offsets;

template <class T>
char *str_cast(T val)
{
	std::string str = std::to_string(val);
	char *cArr = (char *)malloc(str.length() + 1);
	cArr[str.length()] = '\0';
	for (int i = 0; i < str.length(); i++)
		cArr[i] = str[i];
	return cArr;
}

void run_process(int num)
{
	char **args = (char **)malloc(sizeof(char *) * 8);
	args[0] = str_cast(offsets[2 * num]);
	args[1] = str_cast(offsets[2 * num + 1]);
	args[2] = str_cast(width / parts);
	args[3] = str_cast(height / parts);
	args[4] = str_cast(points[2 * num].real());
	args[5] = str_cast(points[2 * num].imag());
	args[6] = str_cast(points[2 * num + 1].real());
	args[7] = str_cast(points[2 * num + 1].imag());
	execvp("./child", args);
}

void init()
{
	double dx = (x_end - x_start) / (double)parts;
	double dy = (y_end - y_start) / (double)parts;
	height = -width * dy / dx;
	int dh = height / parts + 35;
	int dw = width / parts ;
	for (int y = 0; y < parts; y++)
		for (int x = 0; x < parts; x++)
		{
			points.push_back({x_start + dx * x, y_start + dy * y});
			points.push_back({x_start + dx * x + dx, y_start + dy * y + dy});
			offsets.push_back(x * dw);
			offsets.push_back(y * dh);
		}
}

int main(int argc, char **argv)
{
	if (argc == 3)
	{
		try
		{
			width = std::stoi(argv[1]);
			parts = std::stoi(argv[2]);
			if (width < 100 || width > 1600)
				throw std::runtime_error("Invalid width!");
			if (parts < 1 || parts > 4)
				throw std::runtime_error("Invalid parts");
			init();
			for (int i = 0; i < parts * parts; i++)
			{

				pid_t cur_pid = fork();
				if (cur_pid == 0)
				{
					usleep(1000 * 100);
					run_process(parts * parts - i - 1);
				}
			}
			int status;
			waitpid(0, &status, WNOHANG);
		}
		catch(const std::exception &ex)
		{
			std::cout << "Exception: " << ex.what() << std::endl; 
		}
	}

	return 0;
}
