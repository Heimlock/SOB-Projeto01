
#include "CommonLib.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Felipe Ferreira");
MODULE_DESCRIPTION("File Operations");
MODULE_SUPPORTED_DEVICE(DEVICE_NAME);

int init_fops(void)
{
   majorNumber = register_chrdev(0, DEVICE_NAME, &crypto_fops);
   if (majorNumber < 0)
   {
      pr_alert("[%s] | Failed to register a major number\n", DEVICE_NAME);
      return majorNumber;
   }

   pr_info("[%s] | Registered correctly with major number %d\n", DEVICE_NAME , majorNumber);

   cls = class_create(THIS_MODULE, DEVICE_NAME);
   if (IS_ERR(cls))
   {
      unregister_chrdev(majorNumber, DEVICE_NAME);
      pr_alert("[%s] | Failed to register device class\n", DEVICE_NAME);
      return PTR_ERR(cls);
   }

   pr_info("[%s] | Device class registered correctly\n", DEVICE_NAME);

   device = device_create(cls, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
   if (IS_ERR(device))
   {
      class_destroy(cls);
      unregister_chrdev(majorNumber, DEVICE_NAME);
      pr_alert("[%s] | Failed to create the device\n", DEVICE_NAME);
      return PTR_ERR(device);
   }

   pr_info("[%s] | Device class created correctly\n", DEVICE_NAME);
   return 0;
}

void cleanup_fops(void)
{
    device_destroy(cls, MKDEV(majorNumber, 0));
    class_unregister(cls);
    class_destroy(cls);
    unregister_chrdev(majorNumber, DEVICE_NAME);
}

/*
 *      UserSpace Attemps to Open the Device File
 */
int device_open(struct inode *inode, struct file *file)
{
    if (Device_Open)
        return -EBUSY;

    Device_Open++;

    buffer_Ptr = buffer;
    try_module_get(THIS_MODULE);

    return 0;
}

/*
 *      UserSpace Closes the Device File
 */
int device_release(struct inode *inode, struct file *file)
{
    Device_Open--;          /* We're now ready for our next caller */

    /*
     * Decrement the usage count, or else once you opened the file, you'll
     * never get get rid of the module.
     */
    module_put(THIS_MODULE);

    return 0;
}

/*
 *      UserSpace Attemps to Read
 */
ssize_t device_read( struct file *filp, char *buffer_usr, size_t length, loff_t *offset )
{
    unsigned long  bytesRead = 0;
    int ret;

    mutex_lock(&bufferLock);

    //  Verify if has anything on the Buffer
    if( buffer_size != 0 )
    {
        if( length > BUF_LEN )
        {
            bytesRead   =   BUF_LEN;
        }
        else
        {
            bytesRead   =   length;
        }
        //                  to          from        length
        ret = copy_to_user( buffer_usr, bufferOUT, bytesRead );
        if( ret != 0 )
        {
            pr_warning("[%s] | %d bytes Wont be Read. Bytes Read = %lu\n", DEVICE_NAME, ret, (bytesRead - ret));
        }
    }
    else
    {
        pr_warning("[%s] | No Data\n", DEVICE_NAME);
    }

    mutex_unlock(&bufferLock);
    return bytesRead;
}

/*
 *      UserSpace Attemps to Write
 */
ssize_t device_write( struct file *filp, const char *buffer_usr, size_t length, loff_t *offset )
{
    int ret, i;
    int operation = -1;
    unsigned long  bytes2Write = 0;
    char tempBuffer[BUF_LEN];

    mutex_lock(&bufferLock);

    buffer_size    =   0;

    if( length > BUF_LEN )
    {
        bytes2Write   =   BUF_LEN;
    }
    else
    {
        bytes2Write   =   length;
    }
    ret = copy_from_user( tempBuffer, buffer_usr, bytes2Write );
    if( ret != 0 )
    {
        pr_warning("[%s] | %d bytes Wont be Written. Bytes Written = %lu\n", DEVICE_NAME, ret, bytes2Write);
    }


    pr_info( "[%s] | Data Received: '%s'\n", DEVICE_NAME, tempBuffer );

    //  First Char has to be 'c' or 'd' or 'h'
    if( tempBuffer[0] != 'c' && tempBuffer[0] != 'd' && tempBuffer[0] != 'h' )
    {
        pr_alert( "[%s] | Invalid Argument. (buffer[%d]=='%c')\n", DEVICE_NAME, 0, tempBuffer[0] );
        return -EINVAL;
    }

    //  ENCRYPT
    if( tempBuffer[0] == 'c' )        operation = ENCRYPT;
    else if ( tempBuffer[0] == 'd' )  operation = DECRYPT;
    else if ( tempBuffer[0] == 'h' )  operation = SUMHASH;

    //  Go Through the TempBuffer copy it to the Actual Buffer
    //  [0] == Operation & [1] == ' '
    for( i = 0; i < (bytes2Write - 2); i++ )
    {
      buffer[i]  = tempBuffer[i+2];
    }
    buffer_size    =   bytes2Write - 2;
    buffer[buffer_size+1] = '\0';

    // printk("[%s] | Buffer: %s\n[%s] | TempBuffer: %s\n", DEVICE_NAME, buffer, DEVICE_NAME, tempBuffer);

    switch (operation)
    {
      case ENCRYPT:
      {
          pr_err( "[%s] | Not Implemented yet\n", DEVICE_NAME);
        break;
      }
      case DECRYPT:
      {
          pr_err( "[%s] | Not Implemented yet\n", DEVICE_NAME);
        break;
      }
      case SUMHASH:
      {
        if( sumHash( buffer_Ptr, bufferOUT ) == -1 )
        {
              pr_err( "[%s] | ERROR! sumHash Function\n", DEVICE_NAME);
        }
        buffer_size = SHA256_LENGTH;
        // show_hash_result( buffer_Ptr );
        break;
      }
    }

    pr_info( "[%s] | Buffer Stored: '%s'\n", DEVICE_NAME, buffer_Ptr );
    pr_info( "[%s] | Bytes Available: '%d'\n", DEVICE_NAME, BUF_LEN - buffer_size );

    mutex_unlock(&bufferLock);
    return bytes2Write;
}
