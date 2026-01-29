/*

	lexer.c

	Batch No: 38

	Members:

	Amar Kumar 2023A7PS0518P
	Krishna Saraf 2023A7PS0606P
	Ritik Kumar 2023A7PS0553P
	Shivansh Saxena 2023A7PS0637P
	Hitashi Raizada 2023A7PS0500P
	Arnav Sahoo 2023A7PS0560P

*/

#include "lexer.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

// Define bool for C (GitHub style)
typedef enum { false, true } bool;

/* **************************************************************************** */

/* Global Variables */

int state = 1;

int line = 1;
int begin = 0;
int fwd = 0;
int fwd2 = 0;

int keyno = 35;
int tokno = 60;

char* KeywordTable[1009];

char keyword[31][35] =
{
	"integer",
	"real",
	"boolean",
	"of",
	"array",
	"start",
	"end",
	"declare",
	"module",
	"driver",
	"program",
	"get_value",
	"print",
	"use",
	"with",
	"parameters",
	"true",
	"false",
	"takes",
	"input",
	"returns",
	"AND",
	"OR",
	"for",
	"in",
	"switch",
	"case",
	"break",
	"default",
	"while"
};

char buffer[10000];
int buflen = 10000;

/* Global state variables for the lexer */
char* lexer_source = NULL;     // Points to the source code we're reading
size_t lexer_position = 0;     // Current position in the source code

/* Keywords that our programming language recognizes */
const char* keywords[] = {
    "if", "then", "else", "endif",
    "while", "endwhile",
    "read", "write", "return",
    "int", "real",
    "record", "union",
    "call", "with",
    "input", "output", "parameter", "list",
    "global", "_main"
};
const TokenType keyword_tokens[] = {
    TK_IF, TK_THEN, TK_ELSE, TK_ENDIF,
    TK_WHILE, TK_ENDWHILE,
    TK_READ, TK_WRITE, TK_RETURN,
    TK_INT, TK_REAL,
    TK_RECORD, TK_UNION,
    TK_CALL, TK_WITH,
    TK_INPUT, TK_OUTPUT, TK_PARAMETER, TK_LIST,
    TK_GLOBAL, TK_MAIN
};
#define NUM_KEYWORDS (sizeof(keywords) / sizeof(keywords[0]))

Token* newToken()
{
	Token* t = (Token*)malloc(sizeof(Token));
	return t;
}

Node* newNode()
{
	Node* n = (Node*)malloc(sizeof(Node));
	return n;
}

Head* newHead()
{
	Head* h = (Head*)malloc(sizeof(Head));
	return h;
}

void ClearMem(char* c, int len) // To Clear Char Arrays
{
	for (int i = 0; i < len; i++)
	{
		c[i] = '\0';
	}
}

bool isKeyword(char value[30])
{
	int flag = 1;

	for (int i = 0; i < 30; i++)
	{
		flag = strcmp(value, keyword[i]);
		if (flag == 0)
		{
			return true;
		}
	}
	return false;

	// int results=insert(value,KeywordTable);
	// printf("\nFor %s; Result: %d\n\n",value,results);

	/*int booleans=search(value,KeywordTable);

	printf("\nFor %s; %d \n\n",value,booleans);

	return booleans;*/
}

/* END Supporting Functions for Primary Functions */

/* **************************************************************************** */

/* START Primary Functions */

FILE* getStream(FILE* fp)
{
	if (fp == NULL)
	{
		printf("Cannot Open File!");
		exit(0);
	}

	ClearMem(buffer, buflen);

	fread(buffer, buflen, 1, fp);

	// printf("%s",buffer); //for debugging.

	return fp;
}

void removeComments(char* testcaseFile)
{
	FILE* f1 = fopen(testcaseFile, "r");
	if (f1 == NULL)
	{
		printf("Error opening file for removing comments!");
		return;
	}

	char c; // Reading Character my character

	while (!feof(f1))
	{
		c = fgetc(f1); // Gets initial character

		if (c == '*')
		{
			c = fgetc(f1);

			if (c == '*')
			{
				while (1)
				{
					c = fgetc(f1);
					if (c == '\n')
					{
						printf("%c", c);
					}
					else
					{
						if (c == '*')
						{
							c = fgetc(f1);
							if (c == '*')
							{
								break;
							}
							else
							{
								continue;
							}
						}
						else
						{
							continue;
						}
					}
				}
			}
			c = fgetc(f1);
		}
		if (!feof(f1))
			printf("%c", c);
	}
	fclose(f1);
}

/*
 * Initialize the lexer with source code
 * This sets up the global buffer like in the GitHub version
 */
void lexer_init(const char* src) {
    // Make a copy of the source code and put it in the global buffer
    lexer_source = strdup(src);
    strcpy(buffer, src);
    lexer_position = 0;
    fwd = 0;
    line = 1;
    state = 1;
}

/*
 * Look at the next character without moving forward
 */
char lexer_peek() {
    if (lexer_position >= strlen(lexer_source)) {
        return '\0';  // End of input
    }
    return lexer_source[lexer_position];
}

/*
 * Get the next character and move forward
 */
char lexer_get() {
    if (lexer_position >= strlen(lexer_source)) {
        return '\0';  // End of input
    }
    return lexer_source[lexer_position++];
}

/*
 * Skip over whitespace and comments
 */
void lexer_skip_whitespace_and_comments() {
    while (1) {
        // Skip regular whitespace (spaces and tabs)
        while (isspace(lexer_peek())) {
            lexer_get();
        }

        // Check for comments (start with %)
        if (lexer_peek() == '%') {
            // Skip everything until end of line
            while (lexer_peek() && lexer_peek() != '\n') {
                lexer_get();
            }
        } else {
            break;  // No more whitespace or comments
        }
    }
}

/*
 * Check if a word is a keyword
 */
TokenType is_keyword(const char* word) {
    for (size_t i = 0; i < NUM_KEYWORDS; i++) {
        if (strcmp(word, keywords[i]) == 0) {
            return keyword_tokens[i];
        }
    }
    return (TokenType)-1;  // Not a keyword
}

/*
 * Create a new token with the given type and text
 */
Token* create_token(TokenType type, const char* lexeme) {
    Token* token = (Token*)malloc(sizeof(Token));
    token->type = type;
    token->lexeme = strdup(lexeme);  // Make a copy of the lexeme
    return token;
}

/*
 * Main tokenization function - simplified version following GitHub style
 * This reads the next token using state machine approach
 */
Token* lexer_tokenize() {
    // Skip any whitespace and comments first
    lexer_skip_whitespace_and_comments();

    char current = lexer_peek();
    if (!current) {
        // End of input - return EOF token
        return create_token(TK_EOF, "");
    }

    // Check for the special assignment operator "<---"
    if (current == '<' && lexer_position + 3 < strlen(lexer_source) &&
        strncmp(lexer_source + lexer_position, "<---", 4) == 0) {
        lexer_position += 4;
        fwd += 4;
        return create_token(TK_ASSIGNOP, "<---");
    }

    // Check for numbers (integers and real numbers)
    if (isdigit(current)) {
        char buffer[256];  // Temporary buffer for building the number
        int buf_pos = 0;

        // Read the integer part
        while (isdigit(lexer_peek())) {
            buffer[buf_pos++] = lexer_get();
            fwd++;
        }
        buffer[buf_pos] = '\0';

        // Check if it's a real number (has decimal part)
        if (lexer_peek() == '.') {
            buffer[buf_pos++] = lexer_get();  // Add the decimal point
            fwd++;

            if (!isdigit(lexer_peek())) {
                // Error: decimal point but no digits after
                return create_token(TK_ERROR, "Invalid real number - no digits after decimal");
            }

            // Read the fractional part
            while (isdigit(lexer_peek())) {
                buffer[buf_pos++] = lexer_get();
                fwd++;
            }
            buffer[buf_pos] = '\0';

            return create_token(TK_RNUM, buffer);
        } else {
            // It's just an integer
            return create_token(TK_NUM, buffer);
        }
    }

    // Check for identifiers and keywords
    if (isalpha(current) || current == '_') {
        char buffer[256];  // Buffer for the identifier
        int buf_pos = 0;

        // Read the identifier (letters, digits, underscores)
        while (isalnum(lexer_peek()) || lexer_peek() == '_') {
            buffer[buf_pos++] = lexer_get();
            fwd++;
        }
        buffer[buf_pos] = '\0';

        // Check if it's a keyword
        TokenType keyword_type = is_keyword(buffer);
        if (keyword_type != (TokenType)-1) {
            return create_token(keyword_type, buffer);
        }

        // Check for special identifier types
        if (buffer[0] == '_') {
            return create_token(TK_FUNID, buffer);  // Function identifier
        } else {
            return create_token(TK_ID, buffer);  // Regular identifier
        }
    }

    // Check for record field identifiers (start with #)
    if (current == '#') {
        char buffer[256];
        int buf_pos = 0;
        buffer[buf_pos++] = lexer_get();  // Add the #
        fwd++;

        // Record IDs can have lowercase letters after #
        while (islower(lexer_peek())) {
            buffer[buf_pos++] = lexer_get();
            fwd++;
        }
        buffer[buf_pos] = '\0';

        return create_token(TK_RUID, buffer);
    }

    // Handle single-character tokens (operators and punctuation)
    lexer_get();  // Consume the character
    fwd++;
    switch (current) {
        case '+': return create_token(TK_PLUS, "+");
        case '-': return create_token(TK_MINUS, "-");
        case '*': return create_token(TK_MUL, "*");
        case '/': return create_token(TK_DIV, "/");
        case '(': return create_token(TK_OP, "(");
        case ')': return create_token(TK_CL, ")");
        case '.': return create_token(TK_DOT, ".");
        case ',': return create_token(TK_COMMA, ",");
        case ':': return create_token(TK_COLON, ":");
        case ';': return create_token(TK_SEM, ";");
        default:
            // Unknown character - return error token
            char error_msg[32];
            sprintf(error_msg, "Unknown character: %c", current);
            return create_token(TK_ERROR, error_msg);
    }
}

/*
 * GitHub-style getNextToken function
 * This is the main tokenization function with state machine
 */
Node* getNextToken()
{
	Node* newToken = (Node*)malloc(sizeof(Node));
	newToken->t = (Token*)malloc(sizeof(Token));
	ClearMem(newToken->t->token, 30);
	ClearMem(newToken->t->value, 30);
	newToken->t->lineno = 0;

	char ch;
	int tklen = 0; // used here to read into token/value

	char tokeninit[30];
	char valueinit[30];

	ClearMem(tokeninit, 30);
	ClearMem(valueinit, 30);

	while (1)
	{
		ch = buffer[fwd];
		switch (state)
		{
			case 1:  // This branch is for state 1 and all
					//  its direct paths in our original DFA

					if (ch == '\n')
					{
						ch = buffer[++fwd];
						// printf("\n %c \n",ch);
						line++;
						state = 1;
						break;
					}
					else if (ch == ' ' || ch == '\t')
					{
						// printf("Finds a space!\n");
						state = 1;
						ch = buffer[++fwd];
						break;
					}
					else if (((ch >= 'a') && (ch <= 'z')) || ((ch >= 'A') && (ch <= 'Z')))
					{
						valueinit[tklen++] = ch;
						state = 2;

					}
					else if ((ch >= '0') && (ch <= '9'))
					{
						// printf("\n Comes here!\n ");
						valueinit[tklen++] = ch;
						state = 3;
					}
					else if (ch == '+')
					{
						strcpy(newToken->t->token, "PLUS");
						strcpy(newToken->t->value, "+");
						newToken->t->lineno = line;
						fwd++;
						begin = fwd;
						state = 1;
						return newToken;
						break;
					}
					else if (ch == '-')
					{
						strcpy(newToken->t->token, "MINUS");
						strcpy(newToken->t->value, "-");
						newToken->t->lineno = line;
						fwd++;
						begin = fwd;
						state = 1;
						return newToken;
						break;
					}
					else if (ch == '*')
					{
						ch = buffer[++fwd];

						if (ch == '*')
						{
							while (1)
							{
								ch = buffer[++fwd];
								// printf("\n %c \n",ch);
								if (ch == '\n')
								{
									line++;
								}
								else
								{
									if (ch == '*')
									{
										ch = buffer[++fwd];
										if (ch == '*')
										{
											// printf("Comes here!");
											fwd++;
											state = 1;
											// printf("\n %c \n",buffer[fwd]);
											break;
										}
										else
										{
											continue;
										}
									}
									else
									{
										continue;
									}
								}
							}
						}
						else
						{
							strcpy(newToken->t->token, "MUL");
							strcpy(newToken->t->value, "*");
							newToken->t->lineno = line;
							begin = fwd;
							state = 1;
							return newToken;
							break;
						}
					}
					else if (ch == '/')
					{
						strcpy(newToken->t->token, "DIV");
						strcpy(newToken->t->value, "/");
						newToken->t->lineno = line;
						fwd++;
						begin = fwd;
						state = 1;
						return newToken;
						break;
					}
					else if (ch == '<')
					{
						ch = buffer[++fwd];

						if (ch == '=')
						{
							strcpy(newToken->t->token, "LE");
							strcpy(newToken->t->value, "<=");
							newToken->t->lineno = line;
							begin = fwd;
							fwd++;
							// printf("%c",buffer[fwd]);
							state = 1;
							return newToken;
							break;
						}
						else if (ch == '<')
						{
							ch = buffer[++fwd];

							if (ch == '<')
							{
								strcpy(newToken->t->token, "DRIVERDEF");
								strcpy(newToken->t->value, "<<<");
								newToken->t->lineno = line;
								fwd++;
								begin = fwd;
								state = 1;
								return newToken;
								break;
							}
							else
							{
								strcpy(newToken->t->token, "DEF");
								strcpy(newToken->t->value, "<<");
								newToken->t->lineno = line;
								begin = fwd;
								state = 1;
								return newToken;
								break;
							}
						}
						else
						{
							strcpy(newToken->t->token, "LT");
							strcpy(newToken->t->value, "<");
							newToken->t->lineno = line;
							begin = fwd;
							state = 1;
							return newToken;
							break;
						}
					}
					else if (ch == '>')
					{
						ch = buffer[++fwd];

						if (ch == '=')
						{
							strcpy(newToken->t->token, "GE");
							strcpy(newToken->t->value, ">=");
							newToken->t->lineno = line;
							fwd++;
							begin = fwd;
							state = 1;
							return newToken;
							break;
						}
						else if (ch == '>')
						{
							ch = buffer[++fwd];

							if (ch == '>')
							{
								strcpy(newToken->t->token, "DRIVERENDDEF");
								strcpy(newToken->t->value, ">>>");
								newToken->t->lineno = line;
								fwd++;
								begin = fwd;
								state = 1;
								return newToken;
								break;
							}
							else
							{
								strcpy(newToken->t->token, "ENDDEF");
								strcpy(newToken->t->value, ">>");
								newToken->t->lineno = line;
								begin = fwd;
								state = 1;
								return newToken;
								break;
							}
						}
						else
						{
							strcpy(newToken->t->token, "GT");
							strcpy(newToken->t->value, ">");
							newToken->t->lineno = line;
							begin = fwd;
							state = 1;
							return newToken;
							break;
						}
					}
					else if (ch == '=')
					{
						ch = buffer[++fwd];

						if (ch == '=')
						{
							strcpy(newToken->t->token, "EQ");
							strcpy(newToken->t->value, "==");
							newToken->t->lineno = line;
							fwd++;
							begin = fwd;
							state = 1;
							return newToken;
							break;
						}
						else
						{
							strcpy(newToken->t->token, "Error");
							strcpy(newToken->t->value, "=");
							newToken->t->lineno = line;
							// printf("Lexical Error: = at line no: %d\n",line);
							state = 1;
							begin = fwd;
							return newToken;
							break;
						}
					}
					else if (ch == '!')
					{
						ch = buffer[++fwd];

						if (ch == '=')
						{
							strcpy(newToken->t->token, "NE");
							strcpy(newToken->t->value, "!=");
							newToken->t->lineno = line;
							fwd++;
							begin = fwd;
							state = 1;
							return newToken;
							break;
						}
					}
					else if (ch == ':')
					{
						ch = buffer[++fwd];

						if (ch == '=')
						{
							strcpy(newToken->t->token, "ASSIGNOP");
							strcpy(newToken->t->value, ":=");
							newToken->t->lineno = line;
							fwd++;
							begin = fwd;
							state = 1;
							return newToken;
							break;
						}
						else
						{
							strcpy(newToken->t->token, "COLON");
							strcpy(newToken->t->value, ":");
							newToken->t->lineno = line;
							begin = fwd;
							state = 1;
							return newToken;
							break;
						}
					}
					else if (ch == '.')
					{
						ch = buffer[++fwd];

						if (ch == '.')
						{
							strcpy(newToken->t->token, "RANGEOP");
							strcpy(newToken->t->value, "..");
							newToken->t->lineno = line;
							fwd++;
							begin = fwd;
							state = 1;
							return newToken;
							break;
						}
						else
						{
							strcpy(newToken->t->token, "Error");
							newToken->t->value[0] = ch;
							newToken->t->lineno = line;
							// printf("Lexical Error: . at line no: %d\n",line);
							state = 1;
							begin = fwd;
							return newToken;
							break;
						}
					}
					else if (ch == ';')
					{
						strcpy(newToken->t->token, "SEMICOL");
						strcpy(newToken->t->value, ";");
						newToken->t->lineno = line;
						fwd++;
						begin = fwd;
						state = 1;
						return newToken;
						break;
					}
					else if (ch == ',')
					{
						// printf("in here comma");
						strcpy(newToken->t->token, "COMMA");
						strcpy(newToken->t->value, ",");
						newToken->t->lineno = line;
						fwd++;
						// printf(" %c \n",buffer[fwd]);
						begin = fwd;
						state = 1;
						return newToken;
						break;
					}
					else if (ch == '[')
					{
						strcpy(newToken->t->token, "SQBO");
						strcpy(newToken->t->value, "[");
						newToken->t->lineno = line;
						fwd++;
						begin = fwd;
						state = 1;
						return newToken;
						break;
					}
					else if (ch == ']')
					{
						strcpy(newToken->t->token, "SQBC");
						strcpy(newToken->t->value, "]");
						newToken->t->lineno = line;
						fwd++;
						begin = fwd;
						state = 1;
						return newToken;
						break;
					}
					else if (ch == '(')
					{
						strcpy(newToken->t->token, "BO");
						strcpy(newToken->t->value, "(");
						newToken->t->lineno = line;
						fwd++;
						begin = fwd;
						state = 1;
						return newToken;
						break;
					}
					else if (ch == ')')
					{
						strcpy(newToken->t->token, "BC");
						strcpy(newToken->t->value, ")");
						newToken->t->lineno = line;
						fwd++;
						begin = fwd;
						state = 1;
						return newToken;
						break;
					}
					else if (ch == '$')
					{
						strcpy(newToken->t->token, "DOLLAR");
						strcpy(newToken->t->value, "$");
						newToken->t->lineno = line;
						fwd++;
						begin = fwd;
						state = 1;
						return newToken;
						break;
					}
					else
					{
						if (ch != '\0')
						{
							// printf("\nyeh wala error %d\n",ch);
							strcpy(newToken->t->token, "EOF");
							// strcpy(newToken->t->value,ch);
							newToken->t->lineno = line;
							// printf("Lexical Error: %d at line no: %d\n",ch,line);
							state = 1;
							begin = fwd;
							return newToken;
						}
						else
						{
							strcpy(newToken->t->token, "EOB");
							// strcpy(newToken->t->value,ch);
							newToken->t->lineno = line;
							// printf("Lexical Error: %d at line no: %d\n",ch,line);
							state = 1;
							return newToken;
							break;
						}
					}
					break;

			case 2: // This branch is for state 2 and state 3
					// in our original DFA

					// Idenfier limit 20 characters including _

					// isKeyword(char value[30]);
					while (1)
					{
						ch = buffer[++fwd];
						if (((ch >= 'a') && (ch <= 'z')) || ((ch >= 'A') && (ch <= 'Z')) || ((ch >= '0') && (ch <= '9')) || (ch == '_'))
						{
							valueinit[tklen++] = ch;
							state = 2;
						}
						else
						{
							state = 1;
							break;
						}
					}

					bool isk = isKeyword(valueinit);

					// printf("Tklen for %s: %d\n",valueinit,tklen);

					if (isk)
					{
						char keyw[30];
						ClearMem(keyw, 30);

						for (int i = 0; valueinit[i] != '\0'; i++)
						{
							if (valueinit[i] >= 'a' && valueinit[i] <= 'z')
							{
								keyw[i] = valueinit[i] - 32;
							}
							else if (valueinit[i] == '_')
								keyw[i] = '_';
						}
						if (strcmp(valueinit, "AND") == 0)
						{
							strcpy(newToken->t->token, "AND");
							strcpy(newToken->t->value, valueinit);
							newToken->t->lineno = line;
							state = 1;
							begin = fwd;
							return newToken;
							break;
						}
						else if (strcmp(valueinit, "OR") == 0)
						{
							strcpy(newToken->t->token, "OR");
							strcpy(newToken->t->value, valueinit);
							newToken->t->lineno = line;
							state = 1;
							begin = fwd;
							return newToken;
							break;
						}
						else
						{
							strcpy(newToken->t->token, keyw);
							strcpy(newToken->t->value, valueinit);
							newToken->t->lineno = line;
							state = 1;
							begin = fwd;
							return newToken;
							break;
						}
					}
					else
					{
						if (tklen > 20)
						{
							strcpy(newToken->t->token, "Error,Id size>20");
							strcpy(newToken->t->value, valueinit);
							// printf("Identifier size too large! -- ");
							newToken->t->lineno = line;
							// printf("Lexical Error: %s at line no: %d\n",valueinit,line);
							state = 1;
							fwd++;
							begin = fwd;
							return newToken;
							break;
						}
						else
						{
							strcpy(newToken->t->token, "ID");
							strcpy(newToken->t->value, valueinit);
							newToken->t->lineno = line;
							return newToken;
							break;
						}
					}

					break;

			case 3: // This branch is for state 4 to state10
					// in our original DFA
					while (1)
					{
						ch = buffer[++fwd];
						// printf("%c\n",ch);

						if ((ch >= '0') && (ch <= '9'))
						{
							valueinit[tklen++] = ch;
							// printf("\n %s \n",valueinit);
							state = 3;
						}
						else if (ch == '.')
						{
							ch = buffer[++fwd];

							if (ch == '.')
							{
								fwd--;
								strcpy(newToken->t->token, "NUM");
								strcpy(newToken->t->value, valueinit);
								newToken->t->lineno = line;
								state = 1;
								return newToken;
								break;
							}
							else if ((ch >= '0') && (ch <= '9'))
							{
								valueinit[tklen++] = '.';
								valueinit[tklen++] = ch;
								while (1)
								{
									ch = buffer[++fwd];
									if ((ch >= '0') && (ch <= '9'))
									{
										valueinit[tklen++] = ch;
										// ch = buffer[++fwd];
										state = 3;
									}
									else if (ch == 'E' || ch == 'e')
									{
										valueinit[tklen++] = ch;
										ch = buffer[++fwd];
										if (ch == '+' || ch == '-')
										{
											valueinit[tklen++] = ch;
											ch = buffer[++fwd];
											if ((ch >= '0') && (ch <= '9'))
											{
												valueinit[tklen++] = ch;
												while (1)
												{
													ch = buffer[++fwd];
													// printf("\n %c \n",buffer[fwd]);
													if ((ch >= '0') && (ch <= '9'))
													{
														valueinit[tklen++] = ch;
														state = 3;
													}
													else
													{
														// printf("\n %c \n",buffer[fwd]);
														strcpy(newToken->t->token, "RNUM");
														strcpy(newToken->t->value, valueinit);
														newToken->t->lineno = line;
														state = 1;
														return newToken;
														break;
													}
												}
											}
											else
											{
												strcpy(newToken->t->token, "Error");
												strcpy(newToken->t->value, valueinit);
												newToken->t->lineno = line;
												// printf("Lexical Error: %s at line no: %d\n",valueinit,line);
												state = 1;
												begin = fwd;
												return newToken;
												break;
											}

										}
										else if ((ch >= '0') && (ch <= '9'))
										{
											valueinit[tklen++] = ch;
											while (1)
											{
												ch = buffer[++fwd];
												if ((ch >= '0') && (ch <= '9'))
												{
													valueinit[tklen++] = ch;
													ch = buffer[++fwd];
													state = 3;
												}
												else if (((ch >= 'a') && (ch <= 'z')) || ((ch >= 'A') && (ch <= 'Z')))
												{
													strcpy(newToken->t->token, "Error");
													strcpy(newToken->t->value, valueinit);
													newToken->t->lineno = line;
													// printf("Lexical Error: %s at line no: %d\n",valueinit,line);
													state = 1;
													fwd++;
													begin = fwd;
													return newToken;
													break;
												}
												else
												{
													state = 1;
													break;
												}
											}

										}
									}
									else if (((ch >= 'a') && (ch <= 'z')) || ((ch >= 'A') && (ch <= 'Z')))
									{
										// printf("\n %c \n",ch);
										strcpy(newToken->t->token, "Error");
										strcpy(newToken->t->value, valueinit);
										newToken->t->lineno = line;
										// printf("Lexical Error: %s at line no: %d\n",valueinit,line);
										state = 1;
										fwd++;
										begin = fwd;
										return newToken;
										break;
									}
									else
									{
										strcpy(newToken->t->token, "RNUM");
										strcpy(newToken->t->value, valueinit);
										newToken->t->lineno = line;
										state = 1;

										begin = fwd;
										return newToken;
										break;
									}
								}
							}
							else
							{
								fwd--;
								valueinit[tklen++] = buffer[fwd];
								strcpy(newToken->t->token, "Error");
								strcpy(newToken->t->value, valueinit);
								newToken->t->lineno = line;
								// printf("Lexical Error: %s at line no: %d\n",valueinit,line);
								state = 1;
								fwd++;
								begin = fwd;
								return newToken;
								break;
							}
						}

						else
						{
							// printf("\n %c \n",ch);
							// printf("\n %c \n",buffer[fwd]);
							// printf("%s",valueinit);
							strcpy(newToken->t->token, "NUM");
							strcpy(newToken->t->value, valueinit);
							newToken->t->lineno = line;
							state = 1;
							// printf("\n %c \n",buffer[fwd]);
							return newToken;
							break;
						}
					}

					break;

			default:
					printf("No matching case!\n");
					return newToken;
					break;
		}
	}
}

/* END Primary Functions */

/* **************************************************************************** */