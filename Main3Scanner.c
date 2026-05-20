#include "Compilers.h"
#include "Step1Coder.h"
#include "Step2Reader.h"
#include "Step3Scanner.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VIGENERE_KEY STR_LANGNAME // Use your language name

grevity_intg main3Scanner(grevity_intg argc, const grevity_strg* argv) {
    if (argc < 3) {
        printf("Usage: %s 3 <cyphered_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Defensive: Check argument
    grevity_strg cypheredFile = argv[2];
    if (!cypheredFile || strlen(cypheredFile) == 0) {
        printf("Error: Invalid input file path.\n");
        return EXIT_FAILURE;
    }

    // Step 1: Decode (decypher) the file using your language name as a key
    grevity_strg plainText = vigenereMem(cypheredFile, VIGENERE_KEY, 0); // 0 = decode
    if (!plainText) {
        printf("Failed to decypher file. Please check your input file and key.\n");
        return EXIT_FAILURE;
    }

    printf("--- Deciphered Source Code ---\n%s\n\n", plainText);

    // Step 2: Create a buffer and load the plainText into it
    BufferPointer sourceBuffer = grevityReaderCreate(2048);
    if (!sourceBuffer) {
        printf("Error: Could not allocate source buffer.\n");
        free(plainText);
        return EXIT_FAILURE;
    }
    for (int i = 0; plainText[i] != '\0'; ++i)
        grevityReaderAddChar(sourceBuffer, plainText[i]);
    grevityReaderAddChar(sourceBuffer, '\0'); // null-terminate for safety
    free(plainText);

    // Step 3: Start scanner
    msStartScanner(sourceBuffer);

    printf("[Option '3': Starting SCANNER .....]\n\n");
    printf("Token           Attribute\n----------------------------------\n");

    // Step 4: Tokenize all inputs
    MSToken token;
    do {
        token = msTokenizer();
        ms_printToken(token);
    } while (token.code != EOF_T);

    // Step 5: Print statistics if needed
    ms_printStats();
    grevityReaderFree(sourceBuffer);
    return EXIT_SUCCESS;
}
