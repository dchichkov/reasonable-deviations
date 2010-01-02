#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifdef WIN32
#include "getopt.h"
#else
#include <unistd.h>
#endif

#include "quantis.h"
#include "mt19937.h"



/*
* Example RD Experiment
*/
int main(int argc, char *argv[])
{
	const int cardnumber = 0;
	int i,j,k, cnt;
	long long CC = 0;
	long long C = 0;
	time_t T0;	

	unsigned long init_seed[624];				// Init MT19937
	const unsigned int bytesnum = 1024;			// QRNG block size
	unsigned int byte_popcount[256];
	unsigned char *buffer = (unsigned char *) malloc(bytesnum);
	
	if (buffer == NULL) {
		fprintf(stderr, "bytes number too big!\n");
		exit(EXIT_FAILURE);
	}

	cnt = quantisCount();

	if (!cnt || (cnt < cardnumber+1)) {
		fprintf(stderr, "card %d not found\n",cardnumber);
		exit(EXIT_FAILURE);
	}

	if (quantisBoardReset(cardnumber)){
		fprintf(stderr, "cannot reset card %d\n",cardnumber);
		exit(EXIT_FAILURE);
	}
	
	
	// Init MT19937
	if (sizeof(init_seed) != (unsigned) quantisRead(cardnumber, (char *) init_seed, sizeof(init_seed)))
	{
			fprintf(stderr, "an error occured when reading card %d\n", cardnumber);
			exit(EXIT_FAILURE);
	}
	for(i = 0; i < sizeof(init_seed) / sizeof(init_seed[0]); i++) printf("0x%0lX, ", init_seed[i]);
	printf("\n");
        init_by_array(init_seed, sizeof(init_seed) / sizeof(init_seed[0]));


	// Init byte popcount
	for(i = 0; i < 256; i++)
	for(byte_popcount[i] = 0, k = 0; k < 8; k++)
		if((i) & (1 << k)) byte_popcount[i]++;


	
	while(C < 3000000000000LL)
	{
		for(i = 0; i < bytesnum; i++)
		{
			CC += byte_popcount[genrand_int8()];
		}
		C += bytesnum * 8;
		if(C % (bytesnum * 1000000LL) == 0)
		{
			long double E = (2.0 / 2.0 / sqrt((long double)C));			
			printf("\n%lld, %2.15Lf, %2.15Lf, %2.15Lf, %lld", C, (long double)CC/(long double)C, 0.5+E, 0.5-E, CC);
		}
	}

	return 0;
}


