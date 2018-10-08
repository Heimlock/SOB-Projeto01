#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <regex.h>

// validarKey
char * validarKey(char* inputKey) {
  int countSize = 0, i;
  char outputKey[16];

/* ---------- START UPPERCASE ---------- */
  // inputKey toUpper
  char *s = inputKey;
  while(*s) {
    *s = toupper((unsigned char) *s);
    s++;
    countSize++;
  }
/* ---------- END UPPERCASE ---------- */

/* ---------- START REGEX ---------- */
  // regex to check for special characters
  regex_t regex;
  int reti;
  // compile regex
  reti = regcomp(&regex, "^[A-Z0-9]", 0);
  if(reti) {
    printf("regex nao compilada\n");
    exit(1);
  }
  // execute regex
  reti = regexec(&regex, inputKey, 0, NULL, 0);
  if(!reti) {
    printf("regex match -> string dentro do padrao\n");
  } else if(reti == REG_NOMATCH) {
    printf("regex no match -> string fora do padrao\n");
  } else {
    printf("regex match falhou\n");
  }

  // free compiled regex
  regfree(&regex);
/* ---------- END REGEX ---------- */

// populando vetor de output nos tres casos
  if(countSize > 16) {
    for(i = 0; i < 16; i++) {
      outputKey[i] = inputKey[i];
    }
  } else if(countSize == 16) {
      for(i = 0; i < countSize; i++) {
        outputKey[i] = inputKey[i];
      }
  } else if(countSize < 16) {
      for(i = 0; i < countSize; i++) {
        outputKey[i] = inputKey[i];
      } for(i = countSize; i < 16; i++) {
        outputKey[i] = '0';
      }
  }

  return outputKey;
}

// validarInput
char * validarInput(char* inputString, int inputSize) {
  // hardcode do sizeBlock = 16 para testes
  int sizeBlock = 16, i;
  char outputArray[inputSize];

  // inputString toUpper
  char *s = inputString;
  while(*s) {
    *s = toupper((unsigned char) *s);
    s++;
  }

  /* ---------- START REGEX ---------- */
    // regex to check for special characters
    regex_t regex;
    int reti;
    // compile regex
    reti = regcomp(&regex, "^[A-Z0-9]", 0);
    if(reti) {
      printf("regex nao compilada\n");
      exit(1);
    }
    // execute regex
    reti = regexec(&regex, inputKey, 0, NULL, 0);
    if(!reti) {
      printf("regex match -> string dentro do padrao\n");
    } else if(reti == REG_NOMATCH) {
      printf("regex no match -> string fora do padrao\n");
    } else {
      printf("regex match falhou\n");
    }

    // free compiled regex
    regfree(&regex);
  /* ---------- END REGEX ---------- */

  int mod = inputSize/sizeBlock;
  if(mod == 0) {
    printf("tamanho multiplo de sizeBlock\n");
    outputArray = inputString;
  } // adicionar N '0' ao final da entrada -> ate qual tamanho?
  // -------- VERIFICAR ESSE ELSE --------

  return outputArray;
}

// convertStringToHex
// como serao guardados os valores em hexa?
int * convertStringToHex(char* inputString, int inputSize) {
  // nao sei se preciso sempre do input size
  // inputString deve vir depois de passar pela operacao de validacao
  char outputArray[8], auxConcat[2];
  


  return outputArray;
}

int main() {
  char testString[] = "This is my Test String";

  validarKey(testString);

  getchar();
  return 0;
}
