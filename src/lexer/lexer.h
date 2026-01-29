
#ifndef LEXER_H
#define LEXER_H

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "token.h"

// GitHub-style structures
// Token is already defined in token.h
typedef struct Node {
    Token* t;
} Node;

typedef struct Head {
    // Head structure for parse tree
} Head;

// Global state variables for the lexer (GitHub style)
extern int state;
extern int line;
extern int begin;
extern int fwd;
extern int fwd2;
extern int keyno;
extern int tokno;
extern char* KeywordTable[1009];
extern char keyword[31][35];
extern char buffer[10000];
extern int buflen;

// Our simplified lexer variables
extern char* lexer_source;     // The source code we're tokenizing
extern size_t lexer_position;  // Current position in the source

// Function declarations (GitHub style)
Token* newToken();
Node* newNode();
Head* newHead();
void ClearMem(char* c, int len);
bool isKeyword(char value[30]);

// File handling functions (GitHub style)
FILE* getStream(FILE* fp);
void removeComments(char* testcaseFile);

// Simplified lexer functions
void lexer_init(const char* src);
Token* lexer_tokenize();
void lexer_skip_whitespace_and_comments();
char lexer_peek();
char lexer_get();
TokenType is_keyword(const char* word);
Token* create_token(TokenType type, const char* lexeme);

// GitHub-style main tokenization function
Node* getNextToken();

#endif
