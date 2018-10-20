
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
 *	 Main
 */

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

    printk("============================================================\n");
    pr_info("[%s] | Initializated\n", DEVICE_NAME);

    //  Verify if Key is Valid
    if( validate( key, &keyBuffer, (2 * KEY_LENGHT) ) != 0 )
    {
        pr_err( "[%s] | ERROR! validate Function\n", DEVICE_NAME);
        memset( keyHex, 0, KEY_LENGHT );
    }
    else
    {
        deserialize( keyBuffer, keyHex, (2 * KEY_LENGHT) );
        vfree( keyBuffer );
    }
    printHex( keyHex, KEY_LENGHT, "Key Received......" );

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
    printk("============================================================\n");
}

module_init(cryptomodule_init);
module_exit(cryptomodule_exit);
