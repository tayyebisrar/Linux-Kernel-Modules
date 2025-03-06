/*
 * This code can create a readable 'file' in the /proc directory
 * This directory is used for anything that needs to report stuff
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h> // needed to use /proc-related functions

#define procfsname "helloworld"

/* The following struct is used to hold information about the /proc file
 * This struct can configure data about the file, such as the owner
 */

struct proc_dir_entry *My_proc_file;

// Now, put data into it:

/* procfile_read arguments: 1 - Buffer where data can be inserted, 2 - ptr to string for own (non-auto-allocated) buffer,
 * 3 - current file position (offset), 4 - length of argument 1, 
 * 5 - end of file flag, 1 if true, negative if error,
 * 6 - pointer to data, needed if you have 'common reads' between /proc files
 */

int procfile_read(char *buffer, char **buffer_location, off_t offset, int buffer_length, int *eof, void *data){
	
	int ret;
	printk(KERN_INFO "procfile_read, (/proc/%s was called)\n", procfsname);




