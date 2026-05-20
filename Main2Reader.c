/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Summer, 2025
* Author: TO_DO
* Professors: Paulo Sousa
************************************************************
#
# ECHO "=---------------------------------------="
# ECHO "|  COMPILERS - ALGONQUIN COLLEGE (S25)  |"
# ECHO "=---------------------------------------="
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    "
# ECHO "    @@                             @@    "
# ECHO "    @@         G R E V I T Y         @@    "
# ECHO "    @@                             @@    "
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    "
# ECHO "                                         "
# ECHO "[READER MAIN .........................]"
# ECHO "                                         "
*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef CODER_H_
#include "Step1Coder.h"
#endif

#ifndef READER_H_
#include "Step2Reader.h"
#endif

grevity_void displayBuffer(BufferPointer ptr_Buffer);
grevity_long getFileSize(grevity_strg fname);
grevity_intg isNumber(const grevity_strg ns);
grevity_void startReader(grevity_strg, grevity_strg);

grevity_intg main2Reader(grevity_intg argc, grevity_strg* argv) {
    if (argc <= 2) {
        errorPrint("\nDate: %s  Time: %s", __DATE__, __TIME__);
        errorPrint("\nRuntime error at line %d in file %s\n", __LINE__, __FILE__);
        errorPrint("%s%s%s", argv[0], ": ", "Missing parameters.");
        errorPrint("Usage: <Option=2> <SourceFile>");
        exit(EXIT_FAILURE);
    }

    grevity_strg input = argv[2];

    startReader(argv[0], input);

    return EXIT_SUCCESS;
}

grevity_void startReader(grevity_strg program, grevity_strg input) {
    BufferPointer bufferp;
    grevity_intg loadSize = 0;

    // The Vigenère key
    grevity_strg key = STR_LANGNAME;

    // Decrypt the input file into memory
    grevity_strg decrypted = vigenereMem(input, key, DECYPHER);

    if (decrypted == NULL) {
        errorPrint("%s%s", program, ": Decryption failed or file not found.");
        errorPrint("Filename: %s \n", input);
        exit(EXIT_FAILURE);
    }

    bufferp = grevityReaderCreate(READER_DEFAULT_SIZE);
    if (bufferp == NULL) {
        errorPrint("%s%s", program, ": Cannot allocate buffer - Use: buffer <input>.");
        errorPrint("Filename: %s \n", input);
        free(decrypted);
        exit(EXIT_FAILURE);
    }



    loadSize = grevityReaderLoadFromString(bufferp, decrypted);

    grevityReaderChecksum(bufferp);

    if (loadSize == READER_ERROR) {
        printf("The input file %s %s\n", input, "has not been completely loaded.");
        printf("Current size of buffer: %d.\n", grevityReaderGetSize(bufferp));
    }

    displayBuffer(bufferp);

    // Show decrypted text again after the table
    printf("\nDecrypted Text (after table):\n%s\n", decrypted);

    grevityReaderFree(bufferp);
    free(decrypted);
}



grevity_long getFileSize(grevity_strg fname) {
    FILE* input;
    grevity_long flength;
    input = fopen(fname, "r");
    if (input == NULL) {
        errorPrint("%s%s", "Cannot open file: ", fname);
        return 0;
    }
    fseek(input, 0L, SEEK_END);
    flength = ftell(input);
    fclose(input);
    return flength;
}

// Implement other helper functions, changing all sofa_ to grevity_ similarly.

