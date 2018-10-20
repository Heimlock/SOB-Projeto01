
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
 *	 Operações Auxiliares
 */

#include "CommonLib.h"

MODULE_AUTHOR("Felipe Ferreira");
MODULE_DESCRIPTION("Auxiliary Functions");
MODULE_LICENSE("GPL");

/*
 *    [FF]  =>  "FF"
 */
void    serialize( char input[], char output[], int sizeIN )
{
    int i;

    memset( output, 0, (2 * sizeIN) );
    for( i = 0; i < sizeIN; i++)
    {
      sprintf( output + (2*i), "%02hhX", input[i] );
    }
}

/*
 *    "FF" => 255
 */
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

    // #ifdef    DEBUG
    //   printk("Str[%d , %d]: %c%c | 0x%02hhX\n", 0, 1, str[0], str[1], rtn);
    // #endif
    return  rtn;
}

/*
 *    "FF"  =>  [FF]
 */
void    deserialize( char input[], char output[], int sizeIN )
{
    int     i, k;
    char    aux;

    memset( output, 0, (sizeIN / 2) );
    for(i = 0, k = 0; i < sizeIN; i = i + 2, k++)
    {
        sprintf(&aux, "%c", hex2int( &input[i]) );
        output[k] = aux;

        #ifdef    DEBUG
          pr_info("[%s] | Deserialize -- Output[%2d] == 0x%02hhX\n", DEVICE_NAME, k, output[k] );
        #endif
    }
}

int     arrangeText( char input[], char **output, int size )
{
    int   i = 1, actualSize = 0;

    if( (*output) != NULL )
        vfree((*output));

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

    #ifdef    DEBUG
      pr_info( "[%s] | ArrangeText: %s\n", DEVICE_NAME, (*output) );
    #endif

    return  actualSize;
    output_free:
      return -1;
}

void    printHex( char input[], int size, char *info )
{
    char  *bufferAux;

    bufferAux  = vmalloc( (size * 2) + 1 );
    if( !bufferAux )
    {
       pr_crit("[%s] | printHex -- Failed to Alloc bufferAux!\n", DEVICE_NAME);
       goto bufferAux_free;
    }

    serialize( input, bufferAux, size );
    bufferAux[(2 * size)] = '\0';
    pr_info( "[%s] | %s: %s\n", DEVICE_NAME, info, bufferAux );

    vfree(bufferAux);
    bufferAux_free:
    return;
}

/*
 *    Verify if Input Has only valid Characters, And Allocate Output with the Valid Correspondent;
 *    If Input is Smaller than Expected size, fill the remaining with zeros;
 *    If Input is Bigger than Expected size, Copy only the 'size' first values;
 */
int     validate( char *source, char **destiny, int size )
{
    int count = 0;

    if( (*destiny) != NULL )
        vfree((*destiny));

    if ( (size % 2) != 0 )
    {
        pr_err("[%s] | validate -- Source hasn't Even lenght!\n", DEVICE_NAME);
        return -1;
    }

    //  Allocing Destiny
    (*destiny) = vmalloc(size);
    if ((*destiny) == 0)
    {
        pr_crit("[%s] | validate -- Failed to Alloc destiny!\n", DEVICE_NAME);
        return -1;
    }

    //  Passe Source to Destiny after Destiny is Alloced
    while (source[count] != '\0' && count < size)
    {
        //  Checking if is Letter or Number
        if ((source[count] > 64 && source[count] < 71) || (source[count] > 96 && source[count] < 103) || (source[count] > 47 && source[count] < 58))
        {
            if (source[count] > 96 && source[count] < 103)
                (*destiny)[count] = ( source[count] - 32 );
            else
                (*destiny)[count] = source[count];
            count++;
        }
        else
        {
            pr_crit("[%s] | validate -- Not a valid Character!\n", DEVICE_NAME);
            return -1;
        }
    }

    while (count < size)
    {
        (*destiny)[count] = '0';
        count++;
    }
    return 0;
}
