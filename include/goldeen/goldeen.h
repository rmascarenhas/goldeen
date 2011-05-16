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

#ifndef GOLDEEN_GOLDEEN_H
#define GOLDEEN_GOLDEEN_H 1

#ifndef STRING_H
#define STRING_H 1
#include <string.h>
#endif /* STRING_H */

#ifndef STDLIB_H
#define STDLIB_H 1
#include <stdlib.h>
#endif /* STDLIB_H */

/* defines __u32 32b unsigned integer. Blowfish works with 64b blocks */
#include <asm/types.h> 

/* encryption and decryption constants */
#define ENCRYPT 0
#define DECRYPT 1

/* macros for accessing single bytes in a word */
#define FIRSTBYTE(n)	(((n) >> 24) & 255)
#define SECONDBYTE(n)	(((n) >> 16) & 255)
#define THIRDBYTE(n)	(((n) >> 8) & 255)
#define FOURTHBYTE(n)	((n) & 255)


/* 16b sized block */
typedef __u16 goldeen_16b_block;

/* 32b sized block */
typedef __u32 goldeen_32b_block;

/* Blowfish subkeys for internal use: 18 P keys + 4*256 S boxes */
typedef struct { 
    goldeen_32b_block P[18];
	goldeen_32b_block S[4][256];
} subkeys;

/* byte data used for diving data blocks */
typedef unsigned char byte_t;

/* encryption and decryption functions */
int encrypt(const char* user_key, byte_t* content, byte_t** cipher, int* safe);

void decrypt(const char* user_key, void* cipher, int cipher_length, byte_t** plain_text);

/* initial keys of the Blowfish algorithm using digits of Pi */
typedef goldeen_32b_block keys_array[1042];
const keys_array initial_keys;

/* The subkeys variable will be used in the execution of the algorithm */
subkeys keys;

#endif /* GOLDEEN_GOLDEEN_H */
