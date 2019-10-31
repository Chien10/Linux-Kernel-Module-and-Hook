//http://derekmolloy.ie/writing-a-linux-kernel-module-part-2-a-character-device/

#include <linux/init.h>           // Macros used to mark up functions e.g. __init __exit
#include <linux/module.h>         // Core header for loading LKMs into the kernel
#include <linux/device.h>         // Header to support the kernel Driver Model
#include <linux/kernel.h>         // Contains types, macros, functions for the kernel
#include <linux/fs.h>             // Header for the Linux file system support
#include <linux/uaccess.h>        // Required for the copy to user function
#include <linux/mutex.h>          // Mutex functionality

#define  DEVICE_NAME "ebbchar"
#define  CLASS_NAME  "ebb"

// This module is not multi-process safe
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Minh Chien T. and Dat T.");
MODULE_DESCRIPTION("A simple Linux char device driver");
MODULE_VERSION("1.1");

static int    majorNumber;
// Fixed message size
static char   message[256] = {0};           // Memory for the string that is passed from userspace
static short  size_of_message;              // Used to remember the size of the string stored
static int    numberOpens = 0;              // Counts the number of times the device is opened
static struct class*  ebbcharClass  = NULL; // The device-driver class struct pointer
static struct device* ebbcharDevice = NULL; // The device-driver device struct pointer

// The prototype functions for the character driver -- must come before the struct definition
static int dev_open(struct inode *, struct file *);
static int dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

static DEFINE_MUTEX(ebbchar_mutex);  /// A macro that is used to declare a new mutex that is visible in this file
                                     /// results in a semaphore variable ebbchar_mutex with value 1 (unlocked)
                                     /// DEFINE_MUTEX_LOCKED() results in a variable with value 0 (locked)

/* 	Devices are represented as file structure in the kernel. The file_operations structure from
 *  /linux/fs.h lists the callback functions that you wish to associated with your file operations
 *  using a C99 syntax structure. char devices usually implement open, read, write and release calls
*/
static struct file_operations fops =
{
   .read = dev_read,
   .write = dev_write,
   .open = dev_open,
   .release = dev_release
};

static int __init ebbchar_init(void)
{
	printk(KERN_INFO "EBBChar: Initializing the EBBChar LKM.\n");

	// Dynamically allocate a major number for the device
	majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
	if (majorNumber < 0)
	{
		printk(KERN_ALERT "EBBChar failed to register a major number.\n");
		return majorNumber;
	}
	printk(KERN_INFO "EBBChar registered correctly with a major number: %d.\n", majorNumber);

	// Register the device class
	ebbcharClass = class_create(THIS_MODULE, CLASS_NAME);
	if (IS_ERR(ebbcharClass))
	{
		unregister_chrdev(majorNumber, DEVICE_NAME);
		printk(KERN_ALERT "Failed to register device class.\n");
		return PTR_ERR(ebbcharClass);
	}
	printk(KERN_INFO "EBBChar: device class created correctly.\n");

	// Register the device driver
	ebbcharDevice = device_create(ebbcharClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
	if (IS_ERR(ebbcharDevice))
	{
		class_destroy(ebbcharClass);
		unregister_chrdev(majorNumber, DEVICE_NAME);
		printk(KERN_ALERT "Failed to register device driver.\n");
		return PTR_ERR(ebbcharDevice);
	}
	printk(KERN_INFO "EBBChar: device driver created correctly.\n");

  // Initialize the mutex lock dynamically at runtime
  mutex_init(&ebbchar_mutex);

	return 0;
}

static void __exit ebbchar_exit(void)
{
  mutex_destroy(&ebbchar_mutex);

	device_destroy(ebbcharClass, MKDEV(majorNumber, 0));
	class_unregister(ebbcharClass);
	class_destroy(ebbcharClass);

	unregister_chrdev(majorNumber, DEVICE_NAME);

	printk(KERN_INFO "EBBChar: LKM said goodby to you!\n");
}

static int dev_open(struct inode *inodep, struct file *filep)
{
  if (!mutex_trylock(&ebbchar_mutex))
  {
    printk(KERN_ALERT "EBBChar: Device is used by other process.\n");
    return -EBUSY;
  }

	numberOpens++;

	printk(KERN_INFO "EBBChar: Device has been opened %d times.\n", numberOpens);

	return 0;
}

static int dev_release(struct inode *inodep, struct file *filep)
{
  mutex_unlock(&ebbchar_mutex);
	// Called when the device is released/ closed by the user-space program
	printk(KERN_INFO "EBBChar: Device successfully closed.\n");
	return 0;
}

static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
	// Called when data sent from the device to a user in user space
	int err_count = 0;

	// copy_to_user(*to, *from, size)
	err_count = copy_to_user(buffer, message, size_of_message);

	if (err_count == 0)
	{
		printk(KERN_INFO "EBBChar: Sent %d characters to the user.\n", size_of_message);

		size_of_message = 0; // Clear the position to the start
		return size_of_message;
	}

	printk(KERN_INFO "EBBChar: failed to send %d characters to the user.\n", err_count);
	return -EFAULT; // -14 bad address message
}

static ssize_t dev_write(struct  file *filep, const char *buffer, size_t len, loff_t *offset)
{
	sprintf(message, "%s(%zu letters)", buffer, len);
	size_of_message = strlen(message);
	printk(KERN_INFO "EBBChar: received %zu characters from the user.\n", len);
	return len;
}

module_init(ebbchar_init);
module_exit(ebbchar_exit);

// This module's Makefile includes a user-space C program that interacts with it
