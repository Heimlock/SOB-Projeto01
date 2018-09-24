#include "CommomLib.h"

static int __init init_crypto(void)
{
	int i;
	
	for(i = 0; i < KEY_SIZE; i++)
	{
		if(key[i] == '\0')
			key[i] = '0';
		
		if(i == (KEY_SIZE - 1))
			key[i] = '/0';
	}
	
	printk("Initialization\n");
	printk("Key received: %s.", key);
	
	major =  register_chrdev(0, DEVICE_NAME, &crypto_fops);
	
	if(major < 0)
	{
		printk("Failed to register device with major %d.\n", major);
		return major;
	}
	
	printk("Major: %d.\n", major);
	cls = class_create(THIS_MODULE, DEVICE_NAME);
	device_create(cls, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);
	printk("Device created: %s.\n", DEVICE_NAME);

	return 0;
}

module_init(init_crypto);