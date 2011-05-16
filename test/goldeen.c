#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N_TESTS 1000
#define TESTS_LENGTH 1024
#define KEY_LENGTH 56

#include "goldeen.h"

char
random_char() { 
	return (rand() % 94) + 32;
}

int
main(int argc, char* argv[]) { 
	char** tests = NULL;
	char** keys = NULL;

	byte_t* cipher;
	byte_t* plain_text;

	int i, j, safe, cipher_length, errors = 0;

	tests = (char**) malloc(N_TESTS*sizeof(char*));
	keys = (char**) malloc(N_TESTS*sizeof(char*));

	for (i = 0; i < N_TESTS; ++i) { 
		tests[i] = (char*) malloc(TESTS_LENGTH*sizeof(char));
		keys[i] = (char*) malloc(KEY_LENGTH*sizeof(char));
	}

	printf("Generating random texts...\n");

	srand(time(NULL));

	for (i = 0; i < N_TESTS; ++i) {
		for (j = 0; j < TESTS_LENGTH; ++j) {
			tests[i][j] = random_char();
		}
		tests[i][TESTS_LENGTH - 1] = '\0';
	}

	printf("Generating random keys...\n");

	for (i = 0; i < N_TESTS; ++i) {
		for (j = 0; j < KEY_LENGTH; ++j) {
			keys[i][j] = random_char();
		}
		keys[i][KEY_LENGTH - 1] = '\0';
	}

	printf("Testing... ");

	for (i = 0; i < N_TESTS; ++i) {
		cipher_length = encrypt(keys[i], (byte_t*) tests[i], &cipher, &safe);
		decrypt(keys[i], cipher, cipher_length, &plain_text);

		if (memcmp(tests[i], plain_text, cipher_length) != 0) {
			errors += 1;
		}
	}

	printf("done.\n\n");

	for (i = 0; i < N_TESTS; ++i) {
		free(tests[i]);
	}

	printf("%d tests, %d errors.\n", N_TESTS, errors);

	return 0;

}
