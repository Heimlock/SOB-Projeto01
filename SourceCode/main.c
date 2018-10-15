
#define  __MASTER
#include "CommonLib.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Felipe Ferreira");
MODULE_DESCRIPTION("Main File");
MODULE_SUPPORTED_DEVICE(DEVICE_NAME);

module_param(key, charp, 0000);
MODULE_PARM_DESC(key, "Key String - 32bytes lenght");

static int __init cryptomodule_init(void)
{
    int   i;
    char  *keyBuffer =  NULL;
    char  *failSafe  = "000102030405060708090A0B0C0D0E0F";
    pr_info("[%s] | Initializated\n", DEVICE_NAME);

    //  Verify if Key is Valid
    // validate( key, &keyBuffer, (2 * KEY_LENGHT) );
    if( validate( key, &keyBuffer, (2 * KEY_LENGHT) ) != 0 )
    {
          pr_err( "[%s] | ERROR! validate Function\n", DEVICE_NAME);
          keyBuffer = failSafe;
    }

    deserialize( keyBuffer, keyHex, (2 * KEY_LENGHT) );
    printHex( keyHex, KEY_LENGHT, "Key Received" );
    // pr_info("[%s] | KeyBuffer...: %s\n", DEVICE_NAME, keyBuffer);

    if( init_fops() != 0 )
    {
        pr_crit("[%s] | ERRO! <init_fops>\n", DEVICE_NAME);
    }
    return 0;
}

static void __exit cryptomodule_exit(void)
{
    cleanup_fops();

    pr_info("[%s] | Terminated\n", DEVICE_NAME);
}

module_init(cryptomodule_init);
module_exit(cryptomodule_exit);
