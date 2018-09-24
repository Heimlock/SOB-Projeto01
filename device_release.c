#include "CommomLib.h"

static int device_release(struct inode *inode, struct file *file)
{
	device--;
	mutex_destroy(mutex_buffer);
	module_put(THIS_MODULE);
	printk("Device Released.\n");
	
	return 0;
}
