/* Group No: 38
 * Members: Amar Kumar(2023A7PS0518P), Krishna Saraf(2023A7PS0606P), Ritik Kumar(2023A7PS0553P)
 *          Shivansh Saxena(2023A7PS0637P), Hitashi Raizada(2023A7PS0500P), Arnav Sahoo(2023A7PS0560P)
 */
#ifndef PARSERDEF_H
#define PARSERDEF_H

#include <stdio.h>
#include <stdlib.h>
#include "lexerDef.h"

/* Grammar constants */
#define MAX_RULES 120       /* Maximum number of grammar rules */
#define MAX_RHS 20          /* Maximum RHS symbols per rule */
#define NUM_NON_TERMINALS 54 /* Number of non-terminals */
#define NUM_TERMINALS 60     /* Number of terminals */
#define EPS -2              /* Epsilon symbol */
#define DOL -3              /* Dollar symbol (end of input) */
#define MAX_SET_SIZE 60     /* Maximum size for FIRST/FOLLOW sets */

/* Grammar structure */
typedef struct {
    int gnum[MAX_RULES][MAX_RHS]; /* Grammar rules: gnum[i][0]=LHS, gnum[i][1..]=RHS, terminated by -1 */
    int ruleLen[MAX_RULES];  /* Length of each rule's RHS */
    int numRules;            /* Total number of rules */
} Grammar;

/* First and Follow sets */
typedef struct {
    int first[NUM_NON_TERMINALS][MAX_SET_SIZE];   /* first[i][0] = count, first[i][1..count] = symbols */
    int follow[NUM_NON_TERMINALS][MAX_SET_SIZE];  /* follow[i][0] = count, follow[i][1..count] = symbols */
} FirstAndFollow;

/* Parse table */
typedef struct {
    int table[NUM_NON_TERMINALS][NUM_TERMINALS]; /* Parse table [non-terminal][terminal] = rule number or -1 */
} ParseTable;

/* Node structure for token info in parse tree */
typedef struct Node {
    TokenInfo *t;       /* Token information pointer */
} Node;

/* Parse tree node structure */
typedef struct ParseTree {
    int value;              /* Symbol value (terminal enum or non-terminal index) */
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
