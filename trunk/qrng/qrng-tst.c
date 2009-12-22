#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "quantis.h"



/*
* Simple QRNG bias test
*/
int main(int argc, char *argv[])
{
	const int cardnumber = 0;
	int i,j,k, cnt;
	long long CC = 0;
	long long C = 0;

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
	

	// Init byte popcount
	for(i = 0; i < 256; i++)
	for(byte_popcount[i] = 0, k = 0; k < 8; k++)
		if((i) & (1 << k)) byte_popcount[i]++;


	while(C < 100000000000LL)
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
			CC += byte_popcount[buffer[i]];
		}
		C += bytesnum * 8;
		if(C % (bytesnum * 10000LL) == 0)
		{
			long double E = (3.0 / 2.0 / sqrt((long double)C));			
			printf("\n%lld, %2.15Lf, %2.15Lf, %2.15Lf, %lld", C, (long double)CC/(long double)C, 0.499975+E, 0.499975-E, CC);
		}
	}

	return 0;
}


