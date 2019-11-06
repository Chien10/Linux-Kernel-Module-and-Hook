#include <linux/syscalls.h> 
#include <linux/kernel.h> 
#include <linux/sched.> 
#include <linux/init.h> 
#include <linux/tty.h> 
#include <linux/string.h> 

asmlinkage long sysProcessName(char *processName)
{
	struct task_struct *task;
	struct tty_struct *myTty = get_current_tty();
	char name[32];

	for_each_process(task)
	{
		if (strcmp(task->comm, processName) == 0)
		{
			sprintf(name, "PID = %ld.\n", (long) task_pid_nr(task));
			(myTty->driver->ops->write)(myTty, name, strlen(name) + 1);
		}
	}

	return 0;
}

