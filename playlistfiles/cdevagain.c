#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>

static int major;

static ssize_t myread(struct file *f, char __user *u, size_t l, loff_t *o){
	printk(KERN_INFO "Read was called\n");
	return 0;
}

static int myopen(struct inode *inode, struct file *filp){
	printk(KERN_INFO "cdevagain - Major: %d, Minor: %d\n", imajor(inode), iminor(inode));
	printk(KERN_INFO "cdevagain - filp->f_pos %lld\n", filp->f_pos);
	printk(KERN_INFO "cdevagain - flip->f_mode 0x%x\n", filp->f_mode);
	printk(KERN_INFO "cdevagain - flip->f_flags: 0x%x\n", filp->f_flags);

	return 0;
}

static int myrelease(struct inode *inode, struct file *filp){
	pr_info("cdevagain - File closed\n");
	return 0;
}

static struct file_operations fops = {
	.read = myread,
	.open = myopen,
	.release = myrelease
};


static int __init helloadd(void){
	major = register_chrdev(0, "cdevagain", &fops);
	if (major < 0){
		printk(KERN_ALERT "Error registering chardev with cdevagain\n");
		return major;
	}

	printk(KERN_INFO "cdevagain - Major dev number %d\n", major);
	return 0;
}
 
static void __exit  helloremove(void){
	printk(KERN_INFO"Goodbye Kernel\n");
	unregister_chrdev(major, "cdevagain");
}

module_init(helloadd);
module_exit(helloremove);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tayyeb Israr");
MODULE_DESCRIPTION("Registering char device");

