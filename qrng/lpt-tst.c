#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/io.h>

#define LPT1 0x378
#define LPT2 0x278



int main(int argc, char *argv[])
{
  int i;
  time_t T0 = time(NULL);

  ioperm(LPT1, 1, 1);	// 1 port, true
  for(i = 0; i < 1000000; i++)
  {
	  outb(0x01, LPT1);
	  outb(0x00, LPT1);
  }

   printf("\n%d\n", (int)(time(NULL) - T0));


}
