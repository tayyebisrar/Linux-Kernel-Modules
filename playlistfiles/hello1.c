#include <linux/init.h>
#include <linux/module.h>

static int __init helloadd(void){
	printk("Hello Kernel\n");
	return 0;
}

static void __exit  helloremove(void){
	printk("Goodbye Kernel\n");
}

module_init(helloadd);
module_exit(helloremove);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tayyeb Israr");
MODULE_DESCRIPTION("A simple Hello World Linux Kernel Module");

