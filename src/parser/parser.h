
#ifndef PARSER_H
#define PARSER_H

#include "lexer/token.h"

// Grammar constants - simplified version
#define GR 50      // Number of grammar rules
#define NTER 30    // Number of non-terminals
#define TER 30     // Number of terminals
#define EPS -2     // Epsilon symbol
#define DOL -3     // Dollar symbol (end of input)

// Grammar structure
typedef struct {
    int gnum[50][30];  // Grammar rules stored as arrays
} Grammar;

// First and Follow sets
typedef struct {
    int first[120][15];   // First sets for each non-terminal
    int follow[120][15];  // Follow sets for each non-terminal
} FirstAndFollow;

// Parse table
typedef struct {
    int table[30][30];  // Parse table [non-terminal][terminal]
} ParseTable;

// Parse tree node structure
typedef struct ParseTree {
    int value;              // Symbol value (terminal/non-terminal)
    int rule;               // Grammar rule number
    int isleaf;             // Is this a leaf node?
    int isVisited;          // Has this node been visited?
    int isCG;               // Code generation flag
    Token* t;               // Token information
    struct ParseTree* parent;   // Parent node
    struct ParseTree* child;    // First child
    struct ParseTree* right;    // Next sibling
    struct ParseTree* left;     // Previous sibling
} ParseTree;

// Node structure (simplified)
typedef struct Node {
    Token* t;               // Token pointer
} Node;

// Terminal and non-terminal names (simplified)
extern char* terms[60];

// Structure to hold parser state
typedef struct {
    Token** tokens;      // Array of tokens to parse
    size_t num_tokens;   // How many tokens we have
    size_t position;     // Current position in the token array
} Parser;

// Function declarations for the parser
Parser* parser_create(Token** tokens, size_t num_tokens);
void parser_destroy(Parser* parser);
void parser_parse(Parser* parser);
const Token* parser_current(Parser* parser);
void parser_eat(Parser* parser, TokenType expected_type);

// Grammar and parsing functions (GitHub style)
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

// Simplified parsing functions
void parser_assignment(Parser* parser);
void parser_expr(Parser* parser);
void parser_expr_prime(Parser* parser);
void parser_term(Parser* parser);
void parser_term_prime(Parser* parser);
void parser_factor(Parser* parser);
void parser_var(Parser* parser);

#endif
