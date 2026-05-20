# Grevity Compiler

A fully functional compiler pipeline for **Grevity**, a custom programming language, written in C. Built as part of the CST 8152 Compilers course at Algonquin College (Summer 2025).

The project covers all five classic stages of a compiler — from source encryption to code execution — implemented from scratch.

---

## What is Grevity?

Grevity is a simple, readable scripting language designed for this project. It supports variables, arithmetic, string output, conditionals, and loops.

```
@* Grevity Demo *@

set speed = 80;
set distance = 200;
set time = distance / speed;

show "Speed is", speed;
show "Time to travel:", time;

if time > 2 then
    show "Long trip!";
endif
```

---

## Compiler Pipeline

The compiler is split into five independent, runnable stages:

| Stage | Option | Description |
|-------|--------|-------------|
| Step 1 — Coder | `1` | Encrypts/decrypts source files using the **Vigenère cipher** |
| Step 2 — Reader | `2` | Loads source into a dynamic **character buffer** with position tracking and checksums |
| Step 3 — Scanner | `3` | **Tokenizes** the source — identifies keywords, identifiers, operators, literals |
| Step 4 — Parser | `4` | **Validates syntax** using a recursive-descent parser driven by the Grevity BNF grammar |
| Step 5 — Writer | `5` | **Interprets and executes** the source — evaluates expressions and prints output |

---

## Language Features

- **Variables** — `set x = 42;` / `set name = "Bhumit";`
- **Arithmetic** — `+`, `-`, `*`, `/`, `**` (exponentiation)
- **Output** — `show "Hello,", name;`
- **Conditionals** — `if / then / else / endif`
- **Loops** — `while / do / endwhile`
- **Comments** — `@ line comment` and `@* block comment *@`
- **Units** — `$km`, `$mph` (unit-tagged numeric literals)
- **Math builtins** — `power of X by Y`, `sqrt of X`

---

## Token Types (Scanner)

```
SET_T   SHOW_T   IF_T   THEN_T   ELSE_T   ENDIF_T
WHILE_T DO_T     ENDWHILE_T
INT_T   ID_T     STR_T  UNIT_T   COMMENT_T
ASSIGN_T  PLUS_T  MINUS_T  MUL_T  DIV_T  EXP_T
GT_T  LT_T  GE_T  LE_T  EQ_T  NE_T
LPAR_T  RPAR_T  LBRACE_T  RBRACE_T  SEMI_T  COMMA_T
EOF_T   ERR_T
```

---

## Build & Run

**Requirements:** CMake 3.10+, GCC or MSVC

```bash
# Clone and build
git clone https://github.com/bhumitbuha/C-Compiler-Simulation.git
cd C-Compiler-Simulation
mkdir build && cd build
cmake ..
cmake --build .
```

**Run a stage:**

```bash
# Encrypt a source file (Vigenère cipher)
./Compilers 1 Input.txt

# Load and display the buffer
./Compilers 2 Input.txt

# Scan (tokenize) the source
./Compilers 3 Input.txt

# Parse (validate syntax)
./Compilers 4 Input.txt

# Execute (interpret and print output)
./Compilers 5 Input.txt
```

---

## Project Structure

```
.
├── Compilers.c / .h       # Entry point and language type definitions
├── Step1Coder.c / .h      # Vigenère cipher encoder/decoder
├── Step2Reader.c / .h     # Dynamic character buffer
├── Step3Scanner.c / .h    # Lexical analyser (tokenizer)
├── Step4Parser.c / .h     # Recursive-descent parser
├── Step5Writer.c / .h     # Interpreter / executor
├── Main1Coder.c           # Driver for Step 1
├── Main2Reader.c          # Driver for Step 2
├── Main3Scanner.c         # Driver for Step 3
├── Main4Parser.c          # Driver for Step 4
├── Main5Writer.c          # Driver for Step 5
├── Input.txt              # Sample Grevity source file
├── CODED.txt              # Encrypted version of Input.txt
└── CMakeLists.txt         # CMake build configuration
```

---

## Key Technical Details

- **Coder:** Vigenère cipher over the printable ASCII range (32–126) with a secret key, operating character-by-character on the source file
- **Reader:** Self-resizing buffer (doubles on overflow), character histogram, checksum validation, retract/restore/mark cursor operations
- **Scanner:** Hand-written tokenizer with lookahead for two-character operators (`==`, `!=`, `>=`, `<=`, `**`)
- **Parser:** LL(1) recursive-descent parser with panic-mode error recovery syncing to the next semicolon
- **Writer:** Single-pass tree-walking interpreter that evaluates arithmetic expressions and resolves variables at runtime

---

## Author

**Bhumit Buha**  
Student — Algonquin College, Computer Systems Technology  
Course: CST 8152 Compilers | Professor: Paulo Sousa
