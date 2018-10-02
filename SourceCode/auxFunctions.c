#include <stdio.h>
#include <stdlib.h>

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
