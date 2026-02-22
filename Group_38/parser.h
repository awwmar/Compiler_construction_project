/*
	parser.h

	Batch No: 38

	Members:
	Amar Kumar 2023A7PS0518P
	Krishna Saraf 2023A7PS0606P
	Ritik Kumar 2023A7PS0553P
	Shivansh Saxena 2023A7PS0637P
	Hitashi Raizada 2023A7PS0500P
	Arnav Sahoo 2023A7PS0560P
*/

#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include "parserDef.h"

/* Terminal and non-terminal names */
extern char* terms[60];

/* Grammar and parsing functions */
Grammar getGrammar(FILE* f);
int compareTerm(char* tocomp);
void make_stack();
void push(int ele);
int pop();
void printstack();
void insert_in_tree(ParseTree* current, int rule, Grammar G, Node* n);
ParseTree* searchposition(ParseTree* trav, int stacktop);
ParseTree* returnhead();
FirstAndFollow ComputeFirst(FirstAndFollow F, Grammar G);
FirstAndFollow ComputeFollow(FirstAndFollow F, Grammar G);
void printFF(FirstAndFollow F);
ParseTable createParseTable(FirstAndFollow F, ParseTable T, Grammar G);
void printTable(ParseTable T);
void parseInputSourceCode(FILE* f, ParseTable T, Grammar G, FILE* fs);
void printParseTreeToFile(ParseTree* trav);
void printParseTree();
void countNodes(ParseTree* head, int* count);

#endif
