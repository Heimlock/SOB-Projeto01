#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <regex.h>

// validarKey
char * validarKey(char* inputKey) {
  int countSize = 0, i;

  // inputKey toUpper
  char *s = inputKey;
  while(*s) {
    *s = toupper((unsigned char) *s);
    s++;
    countSize++;
  }

  // regex to check for special characters
  regex_t regex;
  int reti;
  // compile regex
  reti = regcomp(&regex, "^[A-Z0-9]", 0);
  if(reti) {
    printf("regex nao compilada");
    exit(1);
  }
  // execute regex
  reti = regexec(&regex, inputKey, 0, NULL, 0);
  if(!reti) {
    printf("regex match -> string dentro do padrao");
  } else if(reti == REG_NOMATCH) {
    printf("regex no match -> string fora do padrao");
  } else {
    printf("regex match falhou");
  }

  // free compiled regex
  regfree(&regex);

  char outputKey[countSize];
  for(i = 0; i < countSize; i++) {
    outputKey[i] = inputKey[i];
  }

  if(countSize > 16) {
    outputKey[17] = '\0';
  } else if(countSize < 16) {
    for(i = countSize; i < 16; i++) {
      outputKey[i] = '0';
    }
  }

  return outputKey;
}

// validarInput
int validarInput(char* inputString, int inputSize) {

  // inputString toUpper
  char *s = inputString;
  while(*s) {
    *s = toupper((unsigned char) *s);
    s++;
  }

  return 1;
}

// convertStringToHex
int * convertStringToHex(char* inputString, int inputSize) {
  int outputArray[inputSize];

  return outputArray;
}

int main() {
  char testString[] = "This is my Test String";

  validarKey(testString);

  getchar();
  return 0;
}
