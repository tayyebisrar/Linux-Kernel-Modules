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

	if (offset > 0) { // if we've read
		ret = 0;
	} else {
		ret = sprintf(buffer, "HelloWorld!\n"); // put the data in
	}
	return ret;
}

int init_module() {
	// Create a /proc file with the name procfsname, permissions of 0644 (owner RW, others R), NULL parent directory (it begins with /)
	My_proc_file = create_proc_entry(procfsname, 0644, NULL);
	
	// If couldn't create the file
	if (My_proc_file == NULL) {
		remove_proc_entry(procfs_name, &proc_root); // remove any remnants of the file if they were made
		printk(KERN_ALERT "Error - Couldn't initialise /proc/%s\n", procfsname);
		return -ENOMEM; // -ENOMEM is memory alloc failed
	}

	My_proc_file->read_proc = procfile_read; // assign procfile_read() as the /proc file's read function (read_proc)
	My_proc_file->owner = THIS_MODULE;
	My_proc_file->mode = S_IFREG | S_IRUGO; // sets mode - S_IFREG is a regular file, S_IRUGO means Readable by all users
	My_proc_file->uid = 0;
	My_proc_file->gid = 0; // uid and gid (User and Group ID) set to 0 for proc entry, so it's root user and root group
	My_proc_file->size=37; // The string 'Helloworld!\n' has size 37
	
	printk(KERN_INFO "/proc/%s created\n", procfsname);
	return 0; // it worked!
}

void cleanup_module() {
	remove_proc_entry(procfsname, &procroot);
	printk(KERN_INFO "/proc/%s removed\n", procfsname);
}

