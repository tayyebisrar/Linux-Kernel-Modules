#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>

static int major;
static char text[64];

// myread ssize_t is the number of bytes read successfully
// struct file is an open file in the kernel - using the pointer we can access stuff like the file permissions, operations, or 'private data' pointer
// char __user is a userspace buffer, as opposed to the text[] buffer in kernel space defined above
// size_t l tells us the length of the buffer
// loff_t is the current file offset pointer. After reading x bytes you increase the pointer by x. Default is 0. Sometimes you won't need this if you dont want to change what you read each time read is called

static ssize_t myread(struct file *filp, char __user *u, size_t len, loff_t *off){
	int not_copied, delta, to_copy;

	if ((len + *off) < sizeof(text)) {
    		to_copy = len;
	} else {
    		to_copy = sizeof(text) - *off;
	}

	printk(KERN_INFO "Read was called - wanted to read %ld bytes. Offset is %lld so we copy %d bytes\n", len, *off, to_copy);

	if (*off >= sizeof(text)){ // if done
		return 0;
	}
	
	// time to copy text[] into userbuffer u[]
	
	not_copied = copy_to_user(u, &text[*off], to_copy); // return value is # of bytes NOT successfully copied.
	delta = to_copy - not_copied;
	if (not_copied > 0) { // if some bytes couldn't be copied
		printk(KERN_WARNING "chardevcomp - could only copy %d bytes\n", delta); 
	}
	
	*off += delta; // increase file offset by the distance copied

	return delta; // return number of bytes read
}

// very similar to read, except user buffer is now constant, and copy_to_user >> copy_from_user (to kernel)
static ssize_t mywrite(struct file *filp, const char __user *u, size_t len, loff_t *off){
	int not_copied, delta, to_copy;

	if ((len + *off) < sizeof(text)) {
    		to_copy = len;
	} else {
    		to_copy = sizeof(text) - *off;
	}

	printk(KERN_INFO "Write was called - wanted to write %ld bytes. Offset is %lld so we copy %d bytes\n", len, *off, to_copy);

	if (*off >= sizeof(text)){ // if done
		return 0;
	}
	
	// time to copy userbuffer u[] into kernel space buffer text[]
	
	not_copied = copy_from_user(&text[*off], u, to_copy); // return value is # of bytes NOT successfully copied.
	delta = to_copy - not_copied;
	if (not_copied > 0) { // if some bytes couldn't be copied
		printk(KERN_WARNING "chardevcomp - could only copy %d bytes\n", delta); 

	}

	*off += delta;

	return delta; // return number of bytes read
}

	

static int myopen(struct inode *inode, struct file *filp){
	printk(KERN_INFO "chardevcomp - Major: %d, Minor: %d\n", imajor(inode), iminor(inode));
	printk(KERN_INFO "chardevcomp - filp->f_pos %lld\n", filp->f_pos);
	printk(KERN_INFO "chardevcomp - flip->f_mode 0x%x\n", filp->f_mode);
	printk(KERN_INFO "chardevcomp - flip->f_flags: 0x%x\n", filp->f_flags);

	return 0;
}

static int myrelease(struct inode *inode, struct file *filp){
	pr_info("chardevcomp - File closed\n");
	return 0;
}

static struct file_operations fops = {
	.read = myread,
	.write = mywrite
};


static int __init helloadd(void){
	major = register_chrdev(0, "chardevcomp", &fops);
	if (major < 0){
		printk(KERN_ALERT "Error registering chardev major with chardevcomp\n");
		return major;
	}

	printk(KERN_INFO "chardevcomp - Major dev number %d\n", major);
	return 0;
}
 
static void __exit  helloremove(void){
	printk(KERN_INFO"Goodbye Kernel\n");
	unregister_chrdev(major, "chardevcomp");
}

module_init(helloadd);
module_exit(helloremove);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tayyeb Israr");
MODULE_DESCRIPTION("64-byte read/write character device driver");

