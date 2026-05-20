#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Compilers.h"
#include "Step1Coder.h"       // For decypher/vigenereMem
#include "Step2Reader.h"
#include "Step3Scanner.h"
#include "Step4Parser.h"

#define DECODE_KEY STR_LANGNAME   // Use your language name as cipher key ("GREVITY")

int main4Parser(int argc, char* argv[]) {
    // Defensive: Require [program] 4 [CodedFile]
    if (argc < 3) {
        printf("Usage: %s 4 <coded_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    printf("        =---------------------------------------=\n");
    printf("        |  COMPILERS - ALGONQUIN COLLEGE (G25)  |\n");
    printf("        =---------------------------------------=\n");
    printf("            @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    \n");
    printf("            @@         G R E V I T Y       @@    \n");
    printf("            @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    \n\n");
    printf("        [PARSER SCRIPT .......................]\n\n");

    char* codedFile = argv[2];
    char* decodeKey = DECODE_KEY;

    // --- DECODE STEP ---
    printf("Decoding file %s with key '%s' ...\n", codedFile, decodeKey);
    char* decodedSource = vigenereMem(codedFile, decodeKey, DECYPHER); // Returns heap-allocated string

    if (!decodedSource) {
        printf("ERROR: Failed to decode file!\n");
        return EXIT_FAILURE;
    }

    // --- LOAD DECODED SOURCE INTO READER BUFFER ---
    int decodedLen = strlen(decodedSource);
    BufferPointer sourceBuffer = grevityReaderCreate(decodedLen + 2); // +2 for safety/EOF
    if (!sourceBuffer) {
        printf("ERROR: Failed to create buffer!\n");
        free(decodedSource);
        return EXIT_FAILURE;
    }
    for (int i = 0; i < decodedLen; ++i)
        grevityReaderAddChar(sourceBuffer, decodedSource[i]);
    grevityReaderAddChar(sourceBuffer, '\0'); // Null-terminate for safety

    // --- SHOW BUFFER INFO ---
    printf("Printing input buffer parameters:\n");
    printf("The capacity of the buffer is:  %d\n", grevityReaderGetSize(sourceBuffer));
    printf("The current size of the buffer is:  %d\n", grevityReaderGetPosWrte(sourceBuffer));
    printf("\nPrinting input buffer contents:\n\n");
    grevityReaderRecover(sourceBuffer);
    grevityReaderPrint(sourceBuffer);

    // --- START SCANNER/PARSER ---
    printf("\nParsing the source file...\n\n");
    msStartScanner(sourceBuffer);

    startParser();

    // --- CLEANUP ---
    free(decodedSource);
    grevityReaderFree(sourceBuffer);

    return EXIT_SUCCESS;
}
