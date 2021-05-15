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
encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
    unsigned char *iv, unsigned char *ciphertext);

int
main(int argc, char *argv[])
{

	if (argc != 3) {
		printf("Usage: encrypt {file} {key}\n");
		return 1;
	}
	char *input_file = argv[1];
	char *output_file = malloc(strlen(input_file + 4));
	char *password = argv[2];

	strcpy(output_file, input_file);
	strcat(output_file, ".enc");

	unsigned char *buffer = 0;
	long length;
	FILE *f;

	f = fopen(input_file, "rb");
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

	unsigned char *ciphertext = malloc(2 * length);

	int ciphertext_len = ciphertext_len = encrypt(buffer, length, key, iv, ciphertext);

	f = fopen(output_file, "w");
	fwrite(ciphertext, ciphertext_len, 1, f);
	fclose(f);

}

void
handleErrors(void)
{
	ERR_print_errors_fp(stderr);
	abort();
}

int
encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
    unsigned char *iv, unsigned char *ciphertext)
{
	EVP_CIPHER_CTX *ctx;

	int len;
	int ciphertext_len;

	if (!(ctx = EVP_CIPHER_CTX_new()))
		handleErrors();

	if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
		handleErrors();

	if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
		handleErrors();

	ciphertext_len = len;

	if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
		handleErrors();
	ciphertext_len += len;

	EVP_CIPHER_CTX_free(ctx);

	return ciphertext_len;
}
