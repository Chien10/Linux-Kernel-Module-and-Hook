#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <string.h>

int main()
{
	char name[32];
	
	puts("Enter a process to find: ");
	scanf("%s", name);

	strtok(name, "\n");

	long int status = syscall(320, name);

	printf("System call returned %ld\n", status);

	return 0;
}