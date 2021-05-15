#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int main(int argc, char *argv[]){
	if (argc != 3){
		printf("Usage: encrypt {file} {key}\n");
		return 1;
	}
	
	char *input_file = argv[1];
	char *output_file = malloc(strlen(input_file+4));
	char *key = argv[2];

	strcpy(output_file, input_file);
	strcat(output_file, ".enc");

	printf("%s, %s, %s\n", input_file, output_file, key);
}
