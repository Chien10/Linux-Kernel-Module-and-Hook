// http://www.tldp.org/LDP/lkmpg/2.6/html/x569.html

/*
	A read-only character device tracks how many times a user've read from a dev file
*/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/unaccess.h>

#define SUCCESS 0
#define DEVICE_NAME "chardev"
#define BUF_LEN 80

static int Major;
static int Device_Open = 0;

static struct file_operations fops = {
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release
};

int init_module(void)
{
	Major = register_chdev(0, DEVICE_NAME, &fops);

	if (Major < 0)
	{
		printk(KERN_ALERT "Registering char device failed with %d\n", Major);
		return Major;
	}

	printk(KERN_INFO "I was assigned major number %d. To talk to\n", Major);
	printk(KERN_INFO "the driver, create a dev file with\n");
	printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, Major);
	printk(KERN_INFO "Try various minor numbers. Try to cat and echo to\n");
	printk(KERN_INFO "the device file.\n");
	printk(KERN_INFO "Remove the device file and module when done.\n");

	return SUCCESS;
}

void cleanup_module(void)
{
	int ret = unregister_chrdev(Major, DEVICE_NAME);
	if (ret < 0) {
		printk(KERN_ALERT "Error in unregister_chrdev: %d\n", ret);
	}
}

static int device_open(struct inode *inode, struct file *file)
{
	// Called when a process tries to open the device file
	static int counter = 0;

	if (DEVICE_OPEN) {
		return -EBUSY;
	}

	Device_Open++;

	sprintf(msg, "I've already told you %d times Hi Chian!\n", counter++);
	msg_Ptr = msg;
	try_module_get(THIS_MODULE);

	return SUCCESS;
}

static int device_release(struct inode *inode, struct file *file)
{
	// Called when a process closes the device file
	Device_Open--;

	module_put(THIS_MODULE);

	return 0;
}

static ssize_t device_read(struct file *filp, char *buffer, ssize_t length, loff_t *offset)
{
	// Called when a process opened a device and want to read from the device
	int bytes_read = 0; // Actual bytes written to the buffer

	if (*msg_Ptr == 0) {
		// End of file
		return 0;
	}

	// Put the data into the buffer
	while (length && *msg_Ptr)
	{
		// Copy data from the kernel data segment to the user data segment
		// Buffer is in user space
		put_user(*(msg_Ptr++), buffer++);
		length--;
		bytes_read++;
	}

	// Number of bytes put into the buffer
	return bytes_read;
}

static ssize_t device_write(struct file *filp, const char *buff, ssize_t len, loff_t *off)
{
	// Called when a process writes to dev file: echo "hi" > /dev/hi
	printk(KERN_ALERT "Sorry, this opreation isn't supported yet.\n");
	return -EINVAL;
}

// This is by convention. When writing a driver, it's OK to put the device file in your current directory. Just make sure you place it in /dev for a production driver