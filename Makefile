CC = gcc
LDFLAGS = -lssl -lcrypto -lm
CFLAGS = -I/usr/local/include -static
BIN = encrypt

all: encrypt.o
	$(CC) $(CFLAGS) encrypt.o -o $(BIN) $(LDFLAGS)

encrypt: encrypt.c
	$(CC) $(CFLAGS) encrypt.c -o encrypt.o $(LDFLAGS)

