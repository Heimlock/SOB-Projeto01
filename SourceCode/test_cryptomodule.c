
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

#define BUFFER_LENGTH   80
#define DEVICE          "/dev/cryptomodule"

static char receive[BUFFER_LENGTH];

int main()
{
    int fd;
    char stringToSend[BUFFER_LENGTH] = "c Felipe Ferreira";


   printf("[TestProgram] | Initialized\n");

    printf("[TestProgram] | Opening the device '%s'.\n", DEVICE);
    // Open the device with read/write access   
    fd  =   open( DEVICE, O_RDWR);
    if ( fd < 0 )
    {
        perror("[TestProgram] | Failed to open the device...");
        return errno;
    }

    printf("[TestProgram] | Writing message to the device '%s'.\n", stringToSend);
    // Send the string to the Module
    if ( write(fd, stringToSend, strlen(stringToSend)) < 0 )
    {
        perror("[TestProgram] | Failed to write the message to the device.");
        return errno;
    }

    sleep(1);

   printf("[TestProgram] | Reading from the device...\n");
   // Read the response from the Module
   if ( read(fd, receive, BUFFER_LENGTH) < 0 )
   {
      perror("[TestProgram] | Failed to read the message from the device.");
      return errno;
   }
   printf("[TestProgram] | The received message is: '%s'\n", receive);

   
   printf("[TestProgram] | Terminated\n");
   return 0;
}