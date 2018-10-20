
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
 *	 Programa Utilizado para facilitar na Comunicação com o Módulo
 */

#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

#define BUFFER_LENGTH   80
#define DEVICE          "/dev/cryptomodule"

typedef	enum STATES	{
 	ENCRYPT, DECRYPT, SUMHASH
 } STATES;

static char receive[BUFFER_LENGTH];

void    show_hash_result(char * hash_sha256);

int     main( int argc, char *argv[] )
{
    int i;
    int fd, operation = -1, bytesRead = 0;
    char  command[80];
    char *stringToSend;

    if( argc < 2 )
    {
      perror("[TestProgram] | No Arguments Provided.");
      return errno;
    }
    if( argc > 2 )
    {
      perror("[TestProgram] | Too many Arguments Provided.");
      return errno;
    }
    stringToSend  = argv[1];

    printf("\n\n[TestProgram] | Initialized\n");

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

    usleep(50);

   printf("[TestProgram] | Reading from the device...\n");
   // Read the response from the Module
   bytesRead = read(fd, receive, BUFFER_LENGTH);
   if ( bytesRead < 0 )
   {
      perror("[TestProgram] | Failed to read the message from the device.");
      return errno;
   }

   if( stringToSend[0] == 'c' )        operation = ENCRYPT;
   else if ( stringToSend[0] == 'd' )  operation = DECRYPT;
   else if ( stringToSend[0] == 'h' )  operation = SUMHASH;

   switch (operation)
   {
     case ENCRYPT:
     {
       printf("[TestProgram] | The received message is: '%s'\n", receive);
       break;
     }
     case DECRYPT:
     {
       printf("[TestProgram] | The received message is: '%s'\n", receive);
       break;
     }
     case SUMHASH:
     {
       show_hash_result(receive);
       break;
     }
   }

   printf("[TestProgram] | Terminated\n\n\n");
   return 0;
}

void show_hash_result(char * hash_sha256)
{
    int i;
    char str[32*2 + 1];

    for (i = 0; i < 32 ; i++)
        sprintf(&str[i*2],"%02x", (unsigned char)hash_sha256[i]);
    str[i*2] = 0;
    printf("[TestProgram] | Sum Hash is: '%s'\n", str);;
}
