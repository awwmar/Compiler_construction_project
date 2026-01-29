
#ifndef TOKEN_H
#define TOKEN_H

#include <stdlib.h>
#include <string.h>

// Token types for our programming language
typedef enum {
    TK_ID, TK_FIELDID, TK_FUNID, TK_RUID,
    TK_NUM, TK_RNUM,
    TK_ASSIGNOP,
    TK_PLUS, TK_MINUS, TK_MUL, TK_DIV,
    TK_LT, TK_LE, TK_EQ, TK_GT, TK_GE, TK_NE,
    TK_OP, TK_CL, TK_DOT, TK_COMMA, TK_COLON, TK_SEM,
    TK_IF, TK_THEN, TK_ELSE, TK_ENDIF,
    TK_WHILE, TK_ENDWHILE,
    TK_READ, TK_WRITE, TK_RETURN,
    TK_INT, TK_REAL, TK_RECORD, TK_UNION,
    TK_CALL, TK_WITH, TK_INPUT, TK_OUTPUT, TK_PARAMETER, TK_LIST, TK_GLOBAL, TK_MAIN,
    TK_EOF, TK_ERROR
} TokenType;

// Structure to hold a token - contains its type and the actual text
typedef struct {
    TokenType type;    // What kind of token this is (like TK_ID, TK_NUM, etc.)
    char* lexeme;      // The actual text from the source code
} Token;

#endif
