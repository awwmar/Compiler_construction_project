/* Group No: 38
 * Members: Amar Kumar(2023A7PS0518P), Krishna Saraf(2023A7PS0606P), Ritik Kumar(2023A7PS0553P)
 *          Shivansh Saxena(2023A7PS0637P), Hitashi Raizada(2023A7PS0500P), Arnav Sahoo(2023A7PS0560P)
 */
#include "lexer.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/* ========== Keyword Table ========== */
typedef struct {
    const char *word;
    TokenType type;
} KeywordEntry;

static KeywordEntry keywordTable[] = {
    {"main",       TK_MAIN}, 
    {"end",        TK_END},
    {"input",      TK_INPUT},
    {"output",     TK_OUTPUT},
    {"parameter",  TK_PARAMETER},
    {"parameters", TK_PARAMETERS},
    {"list",       TK_LIST},
    {"int",        TK_INT},
    {"real",       TK_REAL},
    {"record",     TK_RECORD},
    {"union",      TK_UNION},
    {"type",       TK_TYPE},
    {"endrecord",  TK_ENDRECORD},
    {"endunion",   TK_ENDUNION},
    {"global",     TK_GLOBAL},
    {"call",       TK_CALL},
    {"with",       TK_WITH},
    {"while",      TK_WHILE},
    {"endwhile",   TK_ENDWHILE},
    {"if",         TK_IF},
    {"then",       TK_THEN},
    {"else",       TK_ELSE},
    {"endif",      TK_ENDIF},
    {"read",       TK_READ},
    {"write",      TK_WRITE},
    {"return",     TK_RETURN},
    {"definetype", TK_DEFINETYPE},
    {"as",         TK_AS},
    {"not",        TK_NOT},
    {"and",        TK_AND},
    {"or",         TK_OR},
};

#define NUM_KEYWORDS (sizeof(keywordTable) / sizeof(keywordTable[0]))

static TokenType lookupKeyword(const char *word) {
    for (int i = 0; i < (int)NUM_KEYWORDS; i++) {
        if (strcmp(word, keywordTable[i].word) == 0)
            return keywordTable[i].type;
    }
    return TK_ERROR; /* Not a keyword */
}

/* ========== Token type to string ========== */
const char *tokenTypeToString(TokenType t) {
    switch (t) {
        case TK_MAIN: return "TK_MAIN";
        case TK_END: return "TK_END";
        case TK_INPUT: return "TK_INPUT";
        case TK_OUTPUT: return "TK_OUTPUT";
        case TK_PARAMETER: return "TK_PARAMETER";
        case TK_PARAMETERS: return "TK_PARAMETERS";
        case TK_LIST: return "TK_LIST";
        case TK_INT: return "TK_INT";
        case TK_REAL: return "TK_REAL";
        case TK_RECORD: return "TK_RECORD";
        case TK_UNION: return "TK_UNION";
        case TK_TYPE: return "TK_TYPE";
        case TK_ENDRECORD: return "TK_ENDRECORD";
        case TK_ENDUNION: return "TK_ENDUNION";
        case TK_GLOBAL: return "TK_GLOBAL";
        case TK_CALL: return "TK_CALL";
        case TK_WITH: return "TK_WITH";
        case TK_WHILE: return "TK_WHILE";
        case TK_ENDWHILE: return "TK_ENDWHILE";
        case TK_IF: return "TK_IF";
        case TK_THEN: return "TK_THEN";
        case TK_ELSE: return "TK_ELSE";
        case TK_ENDIF: return "TK_ENDIF";
        case TK_READ: return "TK_READ";
        case TK_WRITE: return "TK_WRITE";
        case TK_RETURN: return "TK_RETURN";
        case TK_DEFINETYPE: return "TK_DEFINETYPE";
        case TK_AS: return "TK_AS";
        case TK_NOT: return "TK_NOT";
        case TK_AND: return "TK_AND";
        case TK_OR: return "TK_OR";
        case TK_ID: return "TK_ID";
        case TK_FUNID: return "TK_FUNID";
        case TK_FIELDID: return "TK_FIELDID";
        case TK_RUID: return "TK_RUID";
        case TK_NUM: return "TK_NUM";
        case TK_RNUM: return "TK_RNUM";
        case TK_SQL: return "TK_SQL";
        case TK_SQR: return "TK_SQR";
        case TK_SEM: return "TK_SEM";
        case TK_COLON: return "TK_COLON";
        case TK_DOT: return "TK_DOT";
        case TK_COMMA: return "TK_COMMA";
        case TK_ASSIGNOP: return "TK_ASSIGNOP";
        case TK_OP: return "TK_OP";
        case TK_CL: return "TK_CL";
        case TK_PLUS: return "TK_PLUS";
        case TK_MINUS: return "TK_MINUS";
        case TK_MUL: return "TK_MUL";
        case TK_DIV: return "TK_DIV";
        case TK_LT: return "TK_LT";
        case TK_LE: return "TK_LE";
        case TK_EQ: return "TK_EQ";
        case TK_GT: return "TK_GT";
        case TK_GE: return "TK_GE";
        case TK_NE: return "TK_NE";
        case TK_COMMENT: return "TK_COMMENT";
        case TK_EOF: return "TK_EOF";
        case TK_ERROR: return "TK_ERROR";
    }
    return "UNKNOWN";
}

/* ========== Twin Buffer Implementation ========== */
static void loadBuffer(TwinBuffer *B, int which) {
    if (B->fileEnded) {
        B->bytesInBuf[which] = 0;
        B->buf[which][0] = '\0';
        return;
    }
    size_t n = fread(B->buf[which], 1, BUFFER_SIZE, B->fp);
    B->bytesInBuf[which] = (int)n;
    B->buf[which][n] = '\0'; /* Sentinel */
    if ((int)n < BUFFER_SIZE)
        B->fileEnded = 1;
}

FILE *getStream(FILE *fp, TwinBuffer *B) {
    if (!fp) {
        printf("Error: Cannot open file!\n");
        return NULL;
    }
    B->fp = fp;
    B->lineNo = 1;
    B->fileEnded = 0;
    B->activeBuf = 0;
    B->forward = 0;
    B->lexBeginBuf = 0;
    B->lexBeginIdx = 0;
    B->bytesInBuf[0] = 0;
    B->bytesInBuf[1] = 0;

    loadBuffer(B, 0);
    loadBuffer(B, 1);

    return fp;
}

/* Get character at forward pointer and advance */
static char nextChar(TwinBuffer *B) {
    if (B->forward >= B->bytesInBuf[B->activeBuf]) {
        /* Switch to other buffer */
        int other = 1 - B->activeBuf;
        if (B->bytesInBuf[other] == 0 && B->fileEnded)
            return EOF;
        B->activeBuf = other;
        B->forward = 0;
        /* Reload the buffer we just left */
        loadBuffer(B, 1 - other);
    }
    if (B->forward >= B->bytesInBuf[B->activeBuf])
        return EOF;
    char c = B->buf[B->activeBuf][B->forward];
    B->forward++;
    return c;
}

/* Peek at current character without advancing */
static char peekChar(TwinBuffer *B) {
    if (B->forward >= B->bytesInBuf[B->activeBuf]) {
        int other = 1 - B->activeBuf;
        if (B->bytesInBuf[other] == 0 && B->fileEnded)
            return EOF;
        /* Look into the other buffer position 0 */
        return B->buf[other][0];
    }
    return B->buf[B->activeBuf][B->forward];
}

/* Retract forward pointer by 1 */
static void retract(TwinBuffer *B) {
    if (B->forward > 0) {
        B->forward--;
    } else {
        /* Go back to the other buffer's last position */
        int other = 1 - B->activeBuf;
        B->activeBuf = other;
        B->forward = B->bytesInBuf[other] - 1;
    }
}

/* Helper to check if char is in [b-d] */
static int isBD(char c) { return c >= 'b' && c <= 'd'; }

/* Helper to check if char is in [2-7] */
static int is27(char c) { return c >= '2' && c <= '7'; }

/* ========== getNextToken ========== */
TokenInfo getNextToken(TwinBuffer *B) {
    TokenInfo tok;
    tok.type = TK_ERROR;
    tok.lexeme[0] = '\0';
    tok.lineNo = B->lineNo;
    tok.value.intVal = 0;
    tok.errorMsg[0] = '\0';

    char c;
    int idx;

    /* Skip whitespace */
    while (1) {
        c = nextChar(B);
        if (c == EOF) {
            tok.type = TK_EOF;
            strcpy(tok.lexeme, "EOF");
            tok.lineNo = B->lineNo;
            return tok;
        }
        if (c == '\n') {
            B->lineNo++;
            continue;
        }
        if (c == ' ' || c == '\t' || c == '\r')
            continue;
        break;
    }

    tok.lineNo = B->lineNo;

    /* ---- COMMENT: % ---- */
    if (c == '%') {
        tok.type = TK_COMMENT;
        strcpy(tok.lexeme, "%");
        /* Skip rest of line */
        while (1) {
            char ch = nextChar(B);
            if (ch == '\n') { B->lineNo++; break; }
            if (ch == EOF) break;
        }
        return tok;
    }

    /* ---- FUNID: _[a-zA-Z0-9]* ---- */
    if (c == '_') {
        idx = 0;
        tok.lexeme[idx++] = c;
        while (1) {
            char ch = peekChar(B);
            if (isalnum(ch) || ch == '_') {
                tok.lexeme[idx++] = nextChar(B);
            } else break;
        }
        tok.lexeme[idx] = '\0';
        if (strcmp(tok.lexeme, "_main") == 0)
            tok.type = TK_MAIN;
        else
            tok.type = TK_FUNID;
        return tok;
    }

    /* ---- RUID: #[a-z]+ ---- */
    if (c == '#') {
        idx = 0;
        tok.lexeme[idx++] = c;
        while (islower(peekChar(B))) {
            tok.lexeme[idx++] = nextChar(B);
        }
        tok.lexeme[idx] = '\0';
        tok.type = TK_RUID;
        return tok;
    }

    /* ---- Identifiers starting with lowercase letter ---- */
    if (islower(c)) {
        idx = 0;
        tok.lexeme[idx++] = c;

        /* Check if this could be TK_ID: [b-d][2-7]([b-d]|[2-7])* */
        if (isBD(c) && is27(peekChar(B))) {
            /* TK_ID path */
            tok.lexeme[idx++] = nextChar(B); /* consume the [2-7] */
            while (isBD(peekChar(B)) || is27(peekChar(B))) {
                if (idx < MAX_LEXEME_LEN - 1)
                    tok.lexeme[idx++] = nextChar(B);
                else
                    nextChar(B); /* consume but don't store */
            }
            tok.lexeme[idx] = '\0';

            if (idx > MAX_ID_LEN) {
                tok.type = TK_ERROR;
                sprintf(tok.errorMsg, "Line No %d: Error :Variable Identifier is longer than the prescribed length of 20 characters.\n", tok.lineNo);
                printf("%s", tok.errorMsg);
                return tok;
            }
            tok.type = TK_ID;
            return tok;
        }

        /* Keyword / FIELDID path: [a-z]+ */
        while (islower(peekChar(B))) {
            if (idx < MAX_LEXEME_LEN - 1)
                tok.lexeme[idx++] = nextChar(B);
            else
                nextChar(B);
        }
        tok.lexeme[idx] = '\0';

        /* Check keyword */
        TokenType kw = lookupKeyword(tok.lexeme);
        if (kw != TK_ERROR) {
            tok.type = kw;
        } else {
            tok.type = TK_FIELDID;
        }
        return tok;
    }

    /* ---- Numbers: [0-9]+ with optional .XX and EXX ---- */
    if (isdigit(c)) {
        idx = 0;
        tok.lexeme[idx++] = c;

        /* Read all integer digits */
        while (isdigit(peekChar(B))) {
            tok.lexeme[idx++] = nextChar(B);
        }
        tok.lexeme[idx] = '\0';

        /* Check for decimal point */
        if (peekChar(B) == '.') {
            char dot = nextChar(B); /* consume dot */

            /* Count fractional digits */
            tok.lexeme[idx++] = dot;

            int fracDigits = 0;
            while (isdigit(peekChar(B))) {
                tok.lexeme[idx++] = nextChar(B);
                fracDigits++;
            }
            tok.lexeme[idx] = '\0';

            /* Must have exactly 2 fractional digits */
            if (fracDigits != 2) {
                /* Error: Unknown pattern */
                tok.type = TK_ERROR;
                sprintf(tok.errorMsg, "Line no: %d : Error: Unknown pattern <%s>\n", tok.lineNo, tok.lexeme);
                printf("%s", tok.errorMsg);
                return tok;
            }

            /* Check for E (scientific notation) */
            if (peekChar(B) == 'E') {
                tok.lexeme[idx++] = nextChar(B); /* consume E */

                /* Optional sign */
                char pk = peekChar(B);
                if (pk == '+' || pk == '-') {
                    tok.lexeme[idx++] = nextChar(B);
                }

                /* Must have exactly 2 exponent digits */
                int expDigits = 0;
                while (isdigit(peekChar(B)) && expDigits < 2) {
                    tok.lexeme[idx++] = nextChar(B);
                    expDigits++;
                }
                tok.lexeme[idx] = '\0';

                if (expDigits != 2) {
                    tok.type = TK_ERROR;
                    sprintf(tok.errorMsg, "Line no: %d : Error: Unknown pattern <%s>\n", tok.lineNo, tok.lexeme);
                    printf("%s", tok.errorMsg);
                    return tok;
                }

                tok.type = TK_RNUM;
                tok.value.realVal = (float)atof(tok.lexeme);
                return tok;
            }

            /* Real number without scientific notation */
            tok.type = TK_RNUM;
            tok.value.realVal = (float)atof(tok.lexeme);
            return tok;
        }

        /* Plain integer */
        tok.type = TK_NUM;
        tok.value.intVal = atoi(tok.lexeme);
        return tok;
    }

    /* ---- Single and multi-character operators ---- */
    switch (c) {
        case '[':
            tok.type = TK_SQL;
            strcpy(tok.lexeme, "[");
            return tok;
        case ']':
            tok.type = TK_SQR;
            strcpy(tok.lexeme, "]");
            return tok;
        case ';':
            tok.type = TK_SEM;
            strcpy(tok.lexeme, ";");
            return tok;
        case ':':
            tok.type = TK_COLON;
            strcpy(tok.lexeme, ":");
            return tok;
        case '.':
            tok.type = TK_DOT;
            strcpy(tok.lexeme, ".");
            return tok;
        case ',':
            tok.type = TK_COMMA;
            strcpy(tok.lexeme, ",");
            return tok;
        case '(':
            tok.type = TK_OP;
            strcpy(tok.lexeme, "(");
            return tok;
        case ')':
            tok.type = TK_CL;
            strcpy(tok.lexeme, ")");
            return tok;
        case '+':
            tok.type = TK_PLUS;
            strcpy(tok.lexeme, "+");
            return tok;
        case '-':
            tok.type = TK_MINUS;
            strcpy(tok.lexeme, "-");
            return tok;
        case '*':
            tok.type = TK_MUL;
            strcpy(tok.lexeme, "*");
            return tok;
        case '/':
            tok.type = TK_DIV;
            strcpy(tok.lexeme, "/");
            return tok;
        case '~':
            tok.type = TK_NOT;
            strcpy(tok.lexeme, "~");
            return tok;

        case '<':
            /* Could be <, <=, or <--- */
            if (peekChar(B) == '=') {
                nextChar(B);
                tok.type = TK_LE;
                strcpy(tok.lexeme, "<=");
            } else if (peekChar(B) == '-') {
                nextChar(B); /* first - */
                if (peekChar(B) == '-') {
                    nextChar(B); /* second - */
                    if (peekChar(B) == '-') {
                        nextChar(B); /* third - */
                        tok.type = TK_ASSIGNOP;
                        strcpy(tok.lexeme, "<---");
                    } else {
                        /* <-- is not valid, retract and return < */
                        retract(B); retract(B);
                        tok.type = TK_LT;
                        strcpy(tok.lexeme, "<");
                    }
                } else {
                    retract(B);
                    tok.type = TK_LT;
                    strcpy(tok.lexeme, "<");
                }
            } else {
                tok.type = TK_LT;
                strcpy(tok.lexeme, "<");
            }
            return tok;

        case '>':
            if (peekChar(B) == '=') {
                nextChar(B);
                tok.type = TK_GE;
                strcpy(tok.lexeme, ">=");
            } else {
                tok.type = TK_GT;
                strcpy(tok.lexeme, ">");
            }
            return tok;

        case '=':
            if (peekChar(B) == '=') {
                nextChar(B);
                tok.type = TK_EQ;
                strcpy(tok.lexeme, "==");
            } else {
                tok.type = TK_ERROR;
                strcpy(tok.lexeme, "=");
                sprintf(tok.errorMsg, "Line No %d: Error : Unknown Symbol <%s>\n", tok.lineNo, tok.lexeme);
                printf("%s", tok.errorMsg);
            }
            return tok;

        case '!':
            if (peekChar(B) == '=') {
                nextChar(B);
                tok.type = TK_NE;
                strcpy(tok.lexeme, "!=");
            } else {
                tok.type = TK_ERROR;
                strcpy(tok.lexeme, "!");
                sprintf(tok.errorMsg, "Line No %d: Error : Unknown Symbol <%s>\n", tok.lineNo, tok.lexeme);
                printf("%s", tok.errorMsg);
            }
            return tok;

        case '&':
            if (peekChar(B) == '&') {
                nextChar(B); /* second & */
                if (peekChar(B) == '&') {
                    nextChar(B); /* third & */
                    tok.type = TK_AND;
                    strcpy(tok.lexeme, "&&&");
                } else {
                    tok.type = TK_ERROR;
                    strcpy(tok.lexeme, "&&");
                    sprintf(tok.errorMsg, "Line no: %d : Error: Unknown pattern <%s>\n", tok.lineNo, tok.lexeme);
                    printf("%s", tok.errorMsg);
                }
            } else {
                tok.type = TK_ERROR;
                strcpy(tok.lexeme, "&");
                sprintf(tok.errorMsg, "Line No %d : Error: Unknown Symbol <%s>\n", tok.lineNo, tok.lexeme);
                printf("%s", tok.errorMsg);
            }
            return tok;

        case '@':
            if (peekChar(B) == '@') {
                nextChar(B);
                if (peekChar(B) == '@') {
                    nextChar(B);
                    tok.type = TK_OR;
                    strcpy(tok.lexeme, "@@@");
                } else {
                    tok.type = TK_ERROR;
                    strcpy(tok.lexeme, "@@");
                    sprintf(tok.errorMsg, "Line no: %d : Error: Unknown pattern <%s>\n", tok.lineNo, tok.lexeme);
                    printf("%s", tok.errorMsg);
                }
            } else {
                tok.type = TK_ERROR;
                strcpy(tok.lexeme, "@");
                sprintf(tok.errorMsg, "Line No %d : Error: Unknown Symbol <%s>\n", tok.lineNo, tok.lexeme);
                printf("%s", tok.errorMsg);
            }
            return tok;

        default:
            /* Unknown symbol */
            tok.type = TK_ERROR;
            tok.lexeme[0] = c;
            tok.lexeme[1] = '\0';
            sprintf(tok.errorMsg, "Line No %d : Error: Unknown Symbol <%s>\n", tok.lineNo, tok.lexeme);
            printf("%s", tok.errorMsg);
            return tok;
    }
}

/* ========== removeComments ========== */
void removeComments(char *testcaseFile) {
    FILE *fin = fopen(testcaseFile, "r");

    if (!fin) {
        printf("Error: Cannot open file %s\n", testcaseFile);
        return;
    }

    int c;
    while ((c = fgetc(fin)) != EOF) {
        if (c == '%') {
            /* Skip comment content until newline */
            while (c != '\n' && c != EOF)
                c = fgetc(fin);
            /* Print/write the newline to preserve line numbers */
            if (c == '\n') {
                printf("%c", c);
            }
        } else {
            printf("%c", c);
        }
    }

    fclose(fin);
}