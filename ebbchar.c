//http://derekmolloy.ie/writing-a-linux-kernel-module-part-2-a-character-device/

#include <linux/init.h>           // Macros used to mark up functions e.g. __init __exit
#include <linux/module.h>         // Core header for loading LKMs into the kernel
#include <linux/device.h>         // Header to support the kernel Driver Model
#include <linux/kernel.h>         // Contains types, macros, functions for the kernel
#include <linux/fs.h>             // Header for the Linux file system support
#include <linux/uaccess.h>          // Required for the copy to user function

#define  DEVICE_NAME "ebbchar"   
#define  CLASS_NAME  "ebb"  

MODULE_LICENSE("GPL");            
MODULE_AUTHOR("Minh Chien T. and Dat T.");    
MODULE_DESCRIPTION("A simple Linux char driver");  
MODULE_VERSION("1.0");

static int    majorNumber;                  
static char   message[256] = {0};           // Memory for the string that is passed from userspace
static short  size_of_message;              // Used to remember the size of the string stored
static int    numberOpens = 0;              // Counts the number of times the device is opened
static struct class*  ebbcharClass  = NULL; // The device-driver class struct pointer
static struct device* ebbcharDevice = NULL; // The device-driver device struct pointer
 
// The prototype functions for the character driver -- must come before the struct definition
/*static int     dev_open(struct inode *, struct file *);
static int     dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);*/
 
/* 	Devices are represented as file structure in the kernel. The file_operations structure from
 *  /linux/fs.h lists the callback functions that you wish to associated with your file operations
 *  using a C99 syntax structure. char devices usually implement open, read, write and release calls
*/
static struct file_operations fops =
{
   .open = dev_open,
   .read = dev_read,
   .write = dev_write,
   .release = dev_release,
}; 