#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Step5Writer.h"

Variable variables[MAX_VARS];
grevity_intg var_count = 0;
grevity_intg initial_phase = 1;
grevity_char output_buffer[MAX_EXPR_LEN * 10] = { 0 };

// --- Trimming Helper ---
void trim(char *str) {
    char *start = str;
    while (isspace((unsigned char)*start)) start++;
    if (start != str) memmove(str, start, strlen(start) + 1);
    char *end = str + strlen(str) - 1;
    while (end >= str && isspace((unsigned char)*end)) *end-- = 0;
}

// --- Variable helpers ---
grevity_intg find_variable(const grevity_strg name) {
    for (grevity_intg i = 0; i < var_count; ++i)
        if (strcmp(variables[i].name, name) == 0) return i;
    return -1;
}

grevity_void assign_numeric_variable(const grevity_strg name, grevity_doub value) {
    char temp[32]; strncpy(temp, name, 31); temp[31]=0; trim(temp);
    grevity_intg idx = find_variable(temp);
    if (idx == -1) idx = var_count++;
    strncpy(variables[idx].name, temp, sizeof(variables[idx].name)-1);
    variables[idx].name[sizeof(variables[idx].name)-1] = EOS;
    variables[idx].type = NUMERIC;
    variables[idx].value.num_value = value;
}

grevity_void assign_string_variable(const grevity_strg name, const grevity_strg value) {
    char temp[32]; strncpy(temp, name, 31); temp[31]=0; trim(temp);
    grevity_intg idx = find_variable(temp);
    if (idx == -1) idx = var_count++;
    strncpy(variables[idx].name, temp, sizeof(variables[idx].name)-1);
    variables[idx].name[sizeof(variables[idx].name)-1] = EOS;
    variables[idx].type = STRING;
    strncpy(variables[idx].value.str_value, value, sizeof(variables[idx].value.str_value)-1);
    variables[idx].value.str_value[sizeof(variables[idx].value.str_value)-1] = EOS;
}

const grevity_strg get_string_value(const grevity_strg name) {
    char temp[32]; strncpy(temp, name, 31); temp[31]=0; trim(temp);
    grevity_intg idx = find_variable(temp);
    if (idx != -1 && variables[idx].type == STRING) return variables[idx].value.str_value;
    return STREMPTY;
}
grevity_doub get_numeric_value(const grevity_strg name) {
    char temp[32]; strncpy(temp, name, 31); temp[31]=0; trim(temp);
    grevity_intg idx = find_variable(temp);
    if (idx != -1 && variables[idx].type == NUMERIC) return variables[idx].value.num_value;
    return 0.0;
}

// --- Expression evaluation (supports +, -, *, /, no parens) ---
grevity_doub eval_expr(const grevity_strg expr_raw) {
    char expr[MAX_EXPR_LEN];
    strncpy(expr, expr_raw, MAX_EXPR_LEN-1); expr[MAX_EXPR_LEN-1]=0;
    char* p = expr;
    grevity_doub result = 0;
    char op = '+'; // current operation
    int first = 1;
    while (*p) {
        while (*p == ' ') p++;
        grevity_doub val = 0;
        if (isdigit(*p) || (*p == '-' && isdigit(*(p+1)))) {
            char* endptr;
            val = strtod(p, &endptr);
            p = endptr;
        } else if (isalpha(*p)) {
            char name[32] = {0};
            int i=0;
            while (isalnum(*p) && i<31) name[i++]=*p++;
            name[i]=0; trim(name);
            val = get_numeric_value(name);
        }
        if (first) { result = val; first = 0; }
        else if (op=='+') result += val;
        else if (op=='-') result -= val;
        else if (op=='*') result *= val;
        else if (op=='/') result /= val;
        while (*p && isspace(*p)) p++;
        if (*p=='+'||*p=='-'||*p=='*'||*p=='/') op=*p++;
        else break;
    }
    return result;
}

// --- Show (output) ---
grevity_void handle_show(const grevity_strg line) {
    grevity_char buffer[MAX_EXPR_LEN] = { 0 };
    int started = 0;
    const grevity_char* p = strstr(line, "show");
    if (p) p += 4;
    while (*p && (isspace(*p) || *p == ',')) p++;

    while (*p && *p != ';') {
        while (*p && (isspace(*p) || *p == ',')) p++;
        if (*p == ';' || *p == EOS) break;

        // Quoted string argument
        if (*p == QUOTES) {
            p++;
            if (started && *p) strcat(buffer, " ");
            while (*p && *p != QUOTES) {
                strncat(buffer, p, 1); p++;
            }
            if (*p == QUOTES) p++;
            started = 1;
        }
        // Variable or expression
        else {
            char expr[MAX_EXPR_LEN] = {0}; int i = 0;
            while (*p && *p != ',' && *p != ';') expr[i++] = *p++;
            expr[i] = 0;
            trim(expr);

            if (*expr) {
                if (started) strcat(buffer, " ");
                if (strpbrk(expr, "+-*/")) {
                    double val = eval_expr(expr);
                    char temp[64]; sprintf(temp, "%.2lf", val);
                    strcat(buffer, temp);
                } else if (isalpha(*expr)) {
                    int idx = find_variable(expr);
                    if (idx != -1) {
                        if (variables[idx].type == NUMERIC) {
                            char temp[64]; sprintf(temp, "%.2lf", variables[idx].value.num_value);
                            strcat(buffer, temp);
                        } else if (variables[idx].type == STRING) {
                            strcat(buffer, variables[idx].value.str_value);
                        }
                    }
                } else if (isdigit(*expr) || *expr == '-') {
                    double val = eval_expr(expr);
                    char temp[64]; sprintf(temp, "%.2lf", val);
                    strcat(buffer, temp);
                }
                started = 1;
            }
        }
    }

    if (initial_phase) {
        strcat(output_buffer, buffer);
        strcat(output_buffer, "\n");
    } else {
        printf("%s\n", buffer);
    }
}

// --- Main calculator ---
grevity_void calculate(const grevity_strg line) {
    grevity_char tmp[MAX_EXPR_LEN]; strcpy(tmp,line);
    if (strncmp(tmp,"set",3)==0) {
        char var[32]={0}, rhs[MAX_EXPR_LEN]={0};
        grevity_intg n = sscanf(tmp,"set %31[^=]= %[^\n;]",var,rhs);
        if (n==2) {
            trim(var);
            char* val=rhs; while(isspace(*val))val++;
            if (*val==QUOTES) {
                val++; char str_val[256]={0}; int i=0;
                while (*val && *val!=QUOTES && i<255) str_val[i++]=*val++;
                str_val[i]=0;
                assign_string_variable(var, str_val);
                if (!initial_phase) printf("%s = \"%s\"\n", var, str_val);
            } else {
                double num = eval_expr(val);
                assign_numeric_variable(var, num);
                if (!initial_phase) printf("%s = %.2lf\n", var, num);
            }
        }
    }
    else if (strncmp(tmp,"show",4)==0) {
        handle_show(tmp);
    }
}

// --- Content processing ---
grevity_void process_content(grevity_strg fileContent) {
    grevity_intg lineCount = 0;
    grevity_strg* lines = splitIntoLines(fileContent, &lineCount);
    if (!lines) return;
    for (grevity_intg i = 0; i < lineCount; i++) {
        if (lines[i][0]=='@' || lines[i][0]==EOS) continue; // Skip comments/blank
        calculate(lines[i]);
    }
    initial_phase = 0;
    printf("%s", output_buffer);
    printf("\nVariable values:\n");
    for (grevity_intg i = 0; i < var_count; i++) {
        if (variables[i].type == STRING)
            printf("%s = \"%s\"\n", variables[i].name, variables[i].value.str_value);
        else if (variables[i].type == NUMERIC)
            printf("%s = %.2lf\n", variables[i].name, variables[i].value.num_value);
    }
    freeLines(lines, lineCount);
}

// --- Line splitting and freeing ---
grevity_strg* splitIntoLines(const grevity_strg content, grevity_intg* lineCount) {
    grevity_strg* lines = malloc(MAX_LINES * sizeof(char*));
    if (!lines) return NULL;
    *lineCount = 0;
    const grevity_char* start = content;
    const grevity_char* end;
    while ((end = strchr(start, NEWLINE)) != NULL) {
        if (*lineCount >= MAX_LINES) break;
        grevity_intg lineLength = (grevity_intg) (end - start);
        lines[*lineCount] = malloc(lineLength + 1);
        strncpy(lines[*lineCount], start, lineLength);
        lines[*lineCount][lineLength] = EOS;
        (*lineCount)++;
        start = end + 1;
    }
    if (*start != EOS && *lineCount < MAX_LINES) {
        size_t lineLength = strlen(start);
        lines[*lineCount] = malloc(lineLength + 1);
        strncpy(lines[*lineCount], start, lineLength);
        lines[*lineCount][lineLength] = EOS;
        (*lineCount)++;
    }
    return lines;
}
grevity_void freeLines(grevity_strg* lines, grevity_intg lineCount) {
    for (grevity_intg i = 0; i < lineCount; i++)
        free(lines[i]);
    free(lines);
}
