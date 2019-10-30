/*
	A program that passes a string to the LKM and reads the response from the LKM.
	The device must be called '/dev/ebbchar'
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_LENGTH 256;
static char receive[BUFFER_LENGTH];

int main()
{
	int ret, fd;
	char stringToSend[BUFFER_LENGTH];

	printf("Starting device test code example.\n");

	fd = open("/dev/ebbchar", O_RDWR);
	if (fd < 0)
	{
		perror("Failed to open the device.\n");
		return errno;
	}

	printf("Type in a short string (no more than 256 chars) to the kernel module:\n");
	scanf("%[^\n]%*c", stringToSend);

	printf("Writing message to the device [%s].\n", stringToSend);
	ret = write(fd, stringToSend, strlen(stringToSend));
	if (ret < 0)
	{
		perror("Failed to write message to the device.\n");
		return errno;
	}

	printf("Press <ENTER> to read the message back from the device.\n");
	getchar();

	printf("Reading from the device.\n");
	ret = read(fd, receive, BUFFER_LENGTH);
	if (ret < 0)
	{
		perror("Failed to read the message from the devie.\n");
		return errno;
	}
	printf("The received message is [%s].\n", receive);

	printf("End of testebbchar.c.\n");

	return 0;
}