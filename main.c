/*
 *  Author: Renato Mascarenhas Costa
 *  renato [dot] mascosta [at] gmail [dot] com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <ctype.h>

#ifndef STRING_H
#define STRING_H 1
#include <string.h>
#endif /* STRING_H */

#include "goldeen.h"

#define GOLDEEN_MAX_PASSWORD_LENGTH 56


void
print_help_message() { 
	printf("\
Usage:\n\
=====\n\n\
\tgoldeen [action] [options]\n\n\
Actions:\n\
=======\n\n\
\tencrypt                       - encrypt the input with the Blowfish algorithm.\n\
\tdecrypt                       - decrypt the input with the Blowfish algorithm.\n\n\
Options:\n\
=======\n\n\
\t-h|--help                    - prints help message and finishes.\n\
\t-i|--input <inputfile>       - takes inputfile as the content to be encrypted/decrypted. Otherwise, you will be prompted.\n\
\t-o|--output <outputfile>     - writes the output of the algorithm to outputfile. Otherwise, stdout will be used.\n\
\t-k|--key-file <keyfile>      - If this options is passed, goldeen will use the content of the keyfile as the key\n\
                                 to the algorithm (56 bytes at most). If the file is bigger, only the first 56 bytes will be considered.\n\n\
License:\n\
=======\n\n\
goldeen - v0.5 - Copyright (C) 2011 Renato Mascarenhas\n\n\
This program is free software: you can redistribute it and/or modify\n\
it under the terms of the GNU General Public License as published by\n\
the Free Software Foundation, either version 3 of the License, or\n\
(at your option) any later version.\n\n\
This program is distributed in the hope that it will be useful,\n\
but WITHOUT ANY WARRANTY; without even the implied warranty of\n\
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n\
GNU General Public License for more details.\n");
}

/* Reads a key input from user, disabling echo property and then
 * turning it on again.
 */
void
read_key(byte_t* key) { 
	static struct termios old_term, new_term;
	int i = 0, c;

	tcgetattr(STDIN_FILENO, &old_term);
	new_term = old_term;

	new_term.c_lflag &= ~(ECHO);

	tcsetattr(STDIN_FILENO, TCSANOW, &new_term);

	while ((c = getchar()) != '\n' && c != EOF) {
		key[i++] = c;
	}

	key[i] = '\0';

	tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
}

/* Removes trailing new line or line feed */
void
chomp(byte_t* str) { 
	char* new_line;

	new_line = strrchr((char*) str, '\r');
	if (new_line) {
		*new_line = 0;
	}

	new_line = strrchr((char*) str, '\n');
	if (new_line) {
		*new_line = 0;
	}
}

/* Reads a file and stores all its content in the buffer
 * parameter, or at most max_bytes, if bigger than 0.
 */
void
read_file(char* filename, byte_t** buffer, int max_bytes) { 
	FILE* fd;
	int file_size;

	if ((fd = fopen(filename, "r")) == NULL) {
		printf("Could not open file `%s`.\n", filename);
		exit(1);
	}

	fseek(fd, 0, SEEK_END);
	file_size = ftell(fd);
	rewind(fd);

	if (max_bytes > 0 && file_size > max_bytes) {
		file_size = max_bytes + 1;
	}

	*buffer = (byte_t*) malloc(file_size*sizeof(byte_t));
	if (*buffer == NULL) {
		printf("Memory error: file %s is too big.\n", filename);
		exit(1);
	}

	fread(*buffer, 1, file_size, fd);
	chomp(*buffer);

	fclose(fd);
}

/* Makes a byte array, given the string read.
 * Takes every chars as a byte in the array.
 */
void
make_byte_array(byte_t* msg, byte_t* array) { 
	int i = 0;

	while (*msg != '\0') {
		if (*msg > '9') {
			array[i] = ((tolower(*msg) - 'a') + 10) << 4;
		} else {
			array[i] = (*msg - '0') << 4;
		}

		++msg;

		if (*msg > '9') {
			array[i] |= tolower(*msg) - 'a' + 10;
		} else {
			array[i] |= *msg - '0';
		}

		++msg; ++i;
	}
}

/* Parses the input given by the user. If inputfile, outputfile or keyfile are given, allocate
 * memory for them and copy the file names.
 */
void
parse_command(int argc, char** argv, char** inputfile, char** outputfile, char** keyfile, char* action) { 
	int i = 1;

	while (i < argc) {
		if ((!strcmp(argv[i], "-i") || !strcmp(argv[i], "--input")) && i+1 < argc) {
			*inputfile = (char*) malloc(255*sizeof(char));
			strncpy(*inputfile, argv[++i], 255);

		} else if ((!strcmp(argv[i], "-o") || !strcmp(argv[i], "--output")) && i+1 < argc) {
			*outputfile = (char*) malloc(255*sizeof(char));
			strncpy(*outputfile, argv[++i], 255);

		} else if ((!strcmp(argv[i], "-k") || !strcmp(argv[i], "--key-file")) && i+1 < argc) {
			*keyfile = (char*) malloc(255*sizeof(char));
			strncpy(*keyfile, argv[++i], 255);

		} else if (!strcmp(argv[i], "encrypt")) {
			*action = ENCRYPT;

		} else if (!strcmp(argv[i], "decrypt")) {
			*action = DECRYPT;
		} else if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
			print_help_message();
			exit(0);
		}

		++i;
	}
}

int
main(int argc, char* argv[]) { 
	char* inputfile = NULL;
	char* outputfile = NULL;
	char* keyfile = NULL;
	char action = -1;

	byte_t* input = NULL;
	byte_t* output = NULL;
	byte_t* key = NULL;

	char* verb = (char*) malloc(10*sizeof(char));
	byte_t* msg = NULL; 

	int cipher_length = 0, i;
	int safe;

	FILE* fd;
	
	parse_command(argc, argv, &inputfile, &outputfile, &keyfile, &action);

	if (action == -1) {
		printf("No action specified. Please use -h for more information.\n");
		exit(1);
	}

	verb = (action == ENCRYPT) ? "encrypted" : "decrypted";

	if (inputfile == NULL) {
		printf("Content to be %s: ", verb);
		input = (byte_t*) malloc(1024*sizeof(byte_t));

		if (action == ENCRYPT) {
			scanf("%1024[^\n]s", input);
			getchar();

		} else if (action == DECRYPT) {
			msg = (byte_t*) malloc(1024*sizeof(byte_t));
			scanf("%1024[^\n]s", msg);
			getchar();

			make_byte_array(msg, input);
		}
	
	} else {
		read_file(inputfile, &msg, 0);

		cipher_length = (strlen((char*) msg) + 1)/2;
		input = (byte_t*) malloc(strlen((char*) msg)*sizeof(char) + 1);
		memcpy(input, msg, strlen((char*) msg) + 1);

		if (action == DECRYPT) {
			make_byte_array(msg, input);
		}
	}

	if (keyfile == NULL) {
		key = (byte_t*) malloc(GOLDEEN_MAX_PASSWORD_LENGTH*sizeof(byte_t));
		printf("Key: ");
		read_key(key);
		printf("\n");
	} else {
		read_file(keyfile, &key, GOLDEEN_MAX_PASSWORD_LENGTH);
	}

	if (action == ENCRYPT) {
		cipher_length = encrypt((char*) key, input, &output, &safe);

		if (outputfile == NULL) {
			for (i = 0; i < cipher_length; ++i) {
				printf("%02x", output[i]);
			}
			printf("\n");
		} else {
			if ((fd = fopen(outputfile, "w")) != NULL) {
				for (i = 0; i < cipher_length; ++i) {
					fprintf(fd, "%02x", output[i]);
				}
				fclose(fd);
			} else {
				printf("Could not open output file `%s`.\n", outputfile);
			}
		}

		if (!safe) {
			printf("WARNING: Weak key. It is recommended to choose a better one\n");
		}

	} else if (action == DECRYPT) {
		decrypt((char*) key, input, cipher_length, &output);

		if (outputfile == NULL) {
			printf("%s\n", output);
		} else {
			if ((fd = fopen(outputfile, "w")) != NULL) {
				fwrite(output, cipher_length, 1, fd);
				fclose(fd);
			} else {
				printf("Could not open output file %s.\n", outputfile);
			}
		}
	} 

	free(inputfile);
	free(outputfile);
	free(keyfile);
	free(input);
	free(output);
	free(key);
	free(msg);


	/*char key[56];
	byte_t text[255];
	byte_t* cipher, *plain_text;

	printf("Content: ");
	scanf("%[^\n]s", text);
	getchar();

	printf("Key: ");
	scanf("%[^\n]s", key);
	getchar();

	int i, cipher_length;
	
	cipher_length = encrypt(key, text, &cipher);
	
	for (i = 0; i < cipher_length; ++i) {
		printf("%02x", cipher[i]);
	}

	printf("\n\nDecrypt:\n");
	decrypt(key, cipher, cipher_length, &plain_text);
	
	printf("%s\n", plain_text);*/ 

	return 0;
}
