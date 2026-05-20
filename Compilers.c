/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Summer, 2025
* Author: TO_DO
* Professors: Paulo Sousa
************************************************************
*/

#include "Compilers.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
***********************************************************
* Function name: main
* Purpose: Main function and menu
*************************************************************
*/

grevity_intg main(int argc, char** argv) {

    grevity_intg i;
    printLogo();
    if (DEBUG) {
        for (i = 0; i < argc; ++i)
            printf("argv[%d] = %s\n", i, argv[i]);
    }
    if (argc < 2) {
        printf("%s%c%s%c%s%c%s%c%s%c%s", "OPTIONS:\n* [",
            PGM_CDR, "] - Coder\n* [",
            PGM_RDR, "] - Reader\n* [",
            PGM_SCN, "] - Scanner\n* [",
            PGM_PSR, "] - Parser\n* [",
            PGM_WRT, "] - Writer\n");
        return EXIT_FAILURE;
    }
    grevity_char option = argv[1][0];
    switch (option) {
        case PGM_CDR:
            printf("%s%c%s", "\n[Option '", PGM_CDR, "': Starting CODER .....]\n\n");
            main1Coder(argc, argv);
            break;

        case PGM_RDR:
            printf("%s%c%s", "\n[Option '", PGM_RDR, "': Starting READER .....]\n\n");
            main2Reader(argc, argv);
            break;

        case PGM_SCN:
            printf("%s%c%s", "\n[Option '", PGM_SCN, "': Starting SCANNER .....]\n\n");
            main3Scanner(argc, argv);
            break;

        case PGM_PSR:
            printf("%s%c%s", "\n[Option '", PGM_PSR, "': Starting PARSER .....]\n\n");
            main4Parser(argc, argv);   // <-- Call your parser main
            break;

        case PGM_WRT:
            printf("%s%c%s", "\n[Option '", PGM_WRT, "': Starting WRITER .....]\n\n");
            main5Writer(argc, argv);
            break;

        default:
            printf("%s%c%s%c%s%c%s%c%s%c%s", "OPTIONS:\n* [",
                PGM_CDR, "] - Coder\n* [",
                PGM_RDR, "] - Reader\n* [",
                PGM_SCN, "] - Scanner\n* [",
                PGM_PSR, "] - Parser\n* [",
                PGM_WRT, "] - Writer\n");
            break;
    }
    return EXIT_SUCCESS;
}

/*
***********************************************************
* Function name: printLogo
* Purpose: Print Logo
*************************************************************
*/

grevity_void printLogo() {
    printf("%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
        "\t=---------------------------------------=\n",
        "\t|  COMPILERS - ALGONQUIN COLLEGE (G25)  |\n",
        "\t=---------------------------------------=\n",
        "\t    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    \n",
        "\t    @@                             @@    \n",
        "\t    @@         @@@@@@@@@@@@@@      @@    \n",
        "\t    @@       @@             @@     @@    \n",
        "\t    @@      @@               @     @@    \n",
        "\t    @@     @@      @@@@@@@         @@    \n",
        "\t    @@     @@      @@              @@    \n",
        "\t    @@     @@      @@   @@@@@@     @@    \n",
        "\t    @@      @@           @@  @     @@    \n",
        "\t    @@       @@         @@  @@     @@    \n",
        "\t    @@        @@       @@   @      @@    \n",
        "\t    @@         @@@@@@@@     @      @@    \n",
        "\t    @@                             @@    \n",
        "\t    @@         G R E V I T Y       @@    \n",
        "\t    @@                             @@    \n",
        "\t    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    \n",
        "\t                                         \n",
        "\t[COMPILER SCRIPT .......................]\n",
        "\t                                         \n"
    );
}

/*
************************************************************
* Error printing function with variable number of arguments
************************************************************
*/

grevity_void errorPrint(grevity_strg fmt, ...) {
    va_list ap;
    va_start(ap, fmt);

    (grevity_void)vfprintf(stderr, fmt, ap);
    va_end(ap);

    /* Move to new line */
    if (strchr(fmt, '\n') == NULL)
        fprintf(stderr, "\n");
}
