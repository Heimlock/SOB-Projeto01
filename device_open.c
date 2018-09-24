#include "CommomLib.h"

static int device_open(struct inode *inode, struct file *file)
{
	int i;
	
	if(device)
		return -EBUSY;
	
	device++;
	mutex_init(mutex_buffer);
	printk("Mutex Initialized.\n");
		
	for(i = 0; i < BUFFER_SIZE; i++)
	{
		buffer[i] = '0';
		printk("Buffer[%d]: %d.\n",i, buffer[i]);
	}
	
	try_module_get(THIS_MODULE);
	return 0;
}
