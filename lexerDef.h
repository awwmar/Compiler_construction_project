/* Group No: 38
 * Members: Amar Kumar(2023A7PS0518P), Krishna Saraf(2023A7PS0606P), Ritik Kumar(2023A7PS0553P)
 *          Shivansh Saxena(2023A7PS0637P), Hitashi Raizada(2023A7PS0500P), Arnav Sahoo(2023A7PS0560P)
 */

#ifndef LEXERDEF_H
#define LEXERDEF_H

#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 4096
#define MAX_ID_LEN 20
#define MAX_LEXEME_LEN 128

typedef enum{

    TK_MAIN,TK_END,TK_INPUT,TK_OUTPUT,
    TK_PARAMETER,TK_PARAMETERS,TK_LIST,
    TK_INT,TK_REAL,TK_RECORD,TK_UNION,
    TK_TYPE,TK_ENDRECORD,TK_ENDUNION,
    TK_GLOBAL,TK_CALL,TK_WITH,
    TK_WHILE,TK_ENDWHILE,
    TK_IF,TK_THEN,TK_ELSE,TK_ENDIF,
    TK_READ,TK_WRITE,TK_RETURN,
    TK_DEFINETYPE,TK_AS,TK_NOT,
    TK_AND,TK_OR,

    TK_ID,TK_FUNID,TK_FIELDID,
    TK_RUID,TK_NUM,TK_RNUM,

    TK_SQL,TK_SQR,TK_SEM,
    TK_COLON,TK_DOT,TK_COMMA,
    TK_ASSIGNOP,TK_OP,TK_CL,
    TK_PLUS,TK_MINUS,TK_MUL,
    TK_DIV,TK_LT,TK_LE,TK_EQ,
    TK_GT,TK_GE,TK_NE,

    TK_COMMENT,TK_EOF,TK_ERROR

}TokenType;

typedef struct{
    TokenType type;
    char lexeme[MAX_LEXEME_LEN];
    int lineNo;
    union{
        int numVal;
        float decVal;
    }value;
    char errorMsg[256];
}TokenInfo;

typedef struct{
    char buf[2][BUFFER_SIZE+1];
    int curBuf;
    int fwd;
    int startBuf;
    int startPos;
    FILE *fp;
    int lineNo;
    int loaded[2];
    int done;
}TwinBuffer;

#endif