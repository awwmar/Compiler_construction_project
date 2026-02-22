/*
	lexerDef.h

	Batch No: 38

	Members:
	Amar Kumar 2023A7PS0518P
	Krishna Saraf 2023A7PS0606P
	Ritik Kumar 2023A7PS0553P
	Shivansh Saxena 2023A7PS0637P
	Hitashi Raizada 2023A7PS0500P
	Arnav Sahoo 2023A7PS0560P
*/

#ifndef LEXERDEF_H
#define LEXERDEF_H

#include <stdio.h>
#include <stdlib.h>

/* Twin buffer size - use 4096 for production, 50 for testing */
#define BUFFER_SIZE 4096

/* Maximum length for identifiers */
#define MAX_ID_LEN 20
#define MAX_LEXEME_LEN 128

/* Token type enumeration */
typedef enum {
    /* Keywords */
    TK_MAIN,
    TK_END,
    TK_INPUT,
    TK_OUTPUT,
    TK_PARAMETER,
    TK_PARAMETERS,
    TK_LIST,
    TK_INT,
    TK_REAL,
    TK_RECORD,
    TK_UNION,
    TK_TYPE,
    TK_ENDRECORD,
    TK_ENDUNION,
    TK_GLOBAL,
    TK_CALL,
    TK_WITH,
    TK_WHILE,
    TK_ENDWHILE,
    TK_IF,
    TK_THEN,
    TK_ELSE,
    TK_ENDIF,
    TK_READ,
    TK_WRITE,
    TK_RETURN,
    TK_DEFINETYPE,
    TK_AS,
    TK_NOT,
    TK_AND,
    TK_OR,

    /* Identifiers and literals */
    TK_ID,
    TK_FUNID,
    TK_FIELDID,
    TK_RUID,
    TK_NUM,
    TK_RNUM,

    /* Operators and punctuation */
    TK_SQL,
    TK_SQR,
    TK_SEM,
    TK_COLON,
    TK_DOT,
    TK_COMMA,
    TK_ASSIGNOP,
    TK_OP,
    TK_CL,
    TK_PLUS,
    TK_MINUS,
    TK_MUL,
    TK_DIV,
    TK_LT,
    TK_LE,
    TK_EQ,
    TK_GT,
    TK_GE,
    TK_NE,

    /* Special */
    TK_COMMENT,
    TK_EOF,
    TK_ERROR
} TokenType;

/* Token information structure */
typedef struct {
    TokenType type;
    char lexeme[MAX_LEXEME_LEN];
    int lineNo;
    union {
        int intVal;
        float realVal;
    } value;
    char errorMsg[256];
} TokenInfo;

/* Twin Buffer structure */
typedef struct {
    char buf[2][BUFFER_SIZE + 1]; /* Two buffers with sentinel space */
    int activeBuf;                /* 0 or 1 - which buffer is active for forward */
    int forward;                  /* Index within current buffer */
    int lexBeginBuf;              /* Buffer of lexeme begin */
    int lexBeginIdx;              /* Index of lexeme begin */
    FILE *fp;                     /* Source file pointer */
    int lineNo;                   /* Current line number */
    int bytesInBuf[2];            /* Bytes loaded in each buffer */
    int fileEnded;                /* Flag: has file been fully read */
} TwinBuffer;

#endif
