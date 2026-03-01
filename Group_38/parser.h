/* Group No: 38
 * Members: Amar Kumar(2023A7PS0518P), Krishna Saraf(2023A7PS0606P), Ritik Kumar(2023A7PS0553P)
 *          Shivansh Saxena(2023A7PS0637P), Hitashi Raizada(2023A7PS0500P), Arnav Sahoo(2023A7PS0560P)
 */
#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include "parserDef.h"

/* Terminal and non-terminal names */
extern char* terms[];
extern int numTerminals;
extern int numNonTerminals;

/* Grammar and parsing functions */
Grammar getGrammar();
int compareTerm(char* tocomp);

/* Stack operations */
void make_stack();
void push(int ele);
int pop();
int stackTop();
int stackEmpty();
void printstack();

/* Parse tree operations */
void insert_in_tree(ParseTree* current, int rule, Grammar G, Node* n);
ParseTree* searchposition(ParseTree* trav, int stacktop);
ParseTree* returnhead();

/* First, Follow, and Parse Table */
FirstAndFollow ComputeFirstAndFollowSets(Grammar G);
ParseTable createParseTable(FirstAndFollow F, ParseTable T, Grammar G);
void printFF(FirstAndFollow F);
void printTable(ParseTable T);

/* Parsing */
void parseInputSourceCode(char* testcaseFile, ParseTable T, Grammar G);
void printParseTree(ParseTree* PT, char* outfile);
void countNodes(ParseTree* head, int* count);

/* Utility */
int isTerminal(int sym);
int isNonTerminal(int sym);
const char* getSymbolName(int sym);

#endif
