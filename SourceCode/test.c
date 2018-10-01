#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SIZE_BLOCk 16

int validarKey(char *password){
	int count =0;
	if (strlen(password) < 16) {
		count = strlen(password);
		while (count < 16) {
			strcat(password, "0");
			count++;
		}
	//	strcat(password, '\0');
		count = 0;
		while(password[count] != '\0'){
			if(isalpha(password[count])){
				if (password[count] < 103 && password[count] > 96) {
						password[count] = password[count]-32;
				}
				else{
					if (password[count] < 65 || password[count] > 70) {
						printf("Invalid\n");
						return 0;
					}
				}
				/*if(password[count] >= 97 && isalpha(password[count])){
					password[count] = password[count]-32;
				}*/
			}
			else{
				if (!isdigit(password[count])) {
					printf("Invalid password type");
					return 0;
				}

			}
			count++;
		}
		printf("Password: %s\n", password);
		return 1;
	}
	else{
		if (strlen(password) == 16) {
			password[16] = '\0';
		}
		count = 0;
		while(password[count] != '\0'){
			if(isalpha(password[count])){
				if (password[count] < 103 && password[count] > 96) {
						password[count] = password[count]-32;
				}
				else{
					if (password[count] < 65 || password[count] > 70) {
						printf("Invalid\n");
						return 0;
					}
				}
				/*if(password[count] >= 97 && isalpha(password[count])){
					password[count] = password[count]-32;
				}*/
			}
			else{
				if (!isdigit(password[count])) {
					printf("Invalid password type");
					return 0;
				}

			}
			count++;
		}
		printf("Password: %s\n", password);
		return 1;
	}


}

void validarInput(char *input){
	int count = 0, size = 0;
	size = strlen(input);
	count = size;
	while ((strlen(input) % SIZE_BLOCk) != 0) {
		input[count+2] = '0';
		input[count+3] = '\0';
		count++;
	}
	printf("%s\n", input);
	//printf("%d\n", size);
}

void main(int argc, char *argv[]){
	int size =0, cont;
	char *password;
	char *test;
	password = argv[1];
	printf("%d\n", strlen(password));
	cont = validarKey(password);
	test = argv[2];
	//validarInput(test);
}
