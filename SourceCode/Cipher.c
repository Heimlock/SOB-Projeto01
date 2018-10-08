
#include "CommonLib.h"

MODULE_AUTHOR("Felipe Ferreira");
MODULE_DESCRIPTION("Cipher Operations");
MODULE_LICENSE("GPL");

#define BLK_SIZE  04 * sizeof(int)

int   encrypt( u8 key[], char input[], char output[], size_t size )
{
    int     rtn   = 0;
    struct  crypto_blkcipher  *tfm  = NULL;
    struct  blkcipher_desc    desc;
    struct  scatterlist       input_sg;
    struct  scatterlist       output_sg;

    size_t  size_bytes  = (size * (BLK_SIZE/sizeof(int)));
    unsigned char *my_iv  = NULL;
    void *iv  = NULL;
    size_t ivsize = (size * (BLK_SIZE/sizeof(int)));


    #ifdef DEBUG
      pr_info("[%s] | ENCRYPT INFO\n", DEVICE_NAME);
      // pr_info("[%s] | Key..: %s\n", DEVICE_NAME, key);
      dump_hex( key, 32, "Key.." );
      pr_info("[%s] | Input: %s\n", DEVICE_NAME, input);
      pr_info("[%s] | Size.: %d\n", DEVICE_NAME, size);
    #endif


    my_iv = vmalloc(size_bytes);
    if (!my_iv)
    {
       printk("[%s] | ENCRYPT -- Failed to Alloc my_iv!\n", DEVICE_NAME);
       rtn  = -1;
       goto my_iv_free;
    }
    memset(my_iv, 0, size_bytes);

    tfm = crypto_alloc_blkcipher("ecb(aes)", 0, 0);
    if (!tfm)
    {
       printk("[%s] | ENCRYPT -- Failed to Alloc tfm!\n", DEVICE_NAME);
       rtn  = -1;
       goto tfm_free;
    }
    crypto_blkcipher_setkey( tfm, key, size_bytes);

    #ifdef  DEBUG
      pr_info("[ENCRYPT] | TFM Allocated\n");
    #endif

    iv = crypto_blkcipher_crt(tfm)->iv;
    ivsize = crypto_blkcipher_ivsize(tfm);
    memcpy(iv, my_iv, ivsize);

    sg_init_one( &input_sg,   input,  size_bytes );
    sg_init_one( &output_sg,  output, size_bytes );


    #ifdef  DEBUG
      pr_info("[ENCRYPT] | SG Init\n");
    #endif

    desc.tfm   = tfm;
    desc.flags = 0;

    rtn = crypto_blkcipher_encrypt( &desc, &output_sg, &input_sg, size );
    if( rtn < 0 )
    {
       printk("[%s] | Encrypt Failed, Code {%d}\n", DEVICE_NAME, rtn);
       printk("[%s] | Input: %s\n", DEVICE_NAME, input);
       goto crypto_free;
    }

    #ifdef  DEBUG
      pr_info("[ENCRYPT] | Encrypt\n");
    #endif

    crypto_free_blkcipher( tfm );

    #ifdef  DEBUG
      pr_info("[ENCRYPT] | TFM Free\n");
    #endif

    crypto_free:
      // vfree(tfm);
    tfm_free:
      vfree(my_iv);
    my_iv_free:
    return rtn;
}

int   decrypt( u8 key[], char input[], char output[], size_t size )
{
    int     rtn   = 0;
    struct  crypto_blkcipher  *tfm;
    struct  blkcipher_desc    desc;
    struct  scatterlist       input_sg;
    struct  scatterlist       output_sg;

    size_t  size_bytes  = (size * (BLK_SIZE/sizeof(int)));
    unsigned char *my_iv;
    void *iv;
    size_t ivsize = (size * (BLK_SIZE/sizeof(int)));

    my_iv = vmalloc(size_bytes);
    if (!my_iv)
    {
       printk("[%s] | DECRYPT -- Failed to Alloc my_iv!\n", DEVICE_NAME);
       rtn  = -1;
       goto my_iv_free;
    }
    memset(my_iv, 0, size_bytes);

    tfm = crypto_alloc_blkcipher("ecb(aes)", 0, 0);
    if (!tfm)
    {
       printk("[%s] | DECRYPT -- Failed to Alloc tfm!\n", DEVICE_NAME);
       rtn  = -1;
       goto tfm_free;
    }

    crypto_blkcipher_setkey( tfm, key, size_bytes);

    #ifdef  DEBUG
      pr_info("[DECRYPT] | TFM Allocated\n");
    #endif

    iv = crypto_blkcipher_crt(tfm)->iv;
    ivsize = crypto_blkcipher_ivsize(tfm);
    memcpy(iv, my_iv, ivsize);

    sg_init_one( &input_sg,   input,  size_bytes );
    sg_init_one( &output_sg,  output, size_bytes );

    #ifdef  DEBUG
      pr_info("[DECRYPT] | SG Init\n");
    #endif

    desc.tfm   = tfm;
    desc.flags = 0;

    rtn = crypto_blkcipher_decrypt( &desc, &output_sg, &input_sg, size );
    if( rtn < 0 )
    {
       printk("[%s] | Decrypt Failed, Code {%d}\n", DEVICE_NAME, rtn);
       goto crypto_free;
    }

    #ifdef  DEBUG
      pr_info("[DECRYPT] | Decrypt\n");
    #endif

    crypto_free_blkcipher( tfm );

    #ifdef  DEBUG
      pr_info("[DECRYPT] | TFM Free\n");
    #endif

    crypto_free:
      vfree(tfm);
    tfm_free:
      vfree(my_iv);
    my_iv_free:
    return rtn;
}


void dump_hex( char input[], int size, char *info )
{
    int   i;
    char  str[(80 * 2) + 1];

    for (i = 0; i < size; i++)
        sprintf(&str[i*2],"%02x", (unsigned char)input[i]);
    str[i*2] = 0;

    printk("[%s] | %s: '%s'\n", DEVICE_NAME, info, str);
}
