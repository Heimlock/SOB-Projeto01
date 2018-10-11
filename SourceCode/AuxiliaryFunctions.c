
#include "CommonLib.h"

MODULE_AUTHOR("Felipe Ferreira");
MODULE_DESCRIPTION("Auxiliary Functions");
MODULE_LICENSE("GPL");

void    serialize( char input[], char output[], int sizeIN )
{
    int i;

    memset( output, 0, (2 * sizeIN) );
    for( i = 0; i < sizeIN; i++)
    {
      sprintf( output + (2*i), "%02hhX", input[i] );
    }
}

int     hex2int( char str[] )
{
    int   i;
    int   values[2], rtn;

    for( i = 0; i < 2; i++ )
    {
      if( str[i] == 'A' )         values[i] =  10;
      else  if( str[i] == 'B' )   values[i] =  11;
      else  if( str[i] == 'C' )   values[i] =  12;
      else  if( str[i] == 'D' )   values[i] =  13;
      else  if( str[i] == 'E' )   values[i] =  14;
      else  if( str[i] == 'F' )   values[i] =  15;
      else  values[i]   = ((int)str[i] - (int)'0');
    }

    rtn   = (values[1] + (16 * values[0]));

    #ifdef    DEBUG
      printk("Str[%d , %d]: %c%c | 0x%02hhX\n", 0, 1, str[0], str[1], rtn);
    #endif
    return  rtn;
}

void    deserialize(char input[], char output[], int sizeIN)
{
    int     i, k;
    char    aux;

    memset( output, 0, (sizeIN / 2) );
    for(i = 0, k = 0; i < sizeIN; i = i + 2, k++)
    {
        sprintf(&aux, "%c", hex2int( &input[i]) );
        output[k] = aux;
    }
}

int     arrangeText( char input[], char **output, int size )
{
    int   i = 1, actualSize = 0;

    while ( actualSize == 0 )
    {
        if( (i * KEY_LENGHT) >= size )
          actualSize = (i * KEY_LENGHT);
        else
          i++;
    }

    (*output) = vmalloc(actualSize);
    if (!(*output))
    {
       pr_crit("[%s] | ArrangeText -- Failed to Alloc str!\n", DEVICE_NAME);
       goto output_free;
    }
    memset( (*output), '\0', actualSize);

    for( i = 0; i < size; i++ )
      (*output)[i]  = input[i];

    return  actualSize;
    output_free:
      return 0;
}

void    printHex( char input[], int size, char *info )
{
    char  *buffer;

    buffer  = vmalloc( (size * 2) );
    if( !buffer )
    {
       pr_crit("[%s] | printHex -- Failed to Alloc buffer!\n", DEVICE_NAME);
       goto buffer_free;
    }

    serialize( input, buffer, size );
    pr_info( "[%s] | %s: %s\n", DEVICE_NAME, info, buffer );

    vfree(buffer);
    buffer_free:
    return;
}