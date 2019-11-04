#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_LENGTH 4
static char receive[BUFFER_LENGTH];

int main()
{
	int fd, ret;

	// Open user-defined LKM
	fd = open("/dev/random_generator", O_RDWR);
	if (fd < 0)
	{
		perror("Failed to Open Device.\n");
		return errno;
	}

	// Read random numbers from the device
	printf("Starting to read random number from random_generator.ko...\n");
	ret = read(fd, receive, BUFFER_LENGTH);
	if (ret < 0)
	{
		printf("Failed to read random number from device.\n");
		return errno;
	}

	printf("Received message: %s.\n", receive);

	return 0;
}

