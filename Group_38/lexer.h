/*
	lexer.h

	Batch No: 38

	Members:
	Amar Kumar 2023A7PS0518P
	Krishna Saraf 2023A7PS0606P
	Ritik Kumar 2023A7PS0553P
	Shivansh Saxena 2023A7PS0637P
	Hitashi Raizada 2023A7PS0500P
	Arnav Sahoo 2023A7PS0560P
*/

#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include "lexerDef.h"

/* Initialize twin buffer and load first chunk */
FILE *getStream(FILE *fp, TwinBuffer *B);

/* Get next token from twin buffer */
TokenInfo getNextToken(TwinBuffer *B);

/* Remove comments and write clean code to file, also prints to console */
void removeComments(char *testcaseFile, char *cleanFile);

/* Get string name of a token type */
const char *tokenTypeToString(TokenType t);

#endif
