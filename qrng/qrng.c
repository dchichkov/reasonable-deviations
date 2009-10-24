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



/*
* Example RD Experiment
*/
int main(int argc, char *argv[])
{
	int cardnumber = 0;
	unsigned int bytesnum = 1024;
	unsigned int byte_popcount[256];
	int i,j,k, cnt;
	long long CC = 0;
	long long C = 0;
	long double S = 0;
	time_t T0;	

	unsigned char *buffer = (unsigned char *) malloc(bytesnum);
	if (buffer == NULL) {
		fprintf(stderr, "bytes number too big!\n");
		exit(EXIT_FAILURE);
	}

	for(i = 0; i < 256; i++)
	for(byte_popcount[i] = 0, k = 0; k < 8; k++)
		// if((i ^ 0x55) & (1 << k)) byte_popcount[i]++;
		if((i ^ 0xAA) & (1 << k)) byte_popcount[i]++;

	cnt = quantisCount();

	if (!cnt || (cnt < cardnumber+1)) {
		fprintf(stderr, "card %d not found\n",cardnumber);
		exit(EXIT_FAILURE);
	}

	if (quantisBoardReset(cardnumber)){
		fprintf(stderr, "cannot reset card %d\n",cardnumber);
		exit(EXIT_FAILURE);
	}
	T0 = time(NULL);

	while(1)
	{
		if (bytesnum != (unsigned) quantisRead(cardnumber, (char *) buffer, bytesnum)){
			fprintf(stderr, "an error occured when reading card %d\n", cardnumber);

			if (quantisBoardReset(cardnumber)){
				fprintf(stderr, "cannot reset card %d\n",cardnumber);
				exit(EXIT_FAILURE);
			}

			continue;
		}

		for(i = 0; i < bytesnum; i++)
		{
			int K = byte_popcount[buffer[i]];
			C += 8; CC += K; K <<= 7;
			for(k = 0; k < K; k++) S += sqrt(42);
		}
		
		if(C % (bytesnum * 1000000) == 0)
			printf("\n%2.15Lf, %lld, %lld, %lld, %Lf", (long double)CC/(long double)C, C, CC, C/(time(NULL) - T0), S);
	}

	return 0;
}
