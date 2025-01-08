/*
 * Showing how modules can span multiple files
 * This is the second of 2 files - start.c and stop.c
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

static void __exit cleanuo(void){
	printk(KERN_INFO "Goodbye, world (stop)\n");
}

module_exit(cleanuo);

MODULE_LICENSE("GPL");
