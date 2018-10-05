/*
 *  hello-5.c - Demonstrates command line argument passing to a module.
*/
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/stat.h>
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

  int count = 0;
  char myKey[17];

  while (key[count] != '\0') {
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

  while (count < 16) {
    myKey[count] = '0';
    count++;
  }
  myKey[count] = '\0';
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
