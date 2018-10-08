
#include "CommonLib.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Felipe Ferreira");
MODULE_DESCRIPTION("Main File");
MODULE_SUPPORTED_DEVICE(DEVICE_NAME);

module_param(key, charp, 0000);
MODULE_PARM_DESC(key, "Key String - 16bits lenght");

static int __init cryptomodule_init(void)
{
    int i;
    pr_info("[%s] | Initializated\n", DEVICE_NAME);

    for( i = 0; i < (KEY_LENGHT+1); i++ )
    {
        if( key[i] == '\0' )
            key[i] = '0';

        //  Verify if it has reached the end
        if( i == (KEY_LENGHT) )
        {
            key[i] = '\0';
            break;
        }
    }

    pr_info("[%s] | Key is a string: %s\n", DEVICE_NAME, key);

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
