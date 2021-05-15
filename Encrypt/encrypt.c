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

	unsigned char *buffer = 0;
        long length;
        FILE *f;

	f = fopen(input_file, "rb");
        if (f)
        {
                fseek(f, 0, SEEK_END);
                length = ftell(f);
                fseek(f, 0, SEEK_SET);
                buffer = malloc(length);
                if (buffer)
                {
			fread(buffer, 1, length, f);
                }
                fclose(f);
        }

}