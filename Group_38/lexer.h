/* Group No: 38
 * Members: Amar Kumar(2023A7PS0518P), Krishna Saraf(2023A7PS0606P), Ritik Kumar(2023A7PS0553P)
 *          Shivansh Saxena(2023A7PS0637P), Hitashi Raizada(2023A7PS0500P), Arnav Sahoo(2023A7PS0560P)
 */

#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include "lexerDef.h"

FILE *getStream(FILE *fp,TwinBuffer *B);
TokenInfo getNextToken(TwinBuffer *B);
void removeComments(char *testcaseFile);
const char *tokenTypeToString(TokenType t);

#endif