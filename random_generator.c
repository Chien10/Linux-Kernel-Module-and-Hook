#include <linux/init.h>           // Macros used to mark up functions e.g. __init __exit
#include <linux/module.h>         // Core header for loading LKMs into the kernel
#include <linux/device.h>         // Header to support the kernel Driver Model
#include <linux/kernel.h>         // Contains types, macros, functions for the kernel
#include <linux/fs.h>             // Header for the Linux file system support
#include <linux/uaccess.h>        // Required for the copy to user function
#include <linux/mutex.h>    	  // Mutex functionality

#include <stdlib.h>
#include <time.h>

#define DEVICE_NAME "RandomGenerator"
#define CLASS_NAME "generator"

MODULE_LICENSE("Dual MIT/GPL");
MODULE_AUTHOR("Minh Chien T. and Thanh Dat N.");
MODULE_DESCRIPTION("A Linux LKM generating random numbers");
MODULE_VERSION("1.0");

static int MajorNumber;
static int deviceOpenTimes = 0;
static struct class *randomGenClass = NULL;
static struct device *randomGenDevice = NULL;

static int     device_open(struct inode *, struct file *);
static int     device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

static DEFINE_MUTEX(ranGenMutex);

static struct file_operations fops = {
	.open = device_open,
	.read = device_read,
	.write = device_write,
	.release = device_release
};

static int __init device_init(void)
{
	printk(KERN_INFO "RandomGenerator: Start initializing RandomGenerator LKM...\n");

	MajorNumber = register_chrdev(0, DEVICE_NAME, &fops);
	if (MajorNumber < 0)
	{
		printk(KERN_ALERT "RandomGenerator Failed to Register a Major Number.\n");
		return MajorNumber;
	}
	printk(KERN_INFO "RandomGenerator: Registered Major Number: %d Successfully.\n" % MajorNumber);

	randomGenClass = class_create(THIS_MODULE, CLASS_NAME);
	if(IS_ERR(randomGenClass))
	{
		unregister_chrdev(MajorNumber, DEVICE_NAME);
		printk(KERN_ALERT "Failed to Register Device Class.\n");
		return PTR_ERR(randomGenClass);
	}
	printk(KERN_INFO "RandomGenerator: Registered Device Class Successfully.\n");

	randomGenDevice = device_create(randomGenClass, NULL, MKDEV(MajorNumber, 0), NULL, DEVICE_NAME);
	if(IS_ERR(randomGenDevice))
	{
		class_destroy(randomGenClass);
		unregister_chrdev(MajorNumber, DEVICE_NAME);
		printk(KERN_ALERT "Failed to Create Device.\n");
		return PTR_ERR(randomGenDevice);
	}
	printk(KERN_INFO "RandomGenerator: Created Device Class Successfully.\n");

	mutex_init(&ranGenMutex);

	return 0;
}

static void __exit device_exit(void)
{
	mutex_destroy(&ranGenMutex);

	device_destroy(randomGenClass, MKDEV(MajorNumber, 0));
	class_unregister(randomGenClass);
	class_destroy(randomGenClass);

	unregister_chrdev(MajorNumber, DEVICE_NAME);

	printk(KERN_INFO "RandomGenerator: LKM Said Goodby to You :))\n");
}

static int device_open(struct inode *inodep, struct file *filep)
{
	isFree = mutex_trylock(&ranGenMutex);
	if (!isFree)
	{
		printk(KERN_ALERT "RandomGenerator: Device Was Being used by Other Process.\n");
		return -EBUSY;
	}

	deviceOpenTimes++;
	printk(KERN_INFO "RandomGenerator: Device has been Opened %d times", deviceOpenTimes);

	return 0;
}

static int ssize_t device_read(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
	int errCount = 0;
	
	int randomNumber;
	get_random_number(&randomNumber, sizeof randomNumber);
	randomNumber %= 250;


}

static int device_release(struct inode *inodep, struct file *filep)
{
	mutex_unclock(&ranGenMutex);

	printk(KERN_INFO "RandomGenerator: Device Successfully Closed.\n");

	retun 0;
}



module_init(device_init);
module_exit(device_exit);

