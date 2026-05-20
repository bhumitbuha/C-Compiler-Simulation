#include <stdio.h>
#include <string.h>
#include "Step4Parser.h"

const char* GrevityBNFStrTable[NUM_BNF_RULES] = {
    "BNF_program",
    "BNF_include_stmt",
    "BNF_comment",
    "BNF_assignment",
    "BNF_output",
    "BNF_power_stmt",
    "BNF_sqrt_stmt",
    "BNF_if_stmt",
    "BNF_while_stmt",
    "BNF_expr_stmt",
    "BNF_id_list",
    "BNF_output_args",
    "BNF_expression",
    "BNF_term",
    "BNF_factor",
    "BNF_statements"
};

ParserData parserStats = {{0}};
int parserErrors = 0;
int gLineNumber = 1;
MSToken lookahead;



void printBNFData() {
    printf("Statistics:\n----------------------------------\n");
    for (int i = 0; i < NUM_BNF_RULES; i++) {
        if (parserStats.bnfHistogram[i] > 0)
            printf("Token[%s]=%d\n", GrevityBNFStrTable[i], parserStats.bnfHistogram[i]);
    }
    printf("----------------------------------\n");
}

void printError(const char* message) {
    printf("Grevity: Syntax error: %s at token '%s'\n", message, lookahead.lexeme);
    parserErrors++;
}

void syncErrorHandler(MSTokenCode code) {
    printError("Synchronizing");
    while (lookahead.code != code && lookahead.code != EOF_T)
        lookahead = msTokenizer();
    if (lookahead.code != EOF_T)
        lookahead = msTokenizer();
}

void matchToken(MSTokenCode expected, const char* kw) {
    int match = (lookahead.code == expected) &&
                ((kw == NULL) || (strcmp(lookahead.lexeme, kw) == 0));
    if (!match) {
        printError("Unexpected token");
        syncErrorHandler(expected);
        return;
    }
    lookahead = msTokenizer();
}

// --- Non-terminals ---

void comment() {
    parserStats.bnfHistogram[BNF_comment]++;
    matchToken(COMMENT_T, NULL);
    printf("Grevity: Comment parsed\n");
}

void assignment() {
    parserStats.bnfHistogram[BNF_assignment]++;
    matchToken(SET_T, NULL);
    id_list();
    matchToken(ASSIGN_T, NULL);
    // Accept string literal or full expression
    if (lookahead.code == STR_T) {
        matchToken(STR_T, NULL);
    } else {
        expression();
    }
    matchToken(SEMI_T, NULL);
    printf("Grevity: Assignment parsed\n");
}

void id_list() {
    parserStats.bnfHistogram[BNF_id_list]++;
    matchToken(ID_T, NULL);
    while (lookahead.code == COMMA_T) {
        matchToken(COMMA_T, NULL);
        matchToken(ID_T, NULL);
    }
    printf("Grevity: Id list parsed\n");
}

void power_stmt() {
    parserStats.bnfHistogram[BNF_power_stmt]++;
    matchToken(SET_T, NULL);
    matchToken(ID_T, NULL);
    matchToken(ASSIGN_T, NULL);
    matchToken(POWER_T, NULL);
    matchToken(OF_T, NULL);
    matchToken(INT_T, NULL);
    matchToken(BY_T, NULL);
    matchToken(INT_T, NULL);
    matchToken(SEMI_T, NULL);
    printf("Grevity: Power statement parsed\n");
}

void sqrt_stmt() {
    parserStats.bnfHistogram[BNF_sqrt_stmt]++;
    matchToken(SET_T, NULL);
    matchToken(ID_T, NULL);
    matchToken(ASSIGN_T, NULL);
    matchToken(SQRT_T, NULL);
    matchToken(OF_T, NULL);
    matchToken(INT_T, NULL);
    matchToken(SEMI_T, NULL);
    printf("Grevity: Sqrt statement parsed\n");
}

void output() {
    parserStats.bnfHistogram[BNF_output]++;
    matchToken(SHOW_T, NULL);
    output_args();
    matchToken(SEMI_T, NULL);
    printf("Grevity: Output statement parsed\n");
}

void output_args() {
    parserStats.bnfHistogram[BNF_output_args]++;
    int arg = 0;
    do {
        // Accept a quoted string, variable, number, or full expression
        if (lookahead.code == STR_T) {
            matchToken(STR_T, NULL);
        } else if (
            lookahead.code == ID_T ||
            lookahead.code == INT_T ||
            lookahead.code == LPAR_T ||
            lookahead.code == MINUS_T
        ) {
            expression();
        } else {
            // Defensive: attempt to parse as an expression for arithmetic output args
            expression();
        }

        if (lookahead.code == COMMA_T)
            matchToken(COMMA_T, NULL);
        else
            break;
        arg++;
    } while (1);
    printf("Grevity: Output args parsed\n");
}

void if_stmt() {
    parserStats.bnfHistogram[BNF_if_stmt]++;
    matchToken(IF_T, NULL);
    expression();
    matchToken(THEN_T, NULL);
    statements();
    if (lookahead.code == ELSE_T) {
        matchToken(ELSE_T, NULL);
        statements();
    }
    if (lookahead.code == ENDIF_T)
        matchToken(ENDIF_T, NULL);
    printf("Grevity: If statement parsed\n");
}

void while_stmt() {
    parserStats.bnfHistogram[BNF_while_stmt]++;
    matchToken(WHILE_T, NULL);
    expression();
    matchToken(DO_T, NULL);
    statements();
    if (lookahead.code == ENDWHILE_T)
        matchToken(ENDWHILE_T, NULL);
    printf("Grevity: While statement parsed\n");
}

void expr_stmt() {
    parserStats.bnfHistogram[BNF_expr_stmt]++;
    expression();
    matchToken(SEMI_T, NULL);
    printf("Grevity: Expression statement parsed\n");
}

void statements() {
    parserStats.bnfHistogram[BNF_statements]++;
    while (lookahead.code != ENDIF_T &&
           lookahead.code != ENDWHILE_T &&
           lookahead.code != EOF_T) {
        if (lookahead.code == COMMENT_T) comment();
        else if (lookahead.code == SET_T) assignment();
        else if (lookahead.code == SHOW_T) output();
        else if (lookahead.code == IF_T) if_stmt();
        else if (lookahead.code == WHILE_T) while_stmt();
        else {
            printError("Invalid statement start");
            syncErrorHandler(SEMI_T);
            if (lookahead.code == SEMI_T) matchToken(SEMI_T, NULL);
        }
    }
    printf("Grevity: Statements parsed\n");
}

void expression() {
    parserStats.bnfHistogram[BNF_expression]++;
    // Accept a string as a valid expression (for set ... = "str";)
    if (lookahead.code == STR_T) {
        matchToken(STR_T, NULL);
    } else {
        term();
        if (lookahead.code == GT_T || lookahead.code == LT_T ||
            lookahead.code == GE_T || lookahead.code == LE_T ||
            lookahead.code == EQ_T || lookahead.code == NE_T) {
            MSTokenCode relop = lookahead.code;
            matchToken(relop, NULL);
            term();
        }
        while (lookahead.code == PLUS_T || lookahead.code == MINUS_T) {
            matchToken(lookahead.code, NULL);
            term();
        }
    }
    printf("Grevity: Expression parsed\n");
}

void term() {
    parserStats.bnfHistogram[BNF_term]++;
    factor();
    while (lookahead.code == MUL_T || lookahead.code == DIV_T || lookahead.code == EXP_T) {
        matchToken(lookahead.code, NULL);
        factor();
    }
    printf("Grevity: Term parsed\n");
}

void factor() {
    parserStats.bnfHistogram[BNF_factor]++;
    if (lookahead.code == INT_T) {
        matchToken(INT_T, NULL);
        if (lookahead.code == UNIT_T)
            matchToken(UNIT_T, NULL);
    }
    else if (lookahead.code == ID_T) {
        matchToken(ID_T, NULL);
        if (lookahead.code == UNIT_T)
            matchToken(UNIT_T, NULL);
    }
    else if (lookahead.code == STR_T) {
        matchToken(STR_T, NULL); // Accept string literal as a factor!
    }
    else if (lookahead.code == LPAR_T) {
        matchToken(LPAR_T, NULL);
        expression();
        matchToken(RPAR_T, NULL);
    }
    else if (lookahead.code == MINUS_T) {
        matchToken(MINUS_T, NULL);
        factor();
    }
    else {
        printError("Invalid factor");
        syncErrorHandler(SEMI_T);
    }
    printf("Grevity: Factor parsed\n");
}

// --- MAIN ENTRY POINT ---
void program() {
    parserStats.bnfHistogram[BNF_program]++;
    while (lookahead.code != EOF_T) {
        if (lookahead.code == COMMENT_T) comment();
        else if (lookahead.code == SET_T) assignment();
        else if (lookahead.code == SHOW_T) output();
        else if (lookahead.code == IF_T) if_stmt();
        else if (lookahead.code == WHILE_T) while_stmt();
        else {
            printError("Invalid statement start");
            syncErrorHandler(SEMI_T);
            if (lookahead.code == SEMI_T) matchToken(SEMI_T, NULL);
        }
    }
    printf("Grevity: Program parsed\n");
}

void startParser() {
    lookahead = msTokenizer();
    program();
    if (lookahead.code != EOF_T)
        printError("Expected end of file");
    printf("Grevity: Source file parsed\n");
    printf("Number of Parser errors: %d\n", parserErrors);
    printBNFData();
}
