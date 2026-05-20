/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Summer, 2025
* Author: TO_DO
* Professors: Paulo Sousa
************************************************************
# ECHO "=---------------------------------------="
# ECHO "|  COMPILERS - ALGONQUIN COLLEGE (G25)  |"
# ECHO "=---------------------------------------="
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    "
# ECHO "    @@                             @@    "
# ECHO "    @@         @@@@@@@@@@@@@@      @@    "
# ECHO "    @@       @@             @@     @@    "
# ECHO "    @@      @@               @     @@    "
# ECHO "    @@     @@      @@@@@@@         @@    "
# ECHO "    @@     @@      @@              @@    "
# ECHO "    @@     @@      @@   @@@@@@     @@    "
# ECHO "    @@      @@           @@  @     @@    "
# ECHO "    @@       @@         @@  @@     @@    "
# ECHO "    @@        @@       @@   @      @@    "
# ECHO "    @@         @@@@@@@@     @      @@    "
# ECHO "    @@                             @@    "
# ECHO "    @@         G R E V I T Y       @@    "
# ECHO "    @@                             @@    "
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    "
# ECHO "                                         "
# ECHO "[COMPILER SCRIPT .......................]"
# ECHO "                                         "


/*
***********************************************************
* File name: Reader.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013]
* Assignment: A12.
* Date: Jan 01 2025
* Professor: Paulo Sousa
* Purpose: This file is the main code for Buffer/Reader (A12)
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * - Please check the "TODO" labels to develop your activity.
 * - Review the functions to use "Defensive Programming".
 *.............................................................................
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef CODER_H_
#include "Step1Coder.h"
#endif

// Function to perform the Vigenère cipher (encoding or decoding)
void vigenereFile(const grevity_strg inputFileName, const grevity_strg outputFileName, const grevity_strg key, grevity_intg encode) {

	// TO_DO: Define the input and output files (ex: FILE* inputFile, FILE* outputFile
	FILE *inputFile = fopen(inputFileName, "r");
	FILE *outputFile = fopen(outputFileName, "w");

	// TO_DO: Use defensive programming (checking files)
	if (inputFile == NULL) {
		perror("Error opening input file");
		return;
	}
	if (outputFile == NULL) {
		perror("Error opening output file");
		fclose(inputFile);
		return;
	}

	// TO_DO: Define local variables
	char ch;
	int keyIndex = 0;
	int keyLength = strlen(key);

	// TO_DO: Logic: check if it is encode / decode to change the char (using Vigenere algorithm) - next function
	while ((ch = fgetc(inputFile)) != EOF) {
		// Check if the character is a visible ASCII character
		if (ch >= ASCII_START && ch <= ASCII_END) {
			char keyChar = key[keyIndex % keyLength]; // Get the corresponding key character
			int shift = keyChar - ASCII_START; // Calculate the shift based on the key character

			// Apply the Vigenère cipher logic
			if (encode) {
				ch = ((ch - ASCII_START + shift) % ASCII_RANGE) + ASCII_START; // Encoding
			} else {
				ch = ((ch - ASCII_START - shift + ASCII_RANGE) % ASCII_RANGE) + ASCII_START; // Decoding
			}
			keyIndex++; // Move to the next character in the key
		}
		fputc(ch, outputFile); // Write the processed character to the output file
	}

	// TO_DO: Close the files
	fclose(inputFile);
	fclose(outputFile);
}

// Function to perform the Vigenère cipher (encoding or decoding)
grevity_strg vigenereMem(const grevity_strg inputFileName, const grevity_strg key, grevity_intg encode) {
	FILE *inputFile = fopen(inputFileName, "r");
	if (inputFile == NULL) {
		perror("Error opening input file");
		return NULL;
	}
	grevity_intg size = getSizeOfFile(inputFileName);
	if (size <= 0) {
		fclose(inputFile);
		return NULL;
	}
	grevity_strg output = (grevity_strg)malloc(size + 1);
	if (output == NULL) {
		fclose(inputFile);
		perror("Memory allocation failed");
		return NULL;
	}

	// **FIX: Use bytesRead from fread**
	size_t bytesRead = fread(output, sizeof(grevity_char), size, inputFile);
	output[bytesRead] = '\0'; // Null-terminate just after last valid byte!

	int keyIndex = 0;
	int keyLength = strlen(key);
	for (size_t i = 0; i < bytesRead; i++) {
		char ch = output[i];
		if (ch >= ASCII_START && ch <= ASCII_END) {
			char keyChar = key[keyIndex % keyLength];
			int shift = keyChar - ASCII_START;
			if (encode) {
				output[i] = ((ch - ASCII_START + shift) % ASCII_RANGE) + ASCII_START;
			} else {
				output[i] = ((ch - ASCII_START - shift + ASCII_RANGE) % ASCII_RANGE) + ASCII_START;
			}
			keyIndex++;
		}
	}

	fclose(inputFile);
	return output;
}
// Function to encode (cypher)
void cypher(const grevity_strg inputFileName, const grevity_strg outputFileName, const grevity_strg key) {
    vigenereFile(inputFileName, outputFileName, key, CYPHER);
}

// Function to decode (decypher)
void decypher(const grevity_strg inputFileName, const grevity_strg outputFileName, const grevity_strg key) {
    vigenereFile(inputFileName, outputFileName, key, DECYPHER);
}

// TO_DO: Get file size (util method)
// Function to get the size of the file
grevity_intg getSizeOfFile(const grevity_strg filename) {
	// TO_DO: Use the logic to get the size of the file
	FILE *file = fopen(filename, "r");
	if (file == NULL) {
		perror("Error opening file to get size");
		return -1; // Return -1 if the file cannot be opened
	}
	fseek(file, 0, SEEK_END); // Move to the end of the file
	grevity_intg size = ftell(file); // Get the current position (size of the file)
	fclose(file); // Close the file
	return size; // Return the size
}
