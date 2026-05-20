#ifndef STEP4PARSER_H_
#define STEP4PARSER_H_

#include "Compilers.h"
#include "Step2Reader.h"
#include "Step3Scanner.h" // For MSTokenCode & MSToken

// --- BNF Rule enumeration ---
enum BNF_RULES {
    BNF_program,
    BNF_include_stmt,
    BNF_comment,
    BNF_assignment,
    BNF_output,
    BNF_power_stmt,
    BNF_sqrt_stmt,
    BNF_if_stmt,
    BNF_while_stmt,
    BNF_expr_stmt,
    BNF_id_list,
    BNF_output_args,
    BNF_expression,
    BNF_term,
    BNF_factor,
    BNF_statements,
    NUM_BNF_RULES
};

// --- BNF rule names for output ---
extern const char* GrevityBNFStrTable[NUM_BNF_RULES];

typedef struct parserData {
    int bnfHistogram[NUM_BNF_RULES];
} ParserData;

// --- Externs for parser state ---
extern ParserData parserStats;
extern int parserErrors;
extern int gLineNumber;
extern MSToken lookahead;

// --- All function prototypes for parser non-terminals ---
void startParser();
void matchToken(MSTokenCode, const char*);
void syncErrorHandler(MSTokenCode);
void printError(const char* message);
void printBNFData();

void program();
void include_stmt(); // Optional, only if you actually implement it
void comment();
void assignment();
void output();
void power_stmt();
void sqrt_stmt();
void if_stmt();
void while_stmt();
void expr_stmt();
void id_list();
void output_args();
void expression();
void term();
void factor();
void statements();

#endif // STEP4PARSER_H_
