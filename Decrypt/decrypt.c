#include<stdio.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include<string.h>
#include<sha1.h>
#include<sha2.h>
#include<stdlib.h>

void handleErrors(void);

int
decrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
    unsigned char *iv, unsigned char *ciphertext);

int
main(int argc, char *argv[])
{

	if (argc != 4) {
		printf("Usage: decrypt {input file} {output file} {key}\n");
		return 1;
	}
	char *input_file = argv[1];
	char *output_file = argv[2];
	char *password = argv[3];

	unsigned char *buffer = 0;
	long length;
	FILE *f;

	f = fopen(input_file, "rb");
	printf("%s\n", input_file);

	if (f) {
		fseek(f, 0, SEEK_END);
		length = ftell(f);
		if (length < 32) {
			fprintf(stderr, "File is too small!\n");
			return 1;
		}
		fseek(f, 0, SEEK_SET);
		buffer = malloc(length);
		if (buffer) {
			fread(buffer, 1, length, f);
		}
		fclose(f);
	} else {

		fprintf(stderr, "Error opening file!\n");
		return 1;
	}

	unsigned char *key = (unsigned char *) SHA256Data(password, strlen(password), NULL);
	unsigned char *iv = (unsigned char *) SHA1Data(password, strlen(password), NULL);

	unsigned char *decryptedtext = malloc(length);

	int decryptedtext_len = decrypt(buffer, length, key, iv, decryptedtext);

	f = fopen(output_file, "w");
	fwrite(decryptedtext, decryptedtext_len, 1, f);
	fclose(f);

}

void
handleErrors(void)
{
	ERR_print_errors_fp(stderr);
	abort();
}

int 
decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
    unsigned char *iv, unsigned char *plaintext)
{
	EVP_CIPHER_CTX *ctx;

	int len;

	int plaintext_len;

	if (!(ctx = EVP_CIPHER_CTX_new()))
		handleErrors();

	if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
		handleErrors();

	if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
		handleErrors();
	plaintext_len = len;

	if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
		handleErrors();
	plaintext_len += len;

	EVP_CIPHER_CTX_free(ctx);

	return plaintext_len;
}
