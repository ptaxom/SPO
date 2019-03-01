#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>

int main()
{
	time_t ltime;
	

	for(int i = 0; i < 10; i++)
	{
		std::cout << "child\n";
		usleep(300*1000);

	}
	return 0;
}  
