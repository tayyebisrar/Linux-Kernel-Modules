/*
 * Showing how modules can span multiple files
 * This is the 'start' file of 2 - start.c and stop.c
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

static int __init yhman(void){
	printk(KERN_INFO "Hello, world (start)\n");
	return 0;
}

module_init(yhman);
