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
* File name: Reader.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A12.
* Date: Jan 01 2025
* Professor: Paulo Sousa
* Purpose: This file is the main header for Reader (.h)
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * Please check the "TODO" labels to develop your activity.
 *.............................................................................
 */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef CODER_H_
#define CODER_H_

 // ASCII range for visible characters
#define ASCII_START 32
#define ASCII_END 126
#define ASCII_RANGE (ASCII_END - ASCII_START + 1)

#define CYPHER 1
#define DECYPHER 0

/* FUNCTIONS DECLARATION:  .................................. */

/* General Operations */
void vigenereFile(const grevity_strg inputFileName, const grevity_strg outputFileName, const grevity_strg key, grevity_intg encode);
grevity_strg vigenereMem(const grevity_strg inputFileName, const grevity_strg key, grevity_intg encode);

void cypher(const grevity_strg inputFileName, const grevity_strg outputFileName, const grevity_strg key);
void decypher(const grevity_strg inputFileName, const grevity_strg outputFileName, const grevity_strg key);
grevity_intg getSizeOfFile(const grevity_strg filename);

#endif
