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
* This is an exemple tool to get random numbers via the
* Quantis PCI card using the quantis library
*/
int main(int argc, char *argv[])
{
	int cardnumber = 0;
	unsigned int bytesnum = 10000;
	int i,j,k, cnt;
	long long CC = 0;
	long long C = 0;

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
			unsigned char byte = buffer[i] ^ 0x55;
			for(j = 0; j < 8; j++)
			{
				unsigned char bit = (byte >> j) & 0x01;
				C++; CC += bit;
				if(bit) for(k = 0; k < 200; k++) sqrt(42);
			}
		}
		if(C % (bytesnum * 100000) == 0)
			printf("\n%2.15Lf, %15Lf", (long double)CC/(long double)C, (long double)C);
	}

	return 0;
}
