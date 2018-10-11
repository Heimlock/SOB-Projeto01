
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
static int  Device_Open = 0;        //  Will Become a Mutex

// static char key[KEY_LENGHT] = "0000000000000000";
// static char *key = "0000000000000000";
// static unsigned char *key = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
// static unsigned char keyInput[(2 * KEY_LENGHT)];
// static char *key = "0000000000000000000000000000000000";
static char *key;
static unsigned char keyHex[KEY_LENGHT];
static int  majorNumber;
static char buffer[BUF_LEN];
static char bufferOUT[BUF_LEN];
static int  buffer_size = 0;
static char *buffer_Ptr;


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

int     sumHash( char *, char * );
void    show_hash_result( char * );

int     encrypt( u8 key[], char input[], char output[], size_t size );
int     decrypt( u8 key[], char input[], char output[], size_t size );

void    serialize   ( char input[], char output[], int sizeIN );
void    deserialize ( char input[], char output[], int sizeIN );
int     arrangeText ( char input[], char **output, int size );
void    printHex( char input[], int size, char *info );
int     validate( char *source, char **destiny, int size);

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
