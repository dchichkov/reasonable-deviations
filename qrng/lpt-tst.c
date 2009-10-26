#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/ppdev.h>
#include <linux/parport.h>



int main(int argc, char *argv[])
{
	unsigned char buf[1000000];
	time_t T0 = time(NULL);

	int fd, mode;
	int buff[1];
	if ((fd=open("/dev/parport0", O_RDWR))<0)
	{
		printf("\nnu deschide\n");
		exit(0);
	}
	
	if (ioctl(fd, PPCLAIM)<0)
	{
		printf("\nnu mi-l claimuieste\n");
		perror("PPCLAIM");
		close(fd);
		exit(0);
	}
	
	mode = IEEE1284_MODE_ECP;
	if (ioctl(fd, PPSETMODE, &mode)<0)
	{
		printf("\nnu seteaza modu ecp\n");
		perror("PPSETMODE");
		close(fd);
		exit(0);
	}
	
	printf("\nwrite\n");
	write(fd,"123",3);
	sleep(1);
	buff[0]=0;
	printf("\nwrite\n");
	write(fd, buff,1);
	
	if (ioctl(fd, PPRELEASE)<0)
	{
		printf("\nnu face release\n");
		close(fd);
	} 
   printf("\n T = %d\n", (int)(time(NULL) - T0));
}
