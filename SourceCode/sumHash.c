
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
 *	 Operações Relacionadas com o Resumo Criptográfico
 */

#include "CommonLib.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Felipe Ferreira");
MODULE_DESCRIPTION("sumHash Function");
MODULE_SUPPORTED_DEVICE(DEVICE_NAME);

int     sumHash( char plaintext[], char hash_sha256[] )
{
    struct  crypto_shash    *sha256;
    struct  shash_desc      *shash;

    sha256 = crypto_alloc_shash("sha256", 0, 0);
    if (IS_ERR(sha256))
        return -1;

    shash = kmalloc(sizeof(struct shash_desc) + crypto_shash_descsize(sha256), GFP_KERNEL);

    if (!shash)
        return -ENOMEM;

    shash->tfm = sha256;
    shash->flags = 0;

    if (crypto_shash_init(shash))
        return -1;

    if (crypto_shash_update(shash, plaintext, strlen(plaintext)))
        return -1;

    if (crypto_shash_final(shash, hash_sha256))
    return -1;

    kfree(shash);
    crypto_free_shash(sha256);
    return 0;
}


int    arrangeHash( char input[], char **output, int size )
{
    int   i = 0;
    #ifdef  DEBUG
      char  str[SHA256_LENGTH + 1];
      memset( str, 0, (SHA256_LENGTH + 1) );
    #endif

    if( (*output) != NULL )
        vfree((*output));

    (*output)   =   vmalloc( SHA256_LENGTH );
    if( !(*output) )
    {
        pr_crit("[%s] | ArrangeHash -- Failed to Alloc output!\n", DEVICE_NAME);
        return -1;
    }
    memset( *output, 0, SHA256_LENGTH );

    for( i = 0; i < SHA256_LENGTH; i++ )
    {
        (*output)[i] = input[i];

        #ifdef  DEBUG
          str[i] = input[i];
        #endif
    }

    #ifdef  DEBUG
      pr_info( "[%s] | ArrangeHash.....: %s\n", DEVICE_NAME, str );
    #endif
    return 0;
}
