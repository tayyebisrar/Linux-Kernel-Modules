/*
 * Using all the module info such as MODULE_LICENSE() and MODULE_AUTHOR()
 *
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#define DRIVER_AUTHOR "Tayyeb Israr"
#define DRIVER_DESC "This is a sample driver"

static int hellonumber __initdata = 4;

static int __init init_hello_4(void){
	printk(KERN_INFO "Hello, world %d\n", hellonumber);
	return 0;
}

static void __exit cleanup_hello_4(void){
	printk(KERN_INFO "Goodbye, world\n");
}

module_init(init_hello_4);
module_exit(cleanup_hello_4);

MODULE_LICENSE("GPL");

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);

