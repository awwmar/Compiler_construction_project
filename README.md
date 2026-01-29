---

# CS F363 – Compiler Construction Project

**Batch No: 38**

## 👥 Team Members

| Name            | ID            |
| --------------- | ------------- |
| Amar Kumar      | 2023A7PS0518P |
| Krishna Saraf   | 2023A7PS0606P |
| Ritik Kumar     | 2023A7PS0553P |
| Shivansh Saxena | 2023A7PS0637P |
| Hitashi Raizada | 2023A7PS0500P |
| Arnav Sahoo     | 2023A7PS0560P |

---

## 📌 Project Overview

This project implements the **front-end phases of a compiler**, focusing on:

### ✅ Implemented Components

1. **Lexical Analysis**

   * Tokenization of source code
   * Preprocessing support
   * DFA-based recognition of tokens

2. **Syntax Analysis**

   * Recursive-descent parser
   * Grammar modified to be **LL(1)-compatible**
   * Validates syntactic correctness of input programs

### ❌ Not Implemented

* Semantic analysis (type checking, scope resolution)
* Intermediate code generation
* Target code generation or execution

---

## 🛠️ How to Build and Run

### Compilation

```bash
g++ -std=c++17 src/main.cpp src/lexer/lexer.cpp src/parser/parser.cpp -o compiler
```

### Execution

```bash
./compiler testcases/valid/sample1.txt
```

---

## 📂 Project Structure

```
.
├── docs/                # Project report, grammar, DFA diagrams (scans/placeholders)
├── handwritten/         # Handwritten DFA diagrams (scanned copies)
├── src/
│   ├── lexer/           # Lexical analyzer implementation
│   ├── parser/          # Recursive-descent parser implementation
│   └── main.cpp         # Driver program
├── testcases/
│   ├── valid/           # Syntactically correct test programs
│   └── invalid/         # Programs with syntax errors
└── README.md
```

---

## ▶️ Program Flow

1. **Input Source Code** is read from a file
2. **Lexer** converts the character stream into tokens
3. **Parser** checks whether the token stream follows the defined grammar
4. Errors (if any) are reported with relevant details

---

## 📖 Notes

* The grammar was refactored to remove **left recursion** and apply **left factoring** to ensure compatibility with LL(1) parsing.
* DFA diagrams for token recognition are included in the documentation/handwritten sections as required by the project guidelines.

---

## 📚 Course Information

**Course:** CS F363 – Compiler Construction
**Institution:** BITS Pilani

---
