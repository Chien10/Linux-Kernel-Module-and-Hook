#include <linux/module.h>
#include <linux/kernel.h>

#define DRIVER_AUTHOR "Minh Chien <chientrangminh@gmail.com> and Thanh Dat"
#define DRIVER_DESC "A loadable kernel module for novice"
#define LI "MIT/GPL"
#define DEVICE "testdevice"


static int __init init_hi(void)
{
  printk(KERN_INFO "Hi Chien.\n");
  return 0;
}

static void __exit exit_hi(void) {
  printk(KERN_INFO "Bye Chien.\n");
}

module_init(init_hi);
module_exit(exit_hi);

MODULE_LICENSE(LI);
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_SUPPORTED_DEVICE(DEVICE);
