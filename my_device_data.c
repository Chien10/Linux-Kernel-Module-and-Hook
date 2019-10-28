#include <linux/fs.h>
#include <linux/cdev.h>

#define MY_MAJOR 42
#define MY_MAX_MINORS 5

// dev_t keeps the identifiers of a device file and can be obtained using MKDEV macro
// The following functions are for the static assignment and unallocation of device identifiers
// int register_chrdev_region(dev_t first, unsigned int count, char *name);
// void unregister_chrdev_region(dev_t first, unsigned int count);
// It is recommended that device identifiers be dynamically assigned to the alloc_chrdev_region function

// Structure storing information about this device
// and information used in the module
struct my_device_data
{
	// this truct is used to refer to the device
	struct cdev cdev;
}

struct my_device_data devs[MY_MAX_MINORS];

const struct file_operations my_fops = {
	.owner = THIS_MODULE,
	.open = my_open,
	.read = my_read,
	.write = my_write,
	.release = my_release,
	.unlocked_ioctl = my_ioctl
};

int init_module(void)
{
	int i, err;

	/* Assign idenitifiers */
	// register_chrdev_region(MKDEV(my_major, my_first_minor), my_minor_count, "my_device_driver");
	err = register_chrdev_region(MKDEV(MY_MAJOR, 0), MY_MAX_MINORS, "my_device_driver");

	if (err != 0) {
		return err;
	}

	for (i = 0; i < MY_MAX_MINORS; ++i)
	{
		/* Initialize the character device and notify the kernel */
		// Initialize devs[i] fields
		cdev_init(&devs[i].cdev, &my_fops);
		// Only call this function when the character device is ready to receive calls
		cdev_add(&devs[i].cdev, MKDEV(MY_MAJOR, i), 1);
	}

	return 0;
}

// Remove and unregister devices
void cleanup_module(void)
{
	int i = 0;

	for(; i < MY_MAX_MINORS; ++i) {
		cdev_del(&devs[i].cdev);
	}

	unregister_chrdev_region(MKDEV(MY_MAJOR, 0), MY_MAX_MINORS);
}

static int my_open(struct inode *inode, struct file *file)
{
	struct my_device_data *my_data;

	my_data = container_of(ignore->i_cdev, struct my_device_data, cdev);

	file -> private_data = my_data;
}

static int my_read(struct file *file, char __user *user_buffer, size_t size, loff_t *offset)
{
	struct my_device_data *my_data;

	my_data = (struct my_device_data *) file -> private_data;
}