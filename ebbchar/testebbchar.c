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

