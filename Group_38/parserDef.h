/* Group No: 38
 * Members: Amar Kumar(2023A7PS0518P), Krishna Saraf(2023A7PS0606P), Ritik Kumar(2023A7PS0553P)
 *          Shivansh Saxena(2023A7PS0637P), Hitashi Raizada(2023A7PS0500P), Arnav Sahoo(2023A7PS0560P)
 */

#ifndef PARSERDEF_H
#define PARSERDEF_H

#include <stdio.h>
#include <stdlib.h>
#include "lexerDef.h"

#define MAX_RULES 120
#define MAX_RHS 20
#define NUM_NON_TERMINALS 54
#define NUM_TERMINALS 60
#define EPS -2
#define DOL -3
#define MAX_SET_SIZE 60

typedef struct{
    int gnum[MAX_RULES][MAX_RHS];
    int ruleLen[MAX_RULES];
    int numRules;
}Grammar;

typedef struct{
    int first[NUM_NON_TERMINALS][MAX_SET_SIZE];
    int follow[NUM_NON_TERMINALS][MAX_SET_SIZE];
}FirstAndFollow;

typedef struct{
    int table[NUM_NON_TERMINALS][NUM_TERMINALS];
}ParseTable;

typedef struct Node{
    TokenInfo *t;
}Node;

typedef struct ParseTree{
    int value;
    int rule;
    int isleaf;
    int isVisited;
    int isCG;
    Node *n;
    struct ParseTree *parent;
    struct ParseTree *child;
    struct ParseTree *right;
    struct ParseTree *left;
}ParseTree;

#endif