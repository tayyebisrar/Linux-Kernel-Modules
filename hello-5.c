/*
 * Using command-line arguments with modules
 *
 */

#include <linux/module.h> // Required by all modules
#include <linux/kernel.h> // Needed for KERN_INFO
#include <linux/init.h> // Adds macros like __init __exit and __initdata
#include <linux/moduleparam.h> // Lets me use the module_param(), module_param_string() and module_param_array() macros
#include <linux/stat.h> // Lets me use sysfs permission flags
			//
			// S_IRUSR - Read permission for user
			// S_IWUSR - Write permission for user
			// S_IRGRP - Read permission for group
			// S_IWGRP - Write permission for group
			// S_IROTH - Read permission for others

#define MOD_DESC "Module to learn how to use command-line arguments for modules. Usage insmod ./hello-5.c my{datatype}=value"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TAYYEB ISRAR");
MODULE_DESCRIPTION(MOD_DESC);

static int hellonumber __initdata = 5;

static short int myshort = 1;
static int myint = 2;
static long int mylong = 49302391;
static char *mystring = "i love pizza";
static int myArray[2] = {1, 2};
static int arrayelements = 0;

module_param(myshort, short, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP); // user and group can read/write myshort
MODULE_PARM_DESC(myshort, "A short integer");
module_param(myint, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH); // user can read or write while group and others can only read myint
MODULE_PARM_DESC(myint, "An integer");
module_param(mylong, long, S_IRUSR); // user can only read mylong
MODULE_PARM_DESC(mylong, "A long");
module_param(mystring, charp, 0000); // nobody can read or write mystring after initialising
MODULE_PARM_DESC(mystring, "A string");

module_param_array(myArray, int, &arrayelements, 0000); // using the pointer &arrayelements lets you record the number of elements entered to myArray
MODULE_PARM_DESC(myArray, "An array of integers");

static int __init init_hello_5(void){
	printk(KERN_INFO "Hello, world %d\n", hellonumber);
	printk(KERN_INFO "Received %d values in myArray\n", arrayelements);
	for (int i = 0; i < (sizeof myArray) / sizeof(int); i++){
		printk(KERN_INFO "myArray[%d] = %d\n", i, myArray[i]);
	}
	printk(KERN_INFO "myint: %d\n", myint);
	printk(KERN_INFO "myshort: %d\n", myshort);
	printk(KERN_INFO "mystring: %s\n", mystring);
	return 0;
}

static void __exit exit_hello_5(void){
	printk(KERN_INFO "Goodbye, world\n");
}

module_init(init_hello_5);
module_exit(exit_hello_5);

