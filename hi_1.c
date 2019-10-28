#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/stat.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Minh Chien T. and T.Dat");

static short int myShort = 1;
static int myInt = 420;
static long int myLong = 9999;
static char *myString = "budgie"
static int myIntArray = {-3, -3};
static int arr_argc = 0;

/*
	module_param(
				parameter name,
				data type of the parameter,
				permission bits)
*/
module_param(myShort, short, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARAM_DESC(myShort, "A short integer");
module_param(myInt, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(myInt, "An integer");
module_param(myLong, long, S_IRUSR);
MODULE_PARM_DESC(myLong, "A long integer");
module_param(myString, charp, 0000);
MODULE_PARM_DESC(myString, "A character string");

/*
	module_param_array(
					   parameter name,
					   date type of elements in the parameter,
					   pointer to variable storing elements of the
					   array initialized by the user at module loading time,
					   permission bits)
*/
module_param_array(myIntArray, int, &arr_argc, 0000);
MODULE_PARM_DESC(myIntArray, "An array of integers");

static int __init hi_2_init(void)
{
	int i;

	printk(KERN_INFO "Hi, Chien 2\n============\n");
	printk(KERN_INFO "myShort is a short integer: %d\n", myShort);
	printk(KERN_INFO "myInt is an integer: %d\n", myInt);
	printk(KERN_INFO "myLong is a long integer: %ld\n", myLong);
	printk(KERN_INFO "myString is a string: %s\n", myString);
	for (i = 0; i < (sizeof myIntArray / sizeof(int)); ++i) {
		printk(KERN_INFO "myIntArray[%d] = %d\n", i, myIntArray[i]);
	}
	printk(KERN_INFO "Got %d arguments for myIntArray.\n", arr_argc);

	return 0;
}

static void __exit hi_2_exit(void) {
	printk(KERN_INFO "Goodbye, Chien 2.\n");
}

module_init(hi_2_init);
module_exit(hi_2_exit);
