
/*
 *		Sistemas Operacionais B
 *		Projeto 01 - Módulo Criptográfico
 *
 *	Integrantes:
 *		Bruno Pereira Bannwart 				RA: 15171572
 *		Felipe Moreira Ferreira 		 	RA: 16116469
 *		Luiz Felipe Zerbetto Masson 	RA: 15166804
 *		Matheus Manganeli de Macedo 	RA: 16250276
 *		Rodrigo da Silva Cardoso 			RA: 16430126
 *
 *	 Operaçõe Relacionadas com a Interface estabelecida com UserSpace
 */

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

   #ifdef    DEBUG
    pr_info("[%s] | Registered correctly with major number %d\n", DEVICE_NAME , majorNumber);
   #endif

   cls = class_create(THIS_MODULE, DEVICE_NAME);
   if (IS_ERR(cls))
   {
      unregister_chrdev(majorNumber, DEVICE_NAME);
      pr_alert("[%s] | Failed to register device class\n", DEVICE_NAME);
      return PTR_ERR(cls);
   }

   #ifdef    DEBUG
    pr_info("[%s] | Device class registered correctly\n", DEVICE_NAME);
   #endif

   device = device_create(cls, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
   if (IS_ERR(device))
   {
      class_destroy(cls);
      unregister_chrdev(majorNumber, DEVICE_NAME);
      pr_alert("[%s] | Failed to create the device\n", DEVICE_NAME);
      return PTR_ERR(device);
   }

   #ifdef    DEBUG
    pr_info("[%s] | Device class created correctly\n", DEVICE_NAME);
   #endif
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
        if( length > (2*BUF_LEN) )
        {
            bytesRead   =   (2*BUF_LEN);
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
        bytesRead = -ENODATA;
    }

    mutex_unlock(&bufferLock);
    return bytesRead;
}

/*
 *      UserSpace Attemps to Write
 */
ssize_t device_write( struct file *filp, const char *buffer_usr, size_t length, loff_t *offset )
{
    int ret, i, operation = -1;
    unsigned long  bytes2Write = 0;
    char  tempBuffer[((2*BUF_LEN) + 2)]; // 2*BUF_LEN + Operation + Space
    char  *cipherText = NULL, *verifiedInput = NULL;

    mutex_lock(&bufferLock);

    //  Presets
    buffer_size    =   0;
    memset( tempBuffer, 0, ((2*BUF_LEN) + 2) );
    memset( bufferIN, 0, 2*BUF_LEN );

    //  Select a Valid Lenght
    if( length > (2*BUF_LEN) )
    {
        bytes2Write   =   (2*BUF_LEN);
    }
    else
    {
        bytes2Write   =   length;
    }

    //                    to          from        length
    ret = copy_from_user( tempBuffer, buffer_usr, bytes2Write );
    if( ret != 0 )
    {
        pr_warning("[%s] | %d bytes Wont be Written. Bytes Written = %lu\n", DEVICE_NAME, ret, bytes2Write);
    }
    pr_info( "[%s] | Data Received.....: '%s'\n", DEVICE_NAME, tempBuffer );

    //  Verify Received Command
    //  First Char has to be 'c' or 'd' or 'h'
    if( tempBuffer[0] != 'c' && tempBuffer[0] != 'd' && tempBuffer[0] != 'h' )
    {
        pr_alert( "[%s] | Invalid Argument. (buffer[%d]=='%c')\n", DEVICE_NAME, 0, tempBuffer[0] );
        buffer_size =   0;
        bytes2Write =   -EPERM;
        goto  validate_error;
    }

    //  Select Operation
    if( tempBuffer[0] == 'c' )        operation = ENCRYPT;
    else if ( tempBuffer[0] == 'd' )  operation = DECRYPT;
    else if ( tempBuffer[0] == 'h' )  operation = SUMHASH;

    //  Go Through the TempBuffer copy it to the Actual Buffer
    //  [0] == Operation & [1] == ' '
    for( i = 0; i < (bytes2Write - 2); i++ )
    {
        bufferIN[i]  = tempBuffer[i+2];
    }
    buffer_size    =   bytes2Write - 2;

    //  Verify if Input is Valid
    if( validate( bufferIN, &verifiedInput, buffer_size ) != 0 )
    {
        pr_alert( "[%s] | ERROR! Validate Function\n", DEVICE_NAME);
        buffer_size =   0;
        bytes2Write =   -EINVAL;
        goto  validate_error;
    }

    //  Deserialize Input
    memset( buffer,     0,   BUF_LEN );
    memset( tempBuffer, 0, 2*BUF_LEN );
    memset( bufferOUT,  0,   BUF_LEN );
    deserialize( verifiedInput, buffer, buffer_size );
    buffer_size = (buffer_size/2);
    pr_info( "[%s] | Buffer Usage......: %d / %d\n", DEVICE_NAME, buffer_size, BUF_LEN );
    // pr_info( "[%s] | Input Buffer......: %s\n", DEVICE_NAME, bufferIN );
    printHex(  buffer, buffer_size, "DeserializedBuffer" );
    switch (operation)
    {
      case ENCRYPT:
      {
          if( (buffer_size  = arrangeText( buffer, &cipherText, buffer_size )) == -1 )
          {
              pr_err( "[%s] | ERROR! arrangeText Function\n", DEVICE_NAME);
              buffer_size =   0;
              break;
          }
          if( encrypt( keyHex, cipherText, tempBuffer, buffer_size ) < 0 )
          {
                pr_err( "[%s] | ERROR! encrypt Function\n", DEVICE_NAME);
                buffer_size =   0;
          }

          printHex(  tempBuffer, buffer_size, "TempBuffer Result." );
          serialize( tempBuffer, bufferOUT, buffer_size );
          buffer_size = (2 * buffer_size);
          break;
      }
      case DECRYPT:
      {
          if( (buffer_size  = arrangeText( buffer, &cipherText, buffer_size )) == -1 )
          {
              pr_err( "[%s] | ERROR! arrangeText Function\n", DEVICE_NAME);
              buffer_size =   0;
              break;
          }
          if( decrypt( keyHex, cipherText, tempBuffer, buffer_size ) < 0 )
          {
                pr_err( "[%s] | ERROR! encrypt Function\n", DEVICE_NAME);
                buffer_size =   0;
          }

          printHex(  tempBuffer, buffer_size, "TempBuffer Result." );
          serialize( tempBuffer, bufferOUT, buffer_size );
          buffer_size = (2 * buffer_size);
          break;
      }
      case SUMHASH:
      {
          if(arrangeHash( buffer, &cipherText, buffer_size ) == -1)
          {
              pr_err( "[%s] | ERROR! sumHash Function\n", DEVICE_NAME);
              buffer_size =   0;
              break;
          }
          if( sumHash( cipherText, bufferOUT ) == -1 )
          {
                pr_err( "[%s] | ERROR! sumHash Function\n", DEVICE_NAME);
                buffer_size =   0;
          }
          buffer_size = SHA256_LENGTH;
          break;
      }
    }
    pr_info( "[%s] | BufferOUT Size....: %d\n", DEVICE_NAME, buffer_size );

    printHex( keyHex,    KEY_LENGHT,  "KeyHex............" );
    pr_info( "[%s] | Input Buffer......: %s\n", DEVICE_NAME, bufferIN );
    printHex( buffer,    buffer_size, "Buffer Stored....." );
    printHex( bufferOUT, buffer_size, "BufferOUT Stored.." );
    pr_info( "[%s] | Bytes Available...: %d / %d\n", DEVICE_NAME, (BUF_LEN - buffer_size), BUF_LEN );
    pr_info( "[%s] | Bytes Received....: %d\n\n", DEVICE_NAME, (int)length );

    vfree(cipherText);
    vfree(verifiedInput);
    validate_error:
    mutex_unlock(&bufferLock);
    return bytes2Write;
}
