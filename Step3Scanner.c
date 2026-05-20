#include "Step3Scanner.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "Step2Reader.h"

int msTokenStatistics[MS_TOKEN_COUNT] = {0};
static BufferPointer ms_buf = NULL;

void msStartScanner(BufferPointer buf) {
    ms_buf = buf;
    for (int i = 0; i < MS_TOKEN_COUNT; ++i) msTokenStatistics[i] = 0;
}

static void ms_skip_ws() {
    grevity_char c;
    while ((c = grevityReaderGetChar(ms_buf)) != '\0' && isspace((unsigned char)c));
    if (c != '\0') grevityReaderRetract(ms_buf);
}

static MSToken ms_scan_id() {
    MSToken token;
    int i = 0;
    grevity_char c = grevityReaderGetChar(ms_buf);

    // Build the identifier string
    while (isalnum((unsigned char)c) || c == '_') {
        if (i < 63) token.lexeme[i++] = c;
        c = grevityReaderGetChar(ms_buf);
    }
    token.lexeme[i] = '\0';
    if (c != '\0') grevityReaderRetract(ms_buf);

    // Default token code is ID_T
    token.code = ID_T;

    // Keyword checks
    if (strcmp(token.lexeme, "set") == 0) token.code = SET_T;
    else if (strcmp(token.lexeme, "show") == 0) token.code = SHOW_T;
    else if (strcmp(token.lexeme, "power") == 0) token.code = POWER_T;
    else if (strcmp(token.lexeme, "sqrt") == 0) token.code = SQRT_T;
    else if (strcmp(token.lexeme, "of") == 0) token.code = OF_T;
    else if (strcmp(token.lexeme, "by") == 0) token.code = BY_T;
    else if (strcmp(token.lexeme, "if") == 0) token.code = IF_T;
    else if (strcmp(token.lexeme, "then") == 0) token.code = THEN_T;
    else if (strcmp(token.lexeme, "else") == 0) token.code = ELSE_T;
    else if (strcmp(token.lexeme, "endif") == 0) token.code = ENDIF_T;
    else if (strcmp(token.lexeme, "while") == 0) token.code = WHILE_T;
    else if (strcmp(token.lexeme, "do") == 0) token.code = DO_T;
    else if (strcmp(token.lexeme, "endwhile") == 0) token.code = ENDWHILE_T;
    // else -- leave as ID_T

    msTokenStatistics[token.code]++;
    return token;
}

static MSToken ms_scan_int() {
    MSToken token = {INT_T, ""};
    int i = 0;
    grevity_char c = grevityReaderGetChar(ms_buf);
    while (isdigit((unsigned char)c)) {
        if (i < 63) token.lexeme[i++] = c;
        c = grevityReaderGetChar(ms_buf);
    }
    token.lexeme[i] = '\0';
    if (c != '\0') grevityReaderRetract(ms_buf);
    msTokenStatistics[INT_T]++;
    return token;
}

static MSToken ms_scan_str() {
    MSToken token = {STR_T, ""};
    int i = 0;
    grevity_char c;
    while ((c = grevityReaderGetChar(ms_buf)) != '"' && c != '\0') {
        if (i < 63) token.lexeme[i++] = c;
    }
    token.lexeme[i] = '\0';
    msTokenStatistics[STR_T]++;
    return token;
}

static MSToken ms_scan_comment() {
    MSToken token = {COMMENT_T, ""};
    grevity_char c = grevityReaderGetChar(ms_buf);
    if (c == '*') { // block comment: @* ... *@
        int i = 0;
        while (1) {
            c = grevityReaderGetChar(ms_buf);
            if (c == '\0') break;
            if (c == '*' && grevityReaderGetChar(ms_buf) == '@')
                break;
            if (i < 63) token.lexeme[i++] = c;
        }
        token.lexeme[i] = '\0';
    } else { // line comment: @ ...
        int i = 0;
        while (c != '\n' && c != '\0') {
            if (i < 63) token.lexeme[i++] = c;
            c = grevityReaderGetChar(ms_buf);
        }
        token.lexeme[i] = '\0';
    }
    msTokenStatistics[COMMENT_T]++;
    return token;
}

MSToken msTokenizer() {
    ms_skip_ws();
    grevity_char c = grevityReaderGetChar(ms_buf);

    if (c == '\0' || c == EOF) {
        MSToken tok = {EOF_T, ""};
        msTokenStatistics[EOF_T]++;
        return tok;
    }

    // Units, e.g. $km, $myunit
    if (c == '$') {
        MSToken token = {UNIT_T, ""};
        int i = 0;
        grevity_char uc = grevityReaderGetChar(ms_buf);
        while (isalnum((unsigned char)uc) || uc == '_') {
            if (i < 63) token.lexeme[i++] = uc;
            uc = grevityReaderGetChar(ms_buf);
        }
        token.lexeme[i] = '\0';
        if (uc != '\0') grevityReaderRetract(ms_buf);
        msTokenStatistics[UNIT_T]++;
        return token;
    }

    // Comments
    if (c == '@') {
        return ms_scan_comment();
    }

    // Numbers
    if (isdigit((unsigned char)c)) {
        grevityReaderRetract(ms_buf);
        return ms_scan_int();
    }

    // Identifiers/keywords
    if (isalpha((unsigned char)c) || c == '_') {
        grevityReaderRetract(ms_buf);
        return ms_scan_id();
    }

    // String literals
    if (c == '"') {
        return ms_scan_str();
    }

    // Operators & delimiters
    MSToken token = {ERR_T, ""};
    token.lexeme[0] = c;
    token.lexeme[1] = '\0';

    switch (c) {
        case '=':
            if ((c = grevityReaderGetChar(ms_buf)) == '=') {
                token.code = EQ_T;
                strcpy(token.lexeme, "==");
            } else {
                grevityReaderRetract(ms_buf);
                token.code = ASSIGN_T;
            }
            msTokenStatistics[token.code]++;
            break;
        case '>':
            if ((c = grevityReaderGetChar(ms_buf)) == '=') {
                token.code = GE_T;
                strcpy(token.lexeme, ">=");
            } else {
                grevityReaderRetract(ms_buf);
                token.code = GT_T;
            }
            msTokenStatistics[token.code]++;
            break;
        case '<':
            if ((c = grevityReaderGetChar(ms_buf)) == '=') {
                token.code = LE_T;
                strcpy(token.lexeme, "<=");
            } else {
                grevityReaderRetract(ms_buf);
                token.code = LT_T;
            }
            msTokenStatistics[token.code]++;
            break;
        case '!':
            if ((c = grevityReaderGetChar(ms_buf)) == '=') {
                token.code = NE_T;
                strcpy(token.lexeme, "!=");
            } else {
                token.code = ERR_T;
            }
            msTokenStatistics[token.code]++;
            break;
        case ';':
            token.code = SEMI_T; msTokenStatistics[SEMI_T]++; break;
        case ',':
            token.code = COMMA_T; msTokenStatistics[COMMA_T]++; break;
        case '+':
            token.code = PLUS_T; msTokenStatistics[PLUS_T]++; break;
        case '-':
            token.code = MINUS_T; msTokenStatistics[MINUS_T]++; break;
        case '*':
            if ((c = grevityReaderGetChar(ms_buf)) == '*') {
                token.code = EXP_T;
                strcpy(token.lexeme, "**");
            } else {
                grevityReaderRetract(ms_buf);
                token.code = MUL_T;
            }
            msTokenStatistics[token.code]++;
            break;
        case '/':
            token.code = DIV_T; msTokenStatistics[DIV_T]++; break;
        case '(':
            token.code = LPAR_T; msTokenStatistics[LPAR_T]++; break;
        case ')':
            token.code = RPAR_T; msTokenStatistics[RPAR_T]++; break;
        case '{':
            token.code = LBRACE_T; msTokenStatistics[LBRACE_T]++; break;
        case '}':
            token.code = RBRACE_T; msTokenStatistics[RBRACE_T]++; break;
        default:
            token.code = ERR_T; msTokenStatistics[ERR_T]++; break;
    }
    return token;
}

void ms_printToken(MSToken token) {
    static const char* names[] = {
        "ERR_T","COMMENT_T","SET_T","SHOW_T","POWER_T","SQRT_T","OF_T","BY_T",
        "IF_T","THEN_T","ELSE_T","ENDIF_T","WHILE_T","DO_T","ENDWHILE_T",
        "INT_T","ID_T","STR_T",
        "ASSIGN_T","SEMI_T","COMMA_T","PLUS_T","MINUS_T","MUL_T","DIV_T","EXP_T",
        "LPAR_T","RPAR_T","LBRACE_T","RBRACE_T",
        "UNIT_T",
        "GT_T","LT_T","GE_T","LE_T","EQ_T","NE_T",
        "EOF_T"
    };
    if (token.code == COMMENT_T)
        printf("%-15s %s\n", names[token.code], token.lexeme);
    else if (token.code == ERR_T || token.code == EOF_T)
        printf("%-15s\n", names[token.code]);
    else
        printf("%-15s %s\n", names[token.code], token.lexeme);
}

void ms_printStats() {
    static const char* names[] = {
        "ERR_T","COMMENT_T","SET_T","SHOW_T","POWER_T","SQRT_T","OF_T","BY_T",
        "IF_T","THEN_T","ELSE_T","ENDIF_T","WHILE_T","DO_T","ENDWHILE_T",
        "INT_T","ID_T","STR_T",
        "ASSIGN_T","SEMI_T","COMMA_T","PLUS_T","MINUS_T","MUL_T","DIV_T","EXP_T",
        "LPAR_T","RPAR_T","LBRACE_T","RBRACE_T",
        "UNIT_T",
        "GT_T","LT_T","GE_T","LE_T","EQ_T","NE_T",
        "EOF_T"
    };
    printf("Statistics:\n----------------------------------\n");
    for (int i = 0; i < MS_TOKEN_COUNT; ++i)
        if (msTokenStatistics[i] > 0)
            printf("Token[%s]=%d\n", names[i], msTokenStatistics[i]);
}
