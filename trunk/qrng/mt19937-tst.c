#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifdef WIN32
#include "getopt.h"
#else
#include <unistd.h>
#endif

#include "mt19937.h"



/*
* MT19937 sanity check
*/
int main(int argc, char *argv[])
{
	const int bytesnum = 10000;
	const long long NN = 500000000000LL;
	int i,j,k, cnt;
	long long CC = 0;
	long long C = 0;

    unsigned long init[3]={6, 9, 42}, length=3;
    init_by_array(init, length);

	while(C < NN)
	{
		for(i = 0; i < bytesnum; i++)
		{
			unsigned char byte = genrand_int8();
			for(j = 0; j < 8; j++)
			{
				unsigned char bit = (byte >> j) & 0x01;
				C++; CC += bit;
			}
		}
		if(C % (bytesnum * 100000) == 0)
			printf("\n%2.15Lf, %15lld, %15lld", (long double)CC/(long double)C, C, CC);
	}

	return 0;
}
