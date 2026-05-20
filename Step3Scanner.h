#ifndef STEP3SCANNER_H_
#define STEP3SCANNER_H_
#define MS_TOKEN_COUNT 38

#include "Compilers.h"

// Token codes for MathScripto / Grevity
typedef enum {
    ERR_T = 0,     // Error token
    COMMENT_T,     // Comment
    SET_T,         // set
    SHOW_T,        // show
    POWER_T,       // power
    SQRT_T,        // sqrt
    OF_T,          // of
    BY_T,          // by
    IF_T,          // if
    THEN_T,        // then
    ELSE_T,        // else
    ENDIF_T,       // endif
    WHILE_T,       // while
    DO_T,          // do
    ENDWHILE_T,    // endwhile
    INT_T,         // integer literal
    ID_T,          // identifier
    STR_T,         // string literal
    ASSIGN_T,      // =
    SEMI_T,        // ;
    COMMA_T,       // ,
    PLUS_T,        // +
    MINUS_T,       // -
    MUL_T,         // *
    DIV_T,         // /
    EXP_T,         // **
    LPAR_T,        // (
    RPAR_T,        // )
    LBRACE_T,      // {
    RBRACE_T,      // }
    UNIT_T,        // $unit
    GT_T,          // >
    LT_T,          // <
    GE_T,          // >=
    LE_T,          // <=
    EQ_T,          // ==
    NE_T,          // !=
    EOF_T          // End of file/input
} MSTokenCode;

typedef struct {
    MSTokenCode code;
    char lexeme[64];
} MSToken;

// For token statistics
extern int msTokenStatistics[MS_TOKEN_COUNT];

// Public interface
void msStartScanner(BufferPointer buf);
MSToken msTokenizer();
void ms_printToken(MSToken token);
void ms_printStats();

#endif // STEP3SCANNER_H_
