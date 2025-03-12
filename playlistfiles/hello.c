#include <linux/init.h>
#include <linux/module.h>

static int helloadd(void){
	printk("Hello Kernel\n");
	return 0;
}

static void helloremove(void){
	printk("Goodbye Kernel\n");
}

module_init(helloadd);
module_exit(helloremove);

MODULE_LICENSE("GPL");

