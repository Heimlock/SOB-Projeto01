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
static char *mystring = "blah";
static int myintArray[2] = { -1, -1 };
static int arr_argc = 0;
/*
 * module_param(foo, int, 0000)
 * The first param is the parameters name
 * The second param is it's data type
 * The final argument is the permissions bits,
 * for exposing parameters in sysfs (if non-zero) at a later stage.
*/
module_param(mystring, charp, 0000);
MODULE_PARM_DESC(mystring, "A character string");
/*
 * module_param_array(name, type, num, perm);
 * The first param is the parameter's (in this case the array's) name
 * The second param is the data type of the elements of the array
 * The third argument is a pointer to the variable that will store the number
 * of elements of the array initialized by the user at module loading time
 * The fourth argument is the permission bits
*/
module_param_array(myintArray, int, &arr_argc, 0000);
MODULE_PARM_DESC(myintArray, "An array of integers");

static int __init hello_5_init(void){
  int i, count = 0;

  while (mystring[count] != '\0') {
    //checking if is letter or number
      if ((mystring[count] > 64 && mystring[count] < 71)||(mystring[count] > 96 && mystring[count] < 103) || (mystring[count] > 47 && mystring[count] < 58)) {
        if (mystring[count] > 96 && mystring[count] < 103) {
          mystring[count] = mystring[count]-32;
        }
        count++;
      }
      else{
        break;
      }

  }
  if (count > 17) {
    mystring[17] = '\0';
  }
  else{
    while (count <17) {
      mystring[count] = '0';
      count ++;
    }
    mystring[17] = '\0';
  }

  pr_info("mystring is a string: %s\n", mystring);
  pr_info("size: %d\n", count);
  //count = validarKey();
  for (i = 0; i < (sizeof myintArray / sizeof (int)); i++)
  {
    pr_info("myintArray[%d] = %d\n", i, myintArray[i]);
  }
  pr_info("got %d arguments for myintArray.\n", arr_argc);
  return 0;
}


static void __exit hello_5_exit(void)
{
  pr_info("Goodbye, world 5\n");
}
module_init(hello_5_init);
module_exit(hello_5_exit);
