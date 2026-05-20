# Grevity Compiler

A fully functional compiler pipeline for **Grevity**, a custom programming language, written in C.
Built as part of the CST 8152 Compilers course at Algonquin College (Summer 2025).

The project covers all five classic stages of a compiler, from source encryption to code execution, implemented from scratch in C.

---

## Table of Contents

1. [What is Grevity?](#what-is-grevity)
2. [Compiler Pipeline](#compiler-pipeline)
3. [Language Features](#language-features)
4. [Build Instructions](#build-instructions)
5. [Running Each Step](#running-each-step)
   - [Step 1 - Coder](#step-1---coder-vigenere-cipher)
   - [Step 2 - Reader](#step-2---reader-buffer)
   - [Step 3 - Scanner](#step-3---scanner-tokenizer)
   - [Step 4 - Parser](#step-4---parser-syntax-validation)
   - [Step 5 - Writer](#step-5---writer-interpreter)
6. [Token Reference](#token-reference)
7. [Project Structure](#project-structure)
8. [Technical Details](#technical-details)
9. [Author](#author)

---

## What is Grevity?

Grevity is a simple, readable scripting language designed for this compiler project.
It supports variables, arithmetic, string output, conditionals, and loops.

**Sample source file (`Input.txt`):**

```
@* Grevity Demo Program *@

set speed = 80;
set distance = 200;
set time = distance / speed;

show "Speed is", speed;
show "Distance is", distance;
show "Time to travel:", time;

set name = "Bhumit";
set city = "Ottawa";
set year = 2025;

show name;
show "Hello,", name, "from", city, "!";

set a = 5;
set b = a * 2 + 7;
show "Sum of a and b is", a + b;

@* More arithmetic *@
set tempC = 25;
set tempF = tempC * 9 / 5 + 32;
show "Temperature in Fahrenheit:", tempF;

@* End of program *@
```

---

## Compiler Pipeline

```
Input.txt  -->  [Step 1: Coder]  -->  CODED.txt (encrypted)
                                           |
                                    [Step 2: Reader]  -->  Buffer + ASCII stats
                                           |
                                    [Step 3: Scanner]  -->  Token stream
                                           |
                                    [Step 4: Parser]  -->  Syntax validation + BNF stats
                                           |
                                    [Step 5: Writer]  -->  Program output + variable table
```

| Stage | Option | Description |
|-------|--------|-------------|
| Step 1: Coder   | `1` | Encrypts and decrypts source files using the Vigenere cipher |
| Step 2: Reader  | `2` | Loads source into a dynamic character buffer with position tracking and checksums |
| Step 3: Scanner | `3` | Tokenizes the source, identifying keywords, identifiers, operators, and literals |
| Step 4: Parser  | `4` | Validates syntax using a recursive-descent parser driven by the Grevity BNF grammar |
| Step 5: Writer  | `5` | Interprets and executes the source, evaluates expressions, and prints output |

---

## Language Features

- **Variables:** `set x = 42;` and `set name = "Bhumit";`
- **Arithmetic:** `+`, `-`, `*`, `/`, `**` (exponentiation)
- **Output:** `show "Hello,", name;`
- **Conditionals:** `if / then / else / endif`
- **Loops:** `while / do / endwhile`
- **Comments:** `@ line comment` and `@* block comment *@`
- **Units:** `$km`, `$mph` (unit-tagged numeric literals)
- **Math builtins:** `power of X by Y` and `sqrt of X`

---

## Build Instructions

**Requirements:** CMake 3.10+, GCC or MSVC

```bash
git clone https://github.com/bhumitbuha/C-Compiler-Simulation.git
cd C-Compiler-Simulation
mkdir build && cd build
cmake ..
cmake --build .
```

The compiled binary will be `Compilers.exe` (Windows) or `Compilers` (Linux/Mac).

---

## Running Each Step

All steps take an encoded `.txt` file as input (except Step 1 which also takes the output filename).
Steps 2 through 5 automatically decrypt the file internally before processing.

---

### Step 1 - Coder (Vigenere Cipher)

Encrypts a source file using the Vigenere cipher with the language name as the key.
After encoding, it immediately decodes the output file in memory to verify correctness.

**Command:**
```bash
./Compilers 1 1 Input.txt CODED.txt
```

**Output:**
```
        =---------------------------------------=
        |  COMPILERS - ALGONQUIN COLLEGE (G25)  |
        =---------------------------------------=
            @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            @@                             @@
            @@         G R E V I T Y       @@
            @@                             @@
            @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

        [COMPILER SCRIPT .......................]

[Option '1': Starting CODER .....]

Operation '1' completed successfully.
OUTPUT:
@* Grevity Demo Program *@

set speed = 80;
set distance = 200;
set time = distance / speed;

show "Speed is", speed;
show "Distance is", distance;
show "Time to travel:", time;

set name = "Bhumit";
set city = "Ottawa";
set year = 2025;

show name;
show "Hello,", name, "from", city, "!";

set a = 5;
set b = a * 2 + 7;
show "Sum of a and b is", a + b;

set tempC = 25;
set tempF = tempC * 9 / 5 + 32;
show "Temperature in Fahrenheit:", tempF;
```

The encrypted `CODED.txt` contains the ciphered version. Use `0` instead of `1` to decrypt:
```bash
./Compilers 1 0 CODED.txt Output.txt
```

---

### Step 2 - Reader (Buffer)

Decrypts the coded file and loads it into a dynamic character buffer.
Prints buffer metadata, a full ASCII character frequency histogram, and the decrypted content.

**Command:**
```bash
./Compilers 2 CODED.txt
```

**Output:**
```
[Option '2': Starting READER .....]

Printing buffer parameters:
Buffer Size: 512
Current Size: 468
First Symbol: @

Flags:
  isEmpty: 0
  isFull: 0
  isRead: 0
  isMoved: 0

Full ASCII Character Usage Table:
+-------+-----+-------+
| ASCII | CHR | COUNT |
+-------+-----+-------+
|    10 |  .  |    24 |
|    32 |     |    98 |
|    34 |  "  |    18 |
|    42 |  *  |     4 |
|    43 |  +  |     3 |
|    44 |  ,  |     8 |
|    47 |  /  |     3 |
|    59 |  ;  |    17 |
|    61 |  =  |    17 |
|    64 |  @  |     4 |
|    97 |  a  |    12 |
|    98 |  b  |     5 |
...
+-------+-----+-------+

Number of Errors: 0
Checksum: 143

Decrypted Text (after table):
@* Grevity Demo Program *@

set speed = 80;
set distance = 200;
set time = distance / speed;
...
```

---

### Step 3 - Scanner (Tokenizer)

Decrypts the source, loads it into the buffer, and produces a token stream.
Each token is printed with its type and lexeme. A frequency table is shown at the end.

**Command:**
```bash
./Compilers 3 CODED.txt
```

**Output:**
```
[Option '3': Starting SCANNER .....]

--- Deciphered Source Code ---
@* Grevity Demo Program *@

set speed = 80;
set distance = 200;
set time = distance / speed;
...

Token           Attribute
----------------------------------
COMMENT_T        Grevity Demo Program
SET_T           set
ID_T            speed
ASSIGN_T        =
INT_T           80
SEMI_T          ;
SET_T           set
ID_T            distance
ASSIGN_T        =
INT_T           200
SEMI_T          ;
SET_T           set
ID_T            time
ASSIGN_T        =
ID_T            distance
DIV_T           /
ID_T            speed
SEMI_T          ;
SHOW_T          show
STR_T           Speed is
COMMA_T         ,
ID_T            speed
SEMI_T          ;
SHOW_T          show
STR_T           Distance is
COMMA_T         ,
ID_T            distance
SEMI_T          ;
SHOW_T          show
STR_T           Time to travel:
COMMA_T         ,
ID_T            time
SEMI_T          ;
SET_T           set
ID_T            name
ASSIGN_T        =
STR_T           Bhumit
SEMI_T          ;
...
EOF_T

Statistics:
----------------------------------
Token[SET_T]=9
Token[SHOW_T]=8
Token[ID_T]=28
Token[INT_T]=7
Token[STR_T]=14
Token[ASSIGN_T]=9
Token[SEMI_T]=17
Token[COMMA_T]=8
Token[DIV_T]=2
Token[MUL_T]=2
Token[PLUS_T]=3
Token[COMMENT_T]=3
```

---

### Step 4 - Parser (Syntax Validation)

Decodes the file, loads it into the buffer, runs the scanner, and validates the full syntax
using a recursive-descent parser. Each grammar rule match is reported as it completes.

**Command:**
```bash
./Compilers 4 CODED.txt
```

**Output:**
```
        =---------------------------------------=
        |  COMPILERS - ALGONQUIN COLLEGE (G25)  |
        =---------------------------------------=
            @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            @@         G R E V I T Y       @@
            @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

        [PARSER SCRIPT .......................]

Decoding file CODED.txt with key 'Grevity' ...
Printing input buffer parameters:
The capacity of the buffer is:  470
The current size of the buffer is:  469

Printing input buffer contents:

@* Grevity Demo Program *@

set speed = 80;
...

Parsing the source file...

Grevity: Comment parsed
Grevity: Factor parsed
Grevity: Term parsed
Grevity: Expression parsed
Grevity: Id list parsed
Grevity: Assignment parsed
Grevity: Factor parsed
Grevity: Term parsed
Grevity: Expression parsed
Grevity: Id list parsed
Grevity: Assignment parsed
Grevity: Factor parsed
Grevity: Term parsed
Grevity: Factor parsed
Grevity: Term parsed
Grevity: Expression parsed
Grevity: Id list parsed
Grevity: Assignment parsed
Grevity: Factor parsed
Grevity: Term parsed
Grevity: Expression parsed
Grevity: Output args parsed
Grevity: Output statement parsed
...
Grevity: Program parsed
Grevity: Source file parsed
Number of Parser errors: 0

Statistics:
----------------------------------
Token[BNF_program]=1
Token[BNF_comment]=3
Token[BNF_assignment]=9
Token[BNF_output]=8
Token[BNF_id_list]=9
Token[BNF_output_args]=8
Token[BNF_expression]=34
Token[BNF_term]=34
Token[BNF_factor]=27
Token[BNF_statements]=0
----------------------------------
```

---

### Step 5 - Writer (Interpreter)

Decrypts and executes the Grevity source file.
Evaluates all `set` assignments and `show` output statements, then prints a final variable table.

**Command:**
```bash
./Compilers 5 CODED.txt
```

**Output:**
```
Reading file CODED.txt ....Please wait

Speed is 80.00
Distance is 200.00
Time to travel: 2.50
Bhumit
Hello, Bhumit from Ottawa !
Sum of a and b is 22.00
Temperature in Fahrenheit: 77.00

Variable values:
speed = 80.00
distance = 200.00
time = 2.50
name = "Bhumit"
city = "Ottawa"
year = 2025.00
a = 5.00
b = 17.00
tempC = 25.00
tempF = 77.00
```

---

## Token Reference

| Token | Lexeme | Description |
|-------|--------|-------------|
| `SET_T` | `set` | Variable assignment keyword |
| `SHOW_T` | `show` | Output keyword |
| `IF_T` / `THEN_T` / `ELSE_T` / `ENDIF_T` | `if then else endif` | Conditional |
| `WHILE_T` / `DO_T` / `ENDWHILE_T` | `while do endwhile` | Loop |
| `POWER_T` / `SQRT_T` / `OF_T` / `BY_T` | `power sqrt of by` | Math builtins |
| `ID_T` | e.g. `speed` | Identifier |
| `INT_T` | e.g. `80` | Integer literal |
| `STR_T` | e.g. `"Hello"` | String literal |
| `UNIT_T` | e.g. `$km` | Unit-tagged literal |
| `COMMENT_T` | `@ ...` or `@* ... *@` | Line or block comment |
| `ASSIGN_T` | `=` | Assignment operator |
| `PLUS_T` / `MINUS_T` / `MUL_T` / `DIV_T` / `EXP_T` | `+ - * / **` | Arithmetic operators |
| `GT_T` / `LT_T` / `GE_T` / `LE_T` / `EQ_T` / `NE_T` | `> < >= <= == !=` | Relational operators |
| `LPAR_T` / `RPAR_T` | `( )` | Parentheses |
| `LBRACE_T` / `RBRACE_T` | `{ }` | Braces |
| `SEMI_T` | `;` | Statement terminator |
| `COMMA_T` | `,` | Argument separator |
| `EOF_T` | end of file | End of token stream |
| `ERR_T` | unrecognized | Lexical error |

---

## Project Structure

```
C-Compiler-Simulation/
|
|-- Compilers.c          Entry point, menu, logo, error printer
|-- Compilers.h          Shared type definitions and token types
|
|-- Step1Coder.c/h       Vigenere cipher (encode and decode)
|-- Step2Reader.c/h      Dynamic character buffer
|-- Step3Scanner.c/h     Lexical analyser (tokenizer)
|-- Step4Parser.c/h      Recursive-descent parser
|-- Step5Writer.c/h      Interpreter and expression evaluator
|
|-- Main1Coder.c         Driver for Step 1
|-- Main2Reader.c        Driver for Step 2
|-- Main3Scanner.c       Driver for Step 3
|-- Main4Parser.c        Driver for Step 4
|-- Main5Writer.c        Driver for Step 5
|
|-- Input.txt            Sample Grevity source file (plaintext)
|-- CODED.txt            Encrypted version of Input.txt
|-- BNF.txt              Grevity grammar (BNF rules)
|-- CMakeLists.txt       CMake build configuration
```

---

## Technical Details

- **Coder:** Vigenere cipher over the printable ASCII range (32 to 126) using the language name as the key. Operates character-by-character on the source file, supporting both encode and decode modes.
- **Reader:** Self-resizing buffer that doubles capacity on overflow. Tracks read, write, and mark cursor positions. Maintains a per-character frequency histogram and running checksum.
- **Scanner:** Hand-written tokenizer with one-character lookahead to distinguish two-character operators (`==`, `!=`, `>=`, `<=`, `**`). Tracks token frequency statistics.
- **Parser:** LL(1) recursive-descent parser with panic-mode error recovery that synchronizes to the next semicolon on a syntax error. Tracks BNF rule hit counts.
- **Writer:** Single-pass interpreter that evaluates arithmetic expressions left-to-right, resolves variable references at runtime, and maintains a typed variable table (numeric and string).

---

## Author

**Bhumit Buha**
Student, Algonquin College - Computer Systems Technology
Course: CST 8152 Compilers | Professor: Paulo Sousa
