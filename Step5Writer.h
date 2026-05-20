#ifndef STEP5WRITER_H_
#define STEP5WRITER_H_

#define MAX_VARS 100
#define MAX_EXPR_LEN 256
#define MAX_LINES 100

#define EOS '\0'
#define STREMPTY ""
#define LPAR '('
#define RPAR ')'
#define QUOTES '"'
#define NEWLINE '\n'

typedef double grevity_doub;
typedef char grevity_char;
typedef char* grevity_strg;
typedef int grevity_intg;
typedef void grevity_void;

typedef enum { NUMERIC, STRING } VarType;

typedef struct {
    grevity_char name[32];
    VarType type;
    union {
        grevity_doub num_value;
        grevity_char str_value[256];
    } value;
} Variable;

grevity_intg find_variable(const grevity_strg name);
grevity_void assign_string_variable(const grevity_strg name, const grevity_strg value);
grevity_void assign_numeric_variable(const grevity_strg name, grevity_doub value);
const grevity_strg get_string_value(const grevity_strg name);
grevity_doub get_numeric_value(const grevity_strg name);

grevity_void handle_show(const grevity_strg line);
grevity_void calculate(const grevity_strg line);
grevity_void process_content(grevity_strg fileContent);
grevity_strg* splitIntoLines(const grevity_strg content, grevity_intg* lineCount);
grevity_void freeLines(grevity_strg* lines, grevity_intg lineCount);

#endif
