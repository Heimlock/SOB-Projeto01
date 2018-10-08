/*
 *  hello-5.c - Demonstrates command line argument passing to a module.
*/
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/stat.h>
#include <linux/types.h>
#include <linux/crypto.h>
#include <linux/scatterlist.h>
#include <linux/gfp.h>
#include <linux/err.h>
#include <linux/timex.h>

#define MAX_BLK_SIZE (64*1024*1024)
#define MIN_BLK_SIZE (16)

#define TEST_TIMES 100

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Luiz");
static char *key = "0000000000000000";
static char *phrase = "0000000000000000";
/*
 * module_param(foo, int, 0000)
 * The first param is the parameters name
 * The second param is it's data type
 * The final argument is the permissions bits,
 * for exposing parameters in sysfs (if non-zero) at a later stage.
*/
module_param(key, charp, 0000);
MODULE_PARM_DESC(key, "Is the key for crypto");

module_param(phrase, charp, 0000);
MODULE_PARM_DESC(phrase, "Is the text to be incrypt");
/*
 * module_param_array(name, type, num, perm);
 * The first param is the parameter's (in this case the array's) name
 * The second param is the data type of the elements of the array
 * The third argument is a pointer to the variable that will store the number
 * of elements of the array initialized by the user at module loading time
 * The fourth argument is the permission bits
*/
//static void validarKey(char  *key);

static int __init hello_5_init(void){
  struct crypto_blkcipher *tfm;
  struct blkcipher_desc desc;
  u32 bs;
  int i,j;
  u32 npages;

  struct scatterlist *src;
  struct scatterlist *dst;
  char *buf;
  char **ins, **outs;

  unsigned int ret;

  int count = 0, count2 = 0;
  char myKey[17], myPhrase[17];

  npages = MAX_BLK_SIZE/PAGE_SIZE;
  //--------------------KEY checking---------------------------//
  while (key[count] != '\0' && count < 16) {
    //checking if is letter or number
      if ((key[count] > 64 && key[count] < 71)||(key[count] > 96 && key[count] < 103) || (key[count] > 47 && key[count] < 58)) {
        if (key[count] > 96 && key[count] < 103) {
          myKey[count] = key[count]-32;
        }
        myKey[count] = key[count];
        count++;
      }
      else{
        myKey[count] = '0';
        count++;
      }

  }

  //-----------------------Crypto preparation---------------//
  src = kmalloc(npages*sizeof(struct scatterlist), __GFP_ZERO|GFP_KERNEL);
   if (!src) {
       printk("taes ERROR: failed to alloc src\n");
       return 1;
   }
   dst = kmalloc(npages*sizeof(struct scatterlist), __GFP_ZERO|GFP_KERNEL);
   if (!dst) {
       printk("taes ERROR: failed to alloc dst\n");
       kfree(src);
       return 1;
   }
   ins = kmalloc(npages*sizeof(char*), __GFP_ZERO|GFP_KERNEL);
   if (!ins) {
       printk("taes ERROR: failed to alloc ins\n");
       kfree(src);
       kfree(dst);
       return 1;
   }
   outs = kmalloc(npages*sizeof(char*), __GFP_ZERO|GFP_KERNEL);
   if (!outs) {
       printk("taes ERROR: failed to alloc outs\n");
       kfree(src);
       kfree(dst);
       kfree(ins);
       return 0;
   }

   tfm = crypto_alloc_blkcipher("ecb(aes-generic)", 0, 0);

   if (IS_ERR(tfm)) {
       printk("failed to load transform for %s: %ld\n", AES_GENERIC, PTR_ERR(tfm));
       goto out;
   }
   desc.tfm = tfm;
   desc.flags = 0;

   ret = crypto_blkcipher_setkey(tfm, myKey, sizeof(myKey));
  if (ret) {
      printk("setkey() failed flags=%x\n", crypto_blkcipher_get_flags(tfm));
       goto out;
  }

  sg_init_table(src, npages);
    for (i=0; i<npages; i++) {
        buf = (void *)__get_free_page(GFP_KERNEL);
        if (!buf) {
            printk("taes ERROR: alloc free page error\n");
            goto free_err_pages;
        }
        ins[i] = buf;
        sg_set_buf(src+i, buf, PAGE_SIZE);
        buf = (void *)__get_free_page(GFP_KERNEL);
        if (!buf) {
            printk("taes ERROR: alloc free page error\n");
            goto free_err_pages;
        }
        outs[i] = buf;
        sg_set_buf(dst+i, buf, PAGE_SIZE);
    }

    for (bs = MIN_BLK_SIZE; bs <= MAX_BLK_SIZE; bs*=2) {
        struct timeval t0, t1;
        long int enc, dec;

        do_gettimeofday(&t0);
        for (j=0; j<TEST_TIMES; j++) {
            if (j%2==0)
                ret = crypto_blkcipher_encrypt(&desc, dst, src, bs);
            else
                ret = crypto_blkcipher_encrypt(&desc, src, dst, bs);
            if (ret) {
                printk("taes ERROR: enc error\n");
                goto free_err_pages;
            }
        }
        do_gettimeofday(&t1);
        enc = 1000000*(t1.tv_sec-t0.tv_sec) +
            ((int)(t1.tv_usec) - (int)(t0.tv_usec));

        do_gettimeofday(&t0);
        for (j=0; j<TEST_TIMES; j++) {
            if (j%2==0)
                ret = crypto_blkcipher_decrypt(&desc, src, dst, bs);
            else
                ret = crypto_blkcipher_decrypt(&desc, dst, src, bs);
            if (ret) {
                printk("taes ERROR: dec error\n");
                goto free_err_pages;
            }
        }
        do_gettimeofday(&t1);
        dec = 1000000*(t1.tv_sec-t0.tv_sec) +
            ((int)(t1.tv_usec) - (int)(t0.tv_usec));

        printk("Size %u, enc %ld, dec %ld\n",
            bs, enc, dec);
    }

    //----------free--------------//
    free_err_pages:
    for (i=0; i<npages && ins[i]; i++){
        free_page((unsigned long)ins[i]);
    }
    for (i=0; i<npages && outs[i]; i++){
        free_page((unsigned long)outs[i]);
    }
out:
    kfree(src);
    kfree(dst);
    kfree(ins);
    kfree(outs);
    crypto_free_blkcipher(tfm);

  //--------------Print Ending------------//
  pr_info("Count: %d\n", count);
  while (count < 16) {
    myKey[count] = '0';
    count++;
  }

  //------------PHRASE CHECKING----------------//
  while (phrase[count] != '\0' && count2 < 16) {
    //checking if is letter or number
      if ((phrase[count2] > 64 && phrase[count2] < 71)||(phrase[count2] > 96 && phrase[count2] < 103) || (phrase[count2] > 47 && phrase[count2] < 58)) {
        if (phrase[count2] > 96 && phrase[count2] < 103) {
          myPhrase[count2] = phrase[count2]-32;
        }
        myPhrase[count2] = phrase[count2];
        count2++;
      }
      else{
        myPhrase[count2] = '0';
        count2++;
      }

  }

  while (count2 < 16) {
    myPhrase[count2] = '0';
    count2++;
  }
  pr_info("key is a string: %s\n", key);
  pr_info("Count: %d\n", count);
  pr_info("phrase is: %s\n",phrase);
  //pr_info("size: %d\n", count);
  return 0;
}


static void __exit hello_5_exit(void)
{
  pr_info("Goodbye, world 5\n");
}

/*static void validarKey(char *key){
}*/

module_init(hello_5_init);
module_exit(hello_5_exit);
