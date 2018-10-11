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

#define MAX_KEY_SIZE (16)
#define MAX_PHRASE_SIZE (16)

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
int validate(char *source, char **destiny, int size);

static int __init hello_5_init(void){

  int valid1, valid2, i=0;
  char *myKey, *myPhrase;

  valid1 = validate(key, &myKey, MAX_KEY_SIZE);
  valid2 = validate(phrase, &myPhrase, MAX_PHRASE_SIZE);

  if (valid1 == -1) {
    pr_info("Error\n");
    return 0;
  }

  if (valid2 == -1) {
    pr_info("Error\n");
    return 0;
  }


  pr_info("key is a string:");
  for ( i = 0; i < MAX_KEY_SIZE; i++) {
    pr_info("%02hhX", myKey[i]);
  }
  pr_info("phrase is:");
  for ( i = 0; i < 16; i++) {
    pr_info("%02hhX", myPhrase[i]);
  }
  //pr_info("Count: %d\n", count);
  pr_info("phrase is: %s\n",phrase);
  //pr_info("size: %d\n", count);
  return 0;
}


static void __exit hello_5_exit(void)
{
  pr_info("Goodbye, world 5\n");
}

int validate(char *source, char **destiny, int size){
  int count = 0;

  if (size%2 != 0) {
    return -1;
  }
  //allocing destiny
  (*destiny) = vmalloc(size);
  if ((*destiny) == 0) {
    pr_info("ERROR VMALLOC");
    return -1;
  }
  //passe source to destiny after destiny is alloced
  while (source[count] != '\0' && count < size) {
    //checking if is letter or number
      if ((source[count] > 64 && source[count] < 71)||(source[count] > 96 && source[count] < 103) || (source[count] > 47 && source[count] < 58)) {
        if (source[count] > 96 && source[count] < 103) {
          (*destiny)[count] = source[count]-32;
        }
        else{
            (*destiny)[count] = source[count];
        }
        count++;
      }
      else{
        pr_err("Character Invalid");
        return -1;
      }

  }

  //pr_info("Count: %d\n", count);
  while (count < size) {
    (*destiny)[count] = '0';
    count++;
  }
  return 0;
}

/*static void validarKey(char *key){
}*/

module_init(hello_5_init);
module_exit(hello_5_exit);
