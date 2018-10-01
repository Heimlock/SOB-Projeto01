
#include "CommonLib.h"

#include <crypto/internal/hash.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Felipe Ferreira");
MODULE_DESCRIPTION("sumHash Function");
MODULE_SUPPORTED_DEVICE(DEVICE_NAME);

int sumHash( char plaintext[], char hash_sha256[] )
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

// void show_hash_result(char * plaintext, char * hash_sha256)
void show_hash_result(char * hash_sha256)
{
    int i;
    char str[SHA256_LENGTH*2 + 1];

    // pr_info("sha256 test for string: \"%s\"\n", plaintext);
    for (i = 0; i < SHA256_LENGTH ; i++)
        sprintf(&str[i*2],"%02x", (unsigned char)hash_sha256[i]);
    str[i*2] = 0;
    pr_info("%s\n", str);
}
