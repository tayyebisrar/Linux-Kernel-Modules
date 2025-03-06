/*
 *  chardev.c: Creates a read-only char device which tells us how many times the dev file is read from
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h> // for the file operations, read, write, open and release, and register/unregister_chrdev()
#include <asm/uaccess.h> // used for the put_user() function near the bottom, lets us move data from kernel to userspace

/*
 * Prototypes which would go in a .h file
 *
 */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tayyeb Israr");
MODULE_DESCRIPTION("Character driver which can report how many times a dev file is read from");

int init_module(void);
void cleanup_module(void);

static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *); // ssize_t is a signed size_t integer which tells you about number of bytes read, or error code. loff_t * is a pointer that keeps track of the file offset, to track where the file is being read or written from/to.
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

#define SUCCESS 0
#define DEVICE_NAME "chardev" // name of the driver as seen in /proc/devices
#define BUFF_LEN 80 // max message length

// Global variables (within the file), statically defined

static int Major; // device driver major number
static int Device_Open = 0; // device open?
static char msg[BUFF_LEN]; // Message that will be given by device when prompted
static char *msg_Ptr;

// file_operations struct which is called fops

static struct file_operations fops = {
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release
};

// When the module is loaded, the following stuff will be called

int init_module(void){
	
	int Major = register_chrdev(0, DEVICE_NAME, &fops); // register_chrdev() dynamically assigns the device in proc/devices with an unused major number. 
	
	if(Major < 0){
		printk(KERN_ALERT "Device registering failed with exit code %d", Major);
		return Major;
	}

	printk(KERN_INFO "This driver was successfully assigned major number %d\n", Major);
	printk(KERN_INFO "To interact with this driver, create a dev file with the command:\n");
	printk(KERN_INFO "mknod /dev/%s/ c %d 0\n", DEVICE_NAME, Major);
	printk(KERN_INFO "Different minor numbers can be used. You can use cat or echo to the dev file.\n");
	printk(KERN_INFO "Remove the device file when done\n");

	return SUCCESS;
}

void cleanup_module(void){
	if (Device_Open){
		printk(KERN_ALERT "Error - Couldn't remove yet - device is still in use!\n");
		return;
	}
	
	unregister_chrdev(Major, DEVICE_NAME); // tries to unregister the driver - if unsuccessful returns a negative number
	printk(KERN_INFO "chardev module has been successfully UNloaded\n");
}


// file operations methods

// device open - called when you run something like cat /dev/mychardriver

static int device_open(struct inode *inode, struct file* file){
	static int counter = 0;
	if (Device_Open){ // Flag used by driver to detect if the device is in use. Returns 1 if true.
		return -EBUSY;
	}
	Device_Open++;
	sprintf(msg, "I'VE SAID %d TIMES: Hello, world!\n", counter++);
	msg_Ptr = msg;
	// try_module_get(THIS_MODULE); // Used to increment usage count of module, but the unregister_chrdev() returns void so can't use

	return SUCCESS;
}

// device close - called when you close the driver

static int device_release(struct inode *inode, struct file *file){
	Device_Open--;

	// Must decrement usage count, else the module can't be removed once it's added

	module_put(THIS_MODULE);

	return 0;
}


// device read - called when a process tries to read from the (opened) dev file
static ssize_t device_read(struct file *filp, char *buffer, size_t length, loff_t * offset) {
	int bytes_read = 0; // bytes written to the buffer

	if (*msg_Ptr == 0){ // if at the end of the message
		return 0;
	}

	// put the data into the buffer
	
	while (length && *msg_Ptr){
		put_user(*(msg_Ptr++), buffer++); 
		// since msg_Ptr is stored in kernel space, we should copy it to userspace into the buffer
		length--;
		bytes_read++;
	}

	return bytes_read; // common for functions to return this
}

static ssize_t device_write(struct file *filp, const char *buff, size_t len, loff_t *off){
	printk(KERN_ALERT "Sorry, the write operation is not supported!\n");
	return -EINVAL; // Invalid argument (aka error code 22)
}
