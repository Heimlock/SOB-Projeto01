
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-function"


#ifndef _COMMON_H
#define _COMMON_H

#define DEBUG

#define DEVICE_NAME     "cryptomodule"
#define KEY_LENGHT      16
#define BUF_LEN         80
#define SHA256_LENGTH   32

#include <linux/cdev.h>
#include <linux/crypto.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <linux/irq.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/poll.h>
#include <linux/scatterlist.h>
#include <asm/uaccess.h>
#include <linux/types.h>
#include <linux/vmalloc.h>

/*
 * Global Variables
 */
 static DEFINE_MUTEX( bufferLock );
#ifdef    __MASTER
  int  Device_Open = 0;
  int  majorNumber;

  char *key;
  unsigned char keyHex[KEY_LENGHT];

  char buffer[BUF_LEN];
  char bufferOUT[BUF_LEN];
  int  buffer_size = 0;
  char *buffer_Ptr;

  EXPORT_SYMBOL(Device_Open);
  EXPORT_SYMBOL(majorNumber);
  EXPORT_SYMBOL(key);
  EXPORT_SYMBOL(keyHex);
  EXPORT_SYMBOL(buffer);
  EXPORT_SYMBOL(bufferOUT);
  EXPORT_SYMBOL(buffer_size);
  EXPORT_SYMBOL(buffer_Ptr);
#endif

#ifndef   __MASTER
  extern int  Device_Open;
  extern int  majorNumber;

  extern char *key;
  extern unsigned char keyHex[KEY_LENGHT];
  extern char buffer[BUF_LEN];
  extern char bufferOUT[BUF_LEN];
  extern int  buffer_size;
  extern char *buffer_Ptr;
#endif

/*
 *  Functions
 */
static int  __init cryptomodule_init( void );
static void __exit cryptomodule_exit( void );

int     init_fops( void );
void    cleanup_fops( void );
int     device_open( struct inode *, struct file * );
int     device_release( struct inode *, struct file * );
ssize_t device_read( struct file *, char *, size_t , loff_t * );
ssize_t device_write( struct file *, const char *, size_t , loff_t * );

int     encrypt( u8 *, char *, char *, size_t );
int     decrypt( u8 *, char *, char *, size_t );

void    serialize   ( char *, char  *,  int    );
void    deserialize ( char *, char  *,  int    );
int     arrangeText ( char *, char **,  int    );
void    printHex    ( char *, int    ,  char * );
int     validate    ( char *, char **,  int    );

/*
 *  Structs
 */

static struct class     *cls;
static struct device    *device = NULL;

static struct file_operations crypto_fops = {
    .read    =  device_read,
    .write   =  device_write,
    .open    =  device_open,
    .release =  device_release
};

/*
 *  Enums
 */

typedef	enum STATES	{
 	ENCRYPT, DECRYPT, SUMHASH
 } STATES;

#endif
