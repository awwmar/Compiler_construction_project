project:
  name: "CS F363 – Compiler Construction Project"
  batch: 38

  team_members:
    - name: "Amar Kumar"
      id: "2023A7PS0518P"
    - name: "Krishna Saraf"
      id: "2023A7PS0606P"
    - name: "Ritik Kumar"
      id: "2023A7PS0553P"
    - name: "Shivansh Saxena"
      id: "2023A7PS0637P"
    - name: "Hitashi Raizada"
      id: "2023A7PS0500P"
    - name: "Arnav Sahoo"
      id: "2023A7PS0560P"

  overview: |
    This project implements the front-end phases of a compiler.

    Implemented:
    - Lexical Analysis (tokenization with preprocessing)
    - Syntax Analysis (recursive-descent parser using LL(1)-compatible grammar)

    Not Implemented:
    - Semantic analysis (type checking, scope resolution)
    - Code generation or execution

  build_and_run: |
    Compilation (C):
      gcc -std=c17 src/main.c src/lexer/lexer.c src/parser/parser.c -o compiler

    Execution:
      ./compiler testcases/valid/sample1.txt

  folder_structure: |
    docs/         -> Report, grammar, DFA scans
    handwritten/  -> Handwritten DFA diagrams
    src/          -> Lexer, parser, and main driver code
    testcases/    -> Valid and invalid test programs
