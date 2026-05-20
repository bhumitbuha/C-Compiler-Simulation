/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Summer, 2025
* Author: TO_DO
* Professors: Paulo Sousa
************************************************************
#
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
************************************************************
* File name: Compilers.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 � Compilers, Lab Section: [011, 012]
* Assignment: A1-A5.
* Date: Jan 01 2025
* Professor: Paulo Sousa
* Purpose: This file defines the functions called by main function.
* Function list: mainReader(), mainScanner(), mainParser().
*************************************************************/

#ifndef COMPILERS_H_
#define COMPILERS_H_

#define DEBUG 0

/* Language name */
#define STR_LANGNAME    "Grevity"

/* Logical constants - adapt for your language */
#define GREVITY_TRUE  1
#define GREVITY_FALSE 0

#define INVALID NULL
#define EOF_CHAR '\0'

/*
------------------------------------------------------------
Data types definitions
------------------------------------------------------------
*/
typedef char            grevity_char;
typedef char*           grevity_strg;
typedef int             grevity_intg;
typedef float           grevity_real;
typedef void            grevity_void;

typedef unsigned char   grevity_boln;
typedef unsigned char   grevity_byte;

typedef long            grevity_long;
typedef double          grevity_doub;

/*
------------------------------------------------------------
Programs:
1: Coder
2: Reader
3: Scanner
4: Parser
5: Writer
------------------------------------------------------------
*/
enum PROGRAMS {
	PGM_CDR = '1', // Coder
	PGM_RDR = '2', // Reader
	PGM_SCN = '3', // Scanner
	PGM_PSR = '4', // Parser
	PGM_WRT = '5', // Writer
};

/*
------------------------------------------------------------
Main functions signatures
------------------------------------------------------------
*/
grevity_intg main1Coder(grevity_intg argc, grevity_strg* argv);
grevity_intg main2Reader(grevity_intg argc, grevity_strg* argv);
grevity_intg main3Scanner(grevity_intg argc, const grevity_strg* argv);
grevity_intg main4Parser(grevity_intg argc, grevity_strg* argv);
grevity_intg main5Writer(grevity_intg argc, grevity_strg* argv);

grevity_void printLogo();
grevity_void errorPrint(grevity_strg fmt, ...);

/*
------------------------------------------------------------
Token types and attributes for Scanner (Grevity language)
------------------------------------------------------------
*/

#define GRV_ID_LEN   32
#define GRV_STR_LEN  128
#define GRV_KW_SIZE  8 // Update if you have more/less keywords

// Forward declaration for your buffer/reader
typedef struct Buffer* BufferPointer;

// Token Types (use your language names)
typedef enum {
	GRV_ERR_T,
	GRV_ID_T,
	GRV_INT_T,
	GRV_STR_T,
	GRV_KW_T,
	GRV_ASSIGN_T,
	GRV_PLUS_T,
	GRV_MINUS_T,
	GRV_MUL_T,
	GRV_DIV_T,
	GRV_BANG_T,
	GRV_SEMI_T,
	GRV_LPAR_T,
	GRV_RPAR_T,
	GRV_LBR_T,
	GRV_RBR_T,
	GRV_COMMENT_T,
	GRV_EOS_T,      // End-of-statement ;
	GRV_SEOF_T,     // Source EOF
	GRV_MNID_T      // main id (like main&)
} GRVTokenCode;

// Token Attribute
typedef struct {
	char idLexeme[GRV_ID_LEN+1];
	int intValue;
	char strLexeme[GRV_STR_LEN+1];
	int keywordIndex;
	char errLexeme[GRV_STR_LEN+1];
} GRVTokenAttribute;

// Token Structure
typedef struct {
	GRVTokenCode code;
	GRVTokenAttribute attribute;
} GRVToken;


#endif // COMPILERS_H_
