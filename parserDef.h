/*
	parserDef.h

	Batch No: 38

	Members:
	Amar Kumar 2023A7PS0518P
	Krishna Saraf 2023A7PS0606P
	Ritik Kumar 2023A7PS0553P
	Shivansh Saxena 2023A7PS0637P
	Hitashi Raizada 2023A7PS0500P
	Arnav Sahoo 2023A7PS0560P
*/

#ifndef PARSERDEF_H
#define PARSERDEF_H

#include <stdio.h>
#include <stdlib.h>
#include "lexerDef.h"

/* Grammar constants */
#define GR 50      /* Number of grammar rules */
#define NTER 30    /* Number of non-terminals */
#define TER 30     /* Number of terminals */
#define EPS -2     /* Epsilon symbol */
#define DOL -3     /* Dollar symbol (end of input) */

/* Grammar structure */
typedef struct {
    int gnum[50][30];  /* Grammar rules stored as arrays */
} Grammar;

/* First and Follow sets */
typedef struct {
    int first[120][15];   /* First sets for each non-terminal */
    int follow[120][15];  /* Follow sets for each non-terminal */
} FirstAndFollow;

/* Parse table */
typedef struct {
    int table[30][30];  /* Parse table [non-terminal][terminal] */
} ParseTable;

/* Node structure */
typedef struct Node {
    TokenInfo *t;       /* Token information pointer */
} Node;

/* Parse tree node structure */
typedef struct ParseTree {
    int value;              /* Symbol value (terminal/non-terminal) */
    int rule;               /* Grammar rule number */
    int isleaf;             /* Is this a leaf node? */
    int isVisited;          /* Has this node been visited? */
    int isCG;               /* Code generation flag */
    Node *n;                /* Node information */
    struct ParseTree *parent;   /* Parent node */
    struct ParseTree *child;    /* First child */
    struct ParseTree *right;    /* Next sibling */
    struct ParseTree *left;     /* Previous sibling */
} ParseTree;

#endif
