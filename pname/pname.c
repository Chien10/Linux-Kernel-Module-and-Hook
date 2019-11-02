#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <linux/sched.>
#include <linux/init.h>
#include <linux/tty.h>
#include <linux/string.h>

/* A user-defined system call that when passed a process name, it returns the corresponding PID of the process
	that initialized the syscall. */

asmlinkage long sys_process_name(char *process_name)
{
	struct task_struct *task;
	struct tty_struct *my_tty = get_current_tty();
	char name[32];

	for_each_process(task)
	{
		if(strcmp(task->comm, process_name) == 0)
		{
			sprintf(name ,"PID = %ld\n", (long) task_pid_nr(task));
			(my_tty->driver->ops->write)(my_tty, name, strlen(name) + 1);
		}
	}

	return 0;
}