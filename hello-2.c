/*
 *  hello-2.c - Demonstrate the module_init() and module_exit() macros
 *  which is preferred over using init_module() and cleanup_module(), which are depreceated
 *
 */

#include <linux/module.h>  // For any module
#include <linux/kernel.h>  // For KERN_INFO
#include <linux/init.h>    // For the new macros


static int __init hello_2_init(void)
{
	printk(KERN_INFO "Hello, world 2\n");
	return 0;
}

static void __exit hello_2_exit(void)
{
	printk(KERN_INFO "Goodbye, world\n");
}

module_init(hello_2_init);
module_exit(hello_2_exit);

MODULE_LICENSE("GPL");
