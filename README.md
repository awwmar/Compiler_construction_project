# CS F363 – Compiler Construction Project

**Batch:** 38  
**Project Scope:** Front-End Compiler Development

---

## 👥 Team Members

| Name | ID Number |
| :--- | :--- |
| **Amar Kumar** | 2023A7PS0518P |
| **Krishna Saraf** | 2023A7PS0606P |
| **Ritik Kumar** | 2023A7PS0553P |
| **Shivansh Saxena** | 2023A7PS0637P |
| **Hitashi Raizada** | 2023A7PS0500P |
| **Arnav Sahoo** | 2023A7PS0560P |

---

## 📖 Overview

This project implements the essential **front-end phases** of a compiler. The system processes source code to ensure it is lexically valid and syntactically correct according to defined grammar rules.

### ✅ Implemented Features
* **Lexical Analysis:** Tokenization engine with integrated preprocessing (handling comments, whitespace, etc.).
* **Syntax Analysis:** A robust recursive-descent parser based on an **LL(1)-compatible grammar**.



### 🚧 Out of Scope
* **Semantic Analysis:** (Type checking, scope resolution).
* **Back-End:** (Intermediate code generation, optimization, or target code execution).

---

## 📂 Folder Structure

| Folder | Contents |
| :--- | :--- |
| `docs/` | Formal report, grammar specifications, and DFA scans. |
| `handwritten/` | Manual DFA diagrams and logic sketches. |
| `src/` | Implementation files (`lexer.c`, `parser.c`, `main.c`). |
| `testcases/` | Collection of valid and invalid test programs for validation. |

---

## 🛠️ Build and Run

Follow these instructions to compile and execute the compiler from your terminal.

### 1. Compilation
The project uses the **C17** standard. Compile all modules using `gcc`:
```bash
gcc -std=c17 src/main.c src/lexer/lexer.c src/parser/parser.c -o compiler
