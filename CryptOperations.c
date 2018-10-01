#include "CommonLib.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Bruno Pereira Bannwart");
MODULE_DESCRIPTION("Crypt Operations");
MODULE_SUPPORTED_DEVICE(DEVICE_NAME);

int validateKey( char *key )
{
    int i;

    pr_info( "[%s] | Key Received: '%s' \n", DEVICE_NAME, key);

    //  validate key size
    for(i = 0; i < (KEY_LENGHT + 1); i++)
	{
		if(key[i] == '\0')
        {
			key[i] = '0';
        }

		if(i == KEY_LENGHT)
        {
            key[i] = '/0';
        }
	}

    //  validate key characters - ASCII Table Values
    for(i = 0; i < KEY_LENGHT; i++)
    {
        if(key[i] > 9)
        {
            if(key[i] > 96 && key[i] < 103)
            {
                key[i] = key[i] - 32;
            }
            else
            {
                if(key[i] < 65 || key[i] > 70)
                {
                    pr_crit(" [%s] | Key Invalid\n", DEVICE_NAME);
                    return -1;
                    
                }
                //  else the character is already a capital letter
            }
        }
        //  else the character is already digit
    }

    pr_info( "[%s] | Key Validated: '%s' \n", DEVICE_NAME, key);
    return 1;
}

int validateInput( char *input, int inputSize)
{
    int i, n;

    pr_info( "[%s] | Input Received: '%s'\n", DEVICE_NAME, input);
    n = (inputSize % SIZE_BLOCK);

    //  It's not a multiple of SIZE_BLOCKs
    if(n != 0)
    {
        for( i = (inputSize - n); i < inputSize; i++)
        {
            input[i] = '0';
        }

        pr_info( "[%s] | Size %i not multiple. Input Completed: '%s'\n", DEVICE_NAME, inputSize, input);
    }

    //  validate input characters - ASCII Table Values
    for(i = 0; i < SIZE_BLOCK; i++)
    {
        if(input[i] > 9)
        {
            if(input[i] > 96 && input[i] < 103)
            {
                input[i] = input[i] - 32;
            }
            else
            {
                if(input[i] < 65 || input[i] > 70)
                {
                    pr_crit(" [%s] | Input Invalid\n", DEVICE_NAME);
                    return -1;
                }
                //  else already is a capital letter
            }
        }
        //  else the character is already digit
    }

    pr_info( "[%s] | Input Validated: '%s'\n", DEVICE_NAME, input);
    return 1;
}

int converterStr2Hex( char *input, int *array)
{
    int i, j, lenght;

    lenght = 0;
    
    while(input[i] != '\0')
    {
        lenght++;
    }

    j = 0;

    for(i = 0; i < lenght; i = i + 2)
    {
        //sprintf();
        j++; 
    }
}

int encrypt( char *input, char *output )
{
    int ret = -EFAULT;
    init_struct();

    sk->tfm = crypto_alloc_skcipher("ECB-AES", 0, 0);
    
    if(IS_ERR(sk->tfm))
    {
        pr_info( " [%s] | Could not allocate skcipher handle\n", DEVICE_NAME);
        return PTR_ERR(sk->tfm);
    }

    sk->req = skcipher_request_alloc(sk->tfm, GFP_KERNEL);

    if (!sk->req) 
    {
        pr_info(" [%s] | Could not allocate skcipher request\n", DEVICE_NAME);
        ret = -ENOMEM;
        goto out;
    }

    skcipher_request_set_callback(sk->req, CRYPTO_TFM_REQ_MAY_BACKLOG, skcipher_cb, &sk.result);

    /* IV will be random 
        ivdata = kmalloc(SIZE_BLOCK, GFP_KERNEL);
    if (!ivdata) {
        pr_info("could not allocate ivdata\n");
        goto out;
    }
    get_random_bytes(ivdata, SIZE_BLOCK);
    */

    sg_init_one(&sk.sg, input, SIZE_BLOCK);
    skcipher_request_set_crypt(sk->req, &sk.sg, &sk.sg, SIZE_BLOCK, /*ivdata*/);
    init_completion(&sk.result.completion);

    ret = skcipher_encrypt(&sk);

    output = sk->result.completion;
    
    pr_info(" [%s] | Encryption triggered successfully\n", DEVICE_NAME);

    if (sk->tfm)
        crypto_free_skcipher(sk->tfm);
    
    if (sk->req)
        skcipher_request_free(sk->req);
    
    if (ivdata)
        kfree(ivdata);
    
    return ret;
}

void init_struct()
{
    sk.sg       = NULL;
    sk->tfm     = NULL;
    sk->req     = NULL;
    sk->result  = NULL;
}

static void skcipher_cb(struct crypto_async_request *req, int error)
{
    struct tcrypt_result *result = req->data;

    if(error == -EINPROGRESS)
                return;
    
    result->err = error;
    complete(&result->completion);
    pr_info(" [%s] | Encryption finished successfully\n", DEVICE_NAME);
}

/* Perform cipher operation */
static unsigned int skcipher_encrypt(struct skcipher_def *sk)
{
    int rc = 0;

    rc = crypto_skcipher_encrypt(sk->req);

    switch (rc) 
    {
        case 0:             break;
        
        case -EINPROGRESS:

        case -EBUSY:        rc = wait_for_completion_interruptible(&sk->result.completion);
                            if (!rc && !sk->result.err) 
                            {
                                reinit_completion(&sk->result.completion);
                                break;
                            }

        default:            pr_info( "[%s] | skcipher encrypt returned with %d result %d\n", DEVICE_NAME, rc, sk->result.err);
                            break;
    }
    
    init_completion(&sk->result.completion);

    return rc;
}

void decrypt( char *input, char *output)
{

}

void sumHash()
{
    
}