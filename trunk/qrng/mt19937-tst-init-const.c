#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

#include "mt19937.h"



/*
* MT19937 sanity check
*/
int main(int argc, char *argv[])
{
	const int bytesnum = 10000;
	const long long NN = 100000000000LL;
	int i,j,k, cnt;
	long long NS = 0, S = 0;
	long long NC = 0, C = 0, B = 0;

     unsigned int byte_popcount[256];
    unsigned long init[3]={6, 9, 42}, length=3;
    init_by_array(init, length);

       // Init byte popcount
        for(i = 0; i < 256; i++)
        for(byte_popcount[i] = 0, k = 0; k < 8; k++)
		// if((i) & (1 << k)) byte_popcount[i]++;
		// if((i ^ 0x55) & (1 << k)) byte_popcount[i]++;
		// if((i ^ 0xAA) & (1 << k)) byte_popcount[i]++;
			if((i ^ 0xF0) & (1 << k)) byte_popcount[i]++;


	while(NC < NN)
	{
		for(i = 0; i < bytesnum; i++)
		{
			S +=  byte_popcount[genrand_int8()]; 
		}
		
		C += bytesnum * 8;
		NC += bytesnum * 8;		
				
		if(NC % 100000 == 0)
		{
			S++;
			// if(NC / 1000000000LL == 0) {S++; B++;}
		}
		if(NC % 100000000LL == 0)
		{
			NS += S;
			
			long double E = 0.5 + (2.0 / 2.0 / sqrt((long double)NC));
			long double N_MEAN = (long double)NS/(long double)NC;
			long double MEAN = (long double)S/(long double)C;
			
			printf("\n%15lld, %2.15Lf, %2.15Lf, %2.15Lf, %lld, %lld, %d", NC, N_MEAN, E, MEAN, C, B, MEAN > 0.5);
			B = 0; C = 0; S = 0;
			
		}
	}

	return 0;
}
