#include<stdio.h>

int main(int argc, char *argv[]){
	if (argc != 3){
		printf("Usage: encrypt {file} {key}\n");
		return 1;
	}
	printf("%s\n", argv[0]);
}
