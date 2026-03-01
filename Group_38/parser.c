/* Group No: 38
 * Members: Amar Kumar(2023A7PS0518P), Krishna Saraf(2023A7PS0606P), Ritik Kumar(2023A7PS0553P)
 *          Shivansh Saxena(2023A7PS0637P), Hitashi Raizada(2023A7PS0500P), Arnav Sahoo(2023A7PS0560P)
 */
#include "parser.h"
#include "lexer.h"
#include <string.h>
#include <stdlib.h>

#define NUM_NT 53
#define NUM_RULES 85
#define TERM_OFFSET 100

static const char* ntNames[] = {
    "program",
    "mainFunction",
    "otherFunctions",
    "function",
    "input_par",
    "output_par",
    "parameter_list",
    "dataType",
    "primitiveDatatype",
    "constructedDatatype",
    "remaining_list",
    "stmts",
    "typeDefinitions",
    "actualOrRedefined",
    "typeDefinition",
    "fieldDefinitions",
    "fieldDefinition",
    "fieldType",
    "moreFields",
    "definetypestmt",
    "declarations",
    "declaration",
    "global_or_not",
    "otherStmts",
    "stmt",
    "assignmentStmt",
    "singleOrRecId",
    "option_single_constructed",
    "oneExpansion",
    "moreExpansions",
    "funCallStmt",
    "outputParameters",
    "inputParameters",
    "iterativeStmt",
    "conditionalStmt",
    "elsePart",
    "ioStmt",
    "arithmeticExpression",
    "expPrime",
    "term",
    "termPrime",
    "factor",
    "highPrecedenceOperators",
    "lowPrecedenceOperators",
    "booleanExpression",
    "var",
    "logicalOp",
    "relationalOp",
    "returnStmt",
    "optionalReturn",
    "idList",
    "more_ids",
    "A"
};

char* terms[120];
int numTerminals = 59;
int numNonTerminals = 45;

int isTerminal(int sym) { return sym >= TERM_OFFSET; }
int isNonTerminal(int sym) { return sym >= 0 && sym < NUM_NT; }

const char* getSymbolName(int sym) {
    if (sym == -2) return "EPS";
    if (sym == -1) return "END";
    if (sym >= 0 && sym < NUM_NT) return ntNames[sym];
    if (sym >= TERM_OFFSET) return tokenTypeToString((TokenType)(sym - TERM_OFFSET));
    return "UNKNOWN";
}

int compareTerm(char* tocomp) {
    int i;
    for (i = 0; i < NUM_NT; i++)
        if (strcmp(tocomp, ntNames[i]) == 0) return i;
    return -1;
}

/* Grammar rules dynamically parsed from file */
static int stringToToken(const char* str) {
    if (strcmp(str, "TK_MAIN") == 0) return 0;
    if (strcmp(str, "TK_END") == 0) return 1;
    if (strcmp(str, "TK_INPUT") == 0) return 2;
    if (strcmp(str, "TK_OUTPUT") == 0) return 3;
    if (strcmp(str, "TK_PARAMETER") == 0) return 4;
    if (strcmp(str, "TK_PARAMETERS") == 0) return 5;
    if (strcmp(str, "TK_LIST") == 0) return 6;
    if (strcmp(str, "TK_INT") == 0) return 7;
    if (strcmp(str, "TK_REAL") == 0) return 8;
    if (strcmp(str, "TK_RECORD") == 0) return 9;
    if (strcmp(str, "TK_UNION") == 0) return 10;
    if (strcmp(str, "TK_TYPE") == 0) return 11;
    if (strcmp(str, "TK_ENDRECORD") == 0) return 12;
    if (strcmp(str, "TK_ENDUNION") == 0) return 13;
    if (strcmp(str, "TK_GLOBAL") == 0) return 14;
    if (strcmp(str, "TK_CALL") == 0) return 15;
    if (strcmp(str, "TK_WITH") == 0) return 16;
    if (strcmp(str, "TK_WHILE") == 0) return 17;
    if (strcmp(str, "TK_ENDWHILE") == 0) return 18;
    if (strcmp(str, "TK_IF") == 0) return 19;
    if (strcmp(str, "TK_THEN") == 0) return 20;
    if (strcmp(str, "TK_ELSE") == 0) return 21;
    if (strcmp(str, "TK_ENDIF") == 0) return 22;
    if (strcmp(str, "TK_READ") == 0) return 23;
    if (strcmp(str, "TK_WRITE") == 0) return 24;
    if (strcmp(str, "TK_RETURN") == 0) return 25;
    if (strcmp(str, "TK_DEFINETYPE") == 0) return 26;
    if (strcmp(str, "TK_AS") == 0) return 27;
    if (strcmp(str, "TK_NOT") == 0) return 28;
    if (strcmp(str, "TK_AND") == 0) return 29;
    if (strcmp(str, "TK_OR") == 0) return 30;
    if (strcmp(str, "TK_ID") == 0) return 31;
    if (strcmp(str, "TK_FUNID") == 0) return 32;
    if (strcmp(str, "TK_FIELDID") == 0) return 33;
    if (strcmp(str, "TK_RUID") == 0) return 34;
    if (strcmp(str, "TK_NUM") == 0) return 35;
    if (strcmp(str, "TK_RNUM") == 0) return 36;
    if (strcmp(str, "TK_SQL") == 0) return 37;
    if (strcmp(str, "TK_SQR") == 0) return 38;
    if (strcmp(str, "TK_SEM") == 0) return 39;
    if (strcmp(str, "TK_COLON") == 0) return 40;
    if (strcmp(str, "TK_DOT") == 0) return 41;
    if (strcmp(str, "TK_COMMA") == 0) return 42;
    if (strcmp(str, "TK_ASSIGNOP") == 0) return 43;
    if (strcmp(str, "TK_OP") == 0) return 44;
    if (strcmp(str, "TK_CL") == 0) return 45;
    if (strcmp(str, "TK_PLUS") == 0) return 46;
    if (strcmp(str, "TK_MINUS") == 0) return 47;
    if (strcmp(str, "TK_MUL") == 0) return 48;
    if (strcmp(str, "TK_DIV") == 0) return 49;
    if (strcmp(str, "TK_LT") == 0) return 50;
    if (strcmp(str, "TK_LE") == 0) return 51;
    if (strcmp(str, "TK_EQ") == 0) return 52;
    if (strcmp(str, "TK_GT") == 0) return 53;
    if (strcmp(str, "TK_GE") == 0) return 54;
    if (strcmp(str, "TK_NE") == 0) return 55;
    if (strcmp(str, "TK_COMMENT") == 0) return 56;
    if (strcmp(str, "TK_EOF") == 0) return 57;
    if (strcmp(str, "TK_ERROR") == 0) return 58;
    return -1;
}

Grammar getGrammar() {
    Grammar G;
    G.numRules = 0;
    int i, j;
    for (i = 0; i < MAX_RULES; i++) {
        G.ruleLen[i] = 0;
        for (j = 0; j < MAX_RHS; j++) G.gnum[i][j] = -1;
    }
    
    FILE* fp = fopen("grammar.txt", "r");
    if (!fp) {
        printf("Error: Could not open grammar.txt\n");
        return G;
    }
    
    char line[512];
    while (fgets(line, sizeof(line), fp)) {
        int len = strlen(line);
        while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r')) {
            line[len-1] = '\0';
            len--;
        }
        if (len == 0) continue;
        
        char* token = strtok(line, " ");
        if (!token) continue;
        
        char lhsStr[64];
        if (token[0] == '<') {
            strncpy(lhsStr, token + 1, strlen(token) - 2);
            lhsStr[strlen(token) - 2] = '\0';
        } else {
            strcpy(lhsStr, token);
        }
        
        int lhsIdx = compareTerm(lhsStr);
        if (lhsIdx == -1) {
            printf("Unknown LHS: %s\n", lhsStr);
            continue;
        }
        
        token = strtok(NULL, " "); // skip ===>
        
        int currRuleIdx = G.numRules++;
        G.gnum[currRuleIdx][0] = lhsIdx;
        int rhsCount = 1;
        
        while ((token = strtok(NULL, " ")) != NULL) {
            if (strcmp(token, "|") == 0) {
                G.gnum[currRuleIdx][rhsCount] = -1;
                G.ruleLen[currRuleIdx] = rhsCount;
                
                currRuleIdx = G.numRules++;
                G.gnum[currRuleIdx][0] = lhsIdx;
                rhsCount = 1;
                continue;
            }
            if (strcmp(token, "eps") == 0 || strcmp(token, "\xE2\x88\x88") == 0) {
                G.gnum[currRuleIdx][rhsCount++] = -2; // EPS
            } else if (token[0] == '<') {
                char ntStr[64];
                strncpy(ntStr, token + 1, strlen(token) - 2);
                ntStr[strlen(token) - 2] = '\0';
                int idx = compareTerm(ntStr);
                if (idx != -1) {
                    G.gnum[currRuleIdx][rhsCount++] = idx;
                } else {
                    printf("Unknown NT: %s\n", ntStr);
                }
            } else {
                int tIdx = stringToToken(token);
                if (tIdx != -1) {
                    G.gnum[currRuleIdx][rhsCount++] = tIdx + TERM_OFFSET;
                } else {
                    printf("Unknown Term: %s\n", token);
                }
            }
        }
        G.gnum[currRuleIdx][rhsCount] = -1;
        G.ruleLen[currRuleIdx] = rhsCount;
    }
    fclose(fp);
    return G;
}

static int inSet(int* set, int val) {
    int i;
    for (i = 1; i <= set[0]; i++) {
        if (set[i] == val) return 1;
    }
    return 0;
}

static void addToSet(int* set, int val) {
    if (!inSet(set, val)) {
        set[++set[0]] = val;
    }
}

FirstAndFollow ComputeFirstAndFollowSets(Grammar G) {
    FirstAndFollow FF;
    int i, j, k;
    for (i = 0; i < NUM_NON_TERMINALS; i++) {
        FF.first[i][0] = 0;
        FF.follow[i][0] = 0;
    }

    int changed = 1;
    while (changed) {
        changed = 0;
        for (i = 0; i < G.numRules; i++) {
            int lhs = G.gnum[i][0];
            int allHasEps = 1;
            for (j = 1; G.gnum[i][j] != -1; j++) {
                int sym = G.gnum[i][j];
                if (sym == EPS) {
                    if (!inSet(FF.first[lhs], EPS)) {
                        addToSet(FF.first[lhs], EPS);
                        changed = 1;
                    }
                    allHasEps = 0;
                    break;
                } else if (isTerminal(sym)) {
                    if (!inSet(FF.first[lhs], sym)) {
                        addToSet(FF.first[lhs], sym);
                        changed = 1;
                    }
                    allHasEps = 0;
                    break;
                } else {
                    int hasEps = 0;
                    for (k = 1; k <= FF.first[sym][0]; k++) {
                        int f_item = FF.first[sym][k];
                        if (f_item == EPS) {
                            hasEps = 1;
                        } else {
                            if (!inSet(FF.first[lhs], f_item)) {
                                addToSet(FF.first[lhs], f_item);
                                changed = 1;
                            }
                        }
                    }
                    if (!hasEps) {
                        allHasEps = 0;
                        break;
                    }
                }
            }
            if (allHasEps && j > 1) {
                if (!inSet(FF.first[lhs], EPS)) {
                    addToSet(FF.first[lhs], EPS);
                    changed = 1;
                }
            }
        }
    }

    addToSet(FF.follow[0], 157);

    changed = 1;
    while (changed) {
        changed = 0;
        for (i = 0; i < G.numRules; i++) {
            int lhs = G.gnum[i][0];
            for (j = 1; G.gnum[i][j] != -1; j++) {
                int rhs_sym = G.gnum[i][j];
                if (isNonTerminal(rhs_sym)) {
                    int nextHasEps = 1;
                    int p;
                    for (p = j + 1; G.gnum[i][p] != -1; p++) {
                        int next_sym = G.gnum[i][p];
                        if (next_sym == EPS) {
                            continue;
                        } else if (isTerminal(next_sym)) {
                            if (!inSet(FF.follow[rhs_sym], next_sym)) {
                                addToSet(FF.follow[rhs_sym], next_sym);
                                changed = 1;
                            }
                            nextHasEps = 0;
                            break;
                        } else {
                            int hasEps = 0;
                            for (k = 1; k <= FF.first[next_sym][0]; k++) {
                                int f_item = FF.first[next_sym][k];
                                if (f_item == EPS) {
                                    hasEps = 1;
                                } else {
                                    if (!inSet(FF.follow[rhs_sym], f_item)) {
                                        addToSet(FF.follow[rhs_sym], f_item);
                                        changed = 1;
                                    }
                                }
                            }
                            if (!hasEps) {
                                nextHasEps = 0;
                                break;
                            }
                        }
                    }
                    if (nextHasEps) {
                        for (k = 1; k <= FF.follow[lhs][0]; k++) {
                            int f_item = FF.follow[lhs][k];
                            // EPS is not in Follow, no need to check
                            if (!inSet(FF.follow[rhs_sym], f_item)) {
                                addToSet(FF.follow[rhs_sym], f_item);
                                changed = 1;
                            }
                        }
                    }
                }
            }
        }
    }
    return FF;
}

ParseTable createParseTable(FirstAndFollow F, ParseTable T, Grammar G) {
    int i, j, k;
    for (i = 0; i < NUM_NON_TERMINALS; i++)
        for (j = 0; j < NUM_TERMINALS; j++)
            T.table[i][j] = -1;

    for (i = 0; i < G.numRules; i++) {
        int lhs = G.gnum[i][0];
        int rhsFirst[MAX_SET_SIZE];
        rhsFirst[0] = 0;
        
        int hasEps = 1;
        for (j = 1; G.gnum[i][j] != -1; j++) {
            int sym = G.gnum[i][j];
            if (sym == EPS) {
                addToSet(rhsFirst, EPS);
                break;
            } else if (isTerminal(sym)) {
                addToSet(rhsFirst, sym);
                hasEps = 0;
                break;
            } else { // sym is NT
                int symHasEps = 0;
                for (k = 1; k <= F.first[sym][0]; k++) {
                    int f_item = F.first[sym][k];
                    if (f_item == EPS) {
                        symHasEps = 1;
                    } else {
                        addToSet(rhsFirst, f_item);
                    }
                }
                if (!symHasEps) {
                    hasEps = 0;
                    break;
                }
            }
        }
        if (hasEps && j > 1) {
            addToSet(rhsFirst, EPS);
        }
        
        for (k = 1; k <= rhsFirst[0]; k++) {
            int term = rhsFirst[k];
            if (term != EPS) {
                T.table[lhs][term - TERM_OFFSET] = i;
            }
        }
        
        if (inSet(rhsFirst, EPS)) {
            for (k = 1; k <= F.follow[lhs][0]; k++) {
                int term = F.follow[lhs][k];
                if (term != EPS) {
                    T.table[lhs][term - TERM_OFFSET] = i;
                }
            }
        }
    }
    return T;
}

void printFF(FirstAndFollow F) {
    int i, j;
    printf("\n=== FIRST SETS ===\n");
    for (i = 0; i < NUM_NT; i++) {
        printf("FIRST(%s) = { ", ntNames[i]);
        for (j = 1; j <= F.first[i][0]; j++) {
            printf("%s ", getSymbolName(F.first[i][j]));
        }
        printf("}\n");
    }
    printf("\n=== FOLLOW SETS ===\n");
    for (i = 0; i < NUM_NT; i++) {
        printf("FOLLOW(%s) = { ", ntNames[i]);
        for (j = 1; j <= F.follow[i][0]; j++) {
            printf("%s ", getSymbolName(F.follow[i][j]));
        }
        printf("}\n");
    }
}

void printTable(ParseTable T) {
    int i, j;
    printf("\n=== PARSE TABLE ===\n");
    for (i = 0; i < NUM_NT; i++)
        for (j = 0; j < 59; j++)
            if (T.table[i][j] != -1)
                printf("T[%s][%s] = Rule %d\n", ntNames[i], tokenTypeToString((TokenType)j), T.table[i][j]);
}

#define MAX_STACK 500
static int stack[MAX_STACK];
static int sp = -1;

void make_stack() { sp = -1; }
void push(int ele) { if (sp < MAX_STACK-1) stack[++sp] = ele; }
int pop() { if (sp >= 0) return stack[sp--]; return -1; }
int stackTop() { if (sp >= 0) return stack[sp]; return -1; }
int stackEmpty() { return sp < 0; }
void printstack() {
    int i;
    printf("Stack: ");
    for (i = sp; i >= 0; i--)
        printf("%s ", getSymbolName(stack[i]));
    printf("\n");
}

static ParseTree* treeRoot = NULL;
static FILE* parseTreeOutFile = NULL;

ParseTree* createTreeNode(int value, int isleaf, ParseTree* parent) {
    ParseTree* node = (ParseTree*)malloc(sizeof(ParseTree));
    node->value = value;
    node->rule = -1;
    node->isleaf = isleaf;
    node->isVisited = 0;
    node->isCG = 0;
    node->n = NULL;
    node->parent = parent;
    node->child = NULL;
    node->right = NULL;
    node->left = NULL;
    return node;
}

void insert_in_tree(ParseTree* current, int rule, Grammar G, Node* n) {
    if (!current) return;
    current->rule = rule;
    (void)n;
    int i;
    int lhs = G.gnum[rule][0];
    (void)lhs;
    ParseTree* prev = NULL;
    for (i = 1; G.gnum[rule][i] != -1; i++) {
        int sym = G.gnum[rule][i];
        if (sym == -2) break; /* epsilon */
        int leaf = (sym >= TERM_OFFSET) ? 1 : 0;
        ParseTree* child = createTreeNode(sym, leaf, current);
        if (prev == NULL) {
            current->child = child;
        } else {
            prev->right = child;
            child->left = prev;
        }
        prev = child;
    }
}

ParseTree* searchposition(ParseTree* trav, int stacktop) {
    /* Find leftmost unvisited leaf matching stacktop */
    if (!trav) return NULL;
    if (trav->isleaf && !trav->isVisited && trav->value == stacktop)
        return trav;
    if (!trav->isleaf && !trav->isVisited && trav->value == stacktop)
        return trav;
    ParseTree* r = searchposition(trav->child, stacktop);
    if (r) return r;
    r = searchposition(trav->right, stacktop);
    return r;
}

ParseTree* returnhead() { return treeRoot; }

void countNodes(ParseTree* head, int* count) {
    if (!head) return;
    (*count)++;
    countNodes(head->child, count);
    countNodes(head->right, count);
}

static void printParseTreeInorder(ParseTree* node, FILE* fp) {
    if (!node) return;
    /* Inorder: left children first, then current, then right siblings */
    printParseTreeInorder(node->child, fp);

    /* Print current node */
    char lexeme[MAX_LEXEME_LEN];
    int lineNo = -1;
    char tokenName[64];
    char valueStr[64];
    char parentSymbol[64];
    char isLeaf[4];
    char nodeSymbol[64];

    strcpy(lexeme, "----");
    strcpy(tokenName, "----");
    strcpy(valueStr, "----");
    strcpy(isLeaf, "no");
    strcpy(nodeSymbol, "----");

    if (node->isleaf) {
        strcpy(isLeaf, "yes");
        if (node->n && node->n->t) {
            strncpy(lexeme, node->n->t->lexeme, MAX_LEXEME_LEN-1);
            lineNo = node->n->t->lineNo;
            strncpy(tokenName, tokenTypeToString(node->n->t->type), 63);
            if (node->n->t->type == TK_NUM)
                sprintf(valueStr, "%d", node->n->t->value.intVal);
            else if (node->n->t->type == TK_RNUM)
                sprintf(valueStr, "%f", node->n->t->value.realVal);
        } else {
            int tv = node->value - TERM_OFFSET;
            if (tv >= 0 && tv < 59)
                strncpy(tokenName, tokenTypeToString((TokenType)tv), 63);
        }
    } else {
        if (node->value >= 0 && node->value < NUM_NT)
            strncpy(nodeSymbol, ntNames[node->value], 63);
    }

    /* Parent symbol */
    if (node->parent == NULL) {
        strcpy(parentSymbol, "ROOT");
    } else if (!node->parent->isleaf && node->parent->value >= 0 && node->parent->value < NUM_NT) {
        strncpy(parentSymbol, ntNames[node->parent->value], 63);
    } else {
        strcpy(parentSymbol, "----");
    }

    fprintf(fp, "%-25s %-5d %-20s %-15s %-25s %-5s %-25s\n",
            lexeme, lineNo, tokenName, valueStr, parentSymbol, isLeaf, nodeSymbol);

    printParseTreeInorder(node->right, fp);
}

void printParseTree(ParseTree* PT, char* outfile) {
    if (!PT) { PT = treeRoot; }
    if (!PT) return;
    FILE* fp = fopen(outfile, "w");
    if (!fp) {
        printf("Error: Cannot open output file %s\n", outfile);
        return;
    }
    fprintf(fp, "%-25s %-5s %-20s %-15s %-25s %-5s %-25s\n",
            "lexeme", "line", "tokenName", "value", "parentNode", "leaf", "nodeSymbol");
    fprintf(fp, "-----------------------------------------------------------"
"------------------------------------------------------\n");
    printParseTreeInorder(PT, fp);
    fclose(fp);
    printf("Parse tree written to %s\n", outfile);
}

void printParseTreeToFile(ParseTree* trav) {
    if (parseTreeOutFile && trav)
        printParseTreeInorder(trav, parseTreeOutFile);
}

void parseInputSourceCode(char* testcaseFile, ParseTable T, Grammar G) {
    FILE* fp = fopen(testcaseFile, "r");
    if (!fp) {
        printf("Error: Cannot open file %s\n", testcaseFile);
        return;
    }

    TwinBuffer B;
    getStream(fp, &B);
    
    make_stack();
    push(157); // Push End-of-Input marker (TK_EOF + TERM_OFFSET)
    int startSymbol = 0; 
    push(startSymbol);

    FirstAndFollow F = ComputeFirstAndFollowSets(G);

    treeRoot = createTreeNode(startSymbol, 0, NULL);
    ParseTree* root = treeRoot;

    TokenInfo tok = getNextToken(&B);
    while (tok.type == TK_COMMENT || tok.type == TK_ERROR) {
        tok = getNextToken(&B);
    }
    
    int parseError = 0;
    while (!stackEmpty()) {
        int top = pop();
        
        if (top == 157) {
            if (tok.type == TK_EOF) {
                if (!parseError) printf("\nInput source code is syntactically correct...........\n");
            } else {
                printf("Line %d Error: Expected EOF, but found Lexeme '%s'\n", tok.lineNo, tok.lexeme);
                parseError = 1;
            }
            break;
        }
        
        if (top == EPS) continue; 
        
        if (isTerminal(top)) {
            int currentTokTypeInt = (int)tok.type + TERM_OFFSET;
            
            if (top == currentTokTypeInt) {
                ParseTree* activeNode = searchposition(root, top);
                if (activeNode != NULL) {
                    activeNode->n = (Node*)malloc(sizeof(Node));
                    activeNode->n->t = (TokenInfo*)malloc(sizeof(TokenInfo));
                    *(activeNode->n->t) = tok;
                    activeNode->isVisited = 1;
                }
                tok = getNextToken(&B);
                while (tok.type == TK_COMMENT || tok.type == TK_ERROR) tok = getNextToken(&B);
            } else {
                printf("Line %d Error: The token %s for lexeme %s  does not match with the expected token %s\n",
                       tok.lineNo, tokenTypeToString(tok.type), tok.lexeme,
                       tokenTypeToString((TokenType)(top - TERM_OFFSET)));
                parseError = 1;
                ParseTree* tnode = searchposition(treeRoot, top);
                if (tnode) tnode->isVisited = 1;
            }
        } 
        else if (isNonTerminal(top)) {
            int currentTokTypeInt = (int)tok.type;
            int ruleIdx = -1;
            if (currentTokTypeInt >= 0 && currentTokTypeInt < NUM_TERMINALS) {
                ruleIdx = T.table[top][currentTokTypeInt];
            }
            
            if (ruleIdx != -1) {
                ParseTree* activeNode = searchposition(root, top);
                if (activeNode != NULL) {
                    insert_in_tree(activeNode, ruleIdx, G, NULL);
                    activeNode->isVisited = 1;
                }
                
                int rhs[MAX_RHS];
                int rhsLength = 0;
                for (int i = 1; G.gnum[ruleIdx][i] != -1; i++) {
                    if (G.gnum[ruleIdx][i] == EPS) break;
                    rhs[rhsLength++] = G.gnum[ruleIdx][i];
                }
                for (int i = rhsLength - 1; i >= 0; i--) {
                    push(rhs[i]);
                }
            } else {
                printf("Line %d Error: Invalid token %s encountered with value %s stack top %s\n",
                       tok.lineNo, tokenTypeToString(tok.type), tok.lexeme, ntNames[top]);
                parseError = 1;
                
                // Panic Mode Error Recovery
                printf("Waiting for an element in follow of \"%s\"\n", ntNames[top]);
                int recovered = 0;
                while (tok.type != TK_EOF) {
                    int tokValAsOffset = (int)tok.type + TERM_OFFSET;
                    int inFollow = 0;
                    for (int fi = 1; fi <= F.follow[top][0]; fi++) {
                        if (F.follow[top][fi] == tokValAsOffset) {
                            inFollow = 1;
                            break;
                        }
                    }
                    if (inFollow) {
                        recovered = 1;
                        break;
                    }
                    tok = getNextToken(&B);
                    while (tok.type == TK_COMMENT || tok.type == TK_ERROR) tok = getNextToken(&B);
                }
                if (recovered) {
                    printf("Token \"%s\" found at line number %d\n", tokenTypeToString(tok.type), tok.lineNo);
                    printf("Resuming parsing\n");
                } else {
                    printf("Panic Mode Error Recovery Not possible, Partial parse tree formed\n");
                    break;
                }
            }
        }
    }
    
    if (parseError) {
        printf("Parsing failed due to syntax errors.\n");
    }
    fclose(fp);
}
