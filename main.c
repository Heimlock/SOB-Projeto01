
#define  __MASTER
#include "CommonLib.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Felipe Ferreira");
MODULE_DESCRIPTION("Main File");
MODULE_SUPPORTED_DEVICE(DEVICE_NAME);

module_param(key, charp, 0000);
MODULE_PARM_DESC(key, "Key String - 32bytes lenght");

static int __init cryptomodule_init(void){
    int   i = 0;
    char  *keyBuffer;
    pr_info("Inicia o Modulo\n");
    pr_info("Chave passada: %s\n", key);

    validate(key, &keyBuffer, KEY_LENGHT);
    //pr_info("Depois de validado: %s\n", keyBuffer);

    deserialize( keyBuffer, keyHex, KEY_LENGHT );
    //pr_info("Depois de desserializado: %s\n", keyHex);

    pr_info("[%s] | keyHex......: %s\n", DEVICE_NAME, keyHex);
    //printHex( keyHex, KEY_LENGHT, "Key Received" );

    pr_info("[%s] | Buffer......: %s\n", DEVICE_NAME, keyBuffer);
    
    if(init_fops() != 0){
        pr_crit("[%s] | ERRO! <init_fops>\n", DEVICE_NAME);
    }
    return 0;
}

static void __exit cryptomodule_exit(void){
    cleanup_fops();

    pr_info("[%s] | Terminated\n", DEVICE_NAME);
}

module_init(cryptomodule_init);
module_exit(cryptomodule_exit);
