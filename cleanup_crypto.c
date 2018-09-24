#include "CommomLib.h"

static void __exit cleanup_crypto(void)
{
	device_destroy(cls, MKDEV(major, 0));
	class_destroy(cls);
	unregister_chrdev(major, DEVICE_NAME);
	printk("Device removed.\n");
}

module_exit(cleanup_crypto);
