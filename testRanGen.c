#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main()
{
	int fd;

	// Open user-defined LKM
	fd = open("/dev/random_generator", O_RDWR);
	if (fd < 0)
	{
		perror("Failed to Open Device.\n");
		return errno;
	}

	// Read random numbers from the device
	
}

