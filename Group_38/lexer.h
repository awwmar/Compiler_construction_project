/* Group No: 38
 * Members: Amar Kumar(2023A7PS0518P), Krishna Saraf(2023A7PS0606P), Ritik Kumar(2023A7PS0553P)
 *          Shivansh Saxena(2023A7PS0637P), Hitashi Raizada(2023A7PS0500P), Arnav Sahoo(2023A7PS0560P)
 */
#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include "lexerDef.h"

// Start the reader and grab the first chunk of code
FILE *getStream(FILE *fp, TwinBuffer *B);

// Get the next token from the pile
TokenInfo getNextToken(TwinBuffer *B);

// Delete comments and just print the clean stuff
void removeComments(char *testcaseFile);

// Change a code label into a name people can actually read
const char *tokenTypeToString(TokenType t);

#endif
