#ifndef CommomLib_H
#define CommomLib_H 

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/moduleparam.h>
#include <linux/mutex.h>
#include <linux/irq.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <linux/poll.h>
#include <linux/cdev.h>

#define DEVICE_NAME "cryptomodule"
#define KEY_SIZE		16
#define BUFFER_SIZE 	80

DEFINE_MUTEX(mutex_buffer);
 
static int major;
static int device = 0;
static struct class *cls;

static char *key;
static char buffer[BUFFER_SIZE];

static struct file_operations crypto_fops = {
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release
};

static int __init init_crypto(void);
static void __exit cleanup_crypto(void);

static int device_open(struct inode *inode, struct file *file);
static int device_release(struct inode *inode, struct file *file);

module_param(key, charp, 0000);
MODULE_PARM_DESC(key, "Cryptographic Key");