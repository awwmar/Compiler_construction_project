
CS F363 – Compiler Project (Completed Reference)

This submission implements:
- Lexical Analysis (tokenization with preprocessing)
- Syntax Analysis (recursive-descent parser using LL(1)-compatible grammar)

Not implemented:
- Semantic analysis (type checking, scope resolution)
- Code generation or execution

How to run (C++):
  g++ -std=c++17 src/main.cpp src/lexer/lexer.cpp src/parser/parser.cpp -o compiler
  ./compiler testcases/valid/sample1.txt

Folders:
- docs/: report, grammar, DFA scans (placeholders provided)
- src/: lexer and parser source code
- testcases/: valid and invalid inputs
- handwritten/: handwritten DFA scans (placeholders)
