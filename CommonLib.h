
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-function"


#ifndef _COMMON_H
#define _COMMON_H

#define DEVICE_NAME     "cryptomodule"
#define KEY_LENGHT      16
#define BUF_LEN         80
#define SIZE_BLOCK      16

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

static DEFINE_MUTEX( bufferLock );
static int  Device_Open = 0;        //  Will Become a Mutex

static char *key = "00000000000000000";
static int  majorNumber;
static char msg[BUF_LEN];
static int  msg_size = 0;
static char *msg_Ptr;

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

int    encrypt( char *, char *);
int    decrypt( char *, char *);
int    sumHash( char *, char *);

/*
*   Aux. Functions
*/

int    validateKey( char * );
int    validateInput( char *);
int    converterStr2Hex( char *, int * );

void    init_struct();
static void skcipher_cb(struct crypto_async_request *, int );
static unsigned int skcipher_encrypt(struct skcipher_def *);


/*
 *  Structs
 */

static struct class     *cls;
static struct device    *device = NULL;

static struct skcipher_def  sk;

static struct file_operations crypto_fops = {
    .read    =  device_read,
    .write   =  device_write,
    .open    =  device_open,
    .release =  device_release
};

static struct skcipher_def 
{
    struct scatterlist sg;
    struct crypto_skcipher *tfm;
    struct skcipher_request *req;
    struct tcrypt_result result;
};

static static struct tcrypt_result 
{
    struct completion completion;
    int err;
};

#endif
