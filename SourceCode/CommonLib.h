
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-function"


#ifndef _COMMON_H
#define _COMMON_H

#define DEVICE_NAME     "cryptomodule"
#define KEY_LENGHT      16
#define BUF_LEN         80

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/irq.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <linux/poll.h>
#include <linux/cdev.h>
#include <linux/mutex.h>

/*
 * Global Variables
 */

static int  Device_Open = 0;        //  Will Become a Mutex

static char *key = "0000000000000000";

/*
 *  Functions
 */
static int  __init cryptomodule_init( void );
static void __exit cryptomodule_exit( void );

#endif