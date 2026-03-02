/* Group No: 38
 * Members: Amar Kumar(2023A7PS0518P), Krishna Saraf(2023A7PS0606P), Ritik Kumar(2023A7PS0553P)
 *          Shivansh Saxena(2023A7PS0637P), Hitashi Raizada(2023A7PS0500P), Arnav Sahoo(2023A7PS0560P)
 */
#include "lexer.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

typedef struct {
    const char *word;
    TokenType type;
} KeywordEntry;

static KeywordEntry kwTable[] = {
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

#define NUM_KEYWORDS (sizeof(kwTable)/sizeof(kwTable[0]))

/* checks if word is a keyword */
static TokenType lookupKeyword(const char *word) {
    for(int i=0; i<(int)NUM_KEYWORDS; i++){
        if(strcmp(word, kwTable[i].word)==0)
            return kwTable[i].type;
    }
    return TK_ERROR;
}

const char *tokenTypeToString(TokenType t) {
    switch(t){
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

/* fills one half of the twin buffer from file */
static void loadBuffer(TwinBuffer *B, int which) {
    if(B->done){
        B->loaded[which]=0;
        B->buf[which][0]='\0';
        return;
    }
    size_t rd=fread(B->buf[which], 1, BUFFER_SIZE, B->fp);
    B->loaded[which]=(int)rd;
    B->buf[which][rd]='\0';
    if((int)rd<BUFFER_SIZE)
        B->done=1;
}

FILE *getStream(FILE *fp, TwinBuffer *B) {
    if(!fp){
        printf("Error: Cannot open file!\n");
        return NULL;
    }
    B->fp=fp;
    B->lineNo=1;
    B->done=0;
    B->curBuf=0;
    B->fwd=0;
    B->startBuf=0;
    B->startPos=0;
    B->loaded[0]=0;
    B->loaded[1]=0;

    loadBuffer(B, 0);
    loadBuffer(B, 1);

    return fp;
}

/* reads char at forward pointer and moves ahead */
static char nextChar(TwinBuffer *B) {
    if(B->fwd>=B->loaded[B->curBuf]){
        int other=1-B->curBuf;
        if(B->loaded[other]==0 && B->done)
            return EOF;
        B->curBuf=other;
        B->fwd=0;
        loadBuffer(B, 1-other);
    }
    if(B->fwd>=B->loaded[B->curBuf])
        return EOF;
    char ch=B->buf[B->curBuf][B->fwd];
    B->fwd++;
    return ch;
}

/* looks at current char without moving */
static char peekChar(TwinBuffer *B) {
    if(B->fwd>=B->loaded[B->curBuf]){
        int other=1-B->curBuf;
        if(B->loaded[other]==0 && B->done)
            return EOF;
        return B->buf[other][0];
    }
    return B->buf[B->curBuf][B->fwd];
}

/* goes back one position */
static void retract(TwinBuffer *B) {
    if(B->fwd>0){
        B->fwd--;
    } else {
        int other=1-B->curBuf;
        B->curBuf=other;
        B->fwd=B->loaded[other]-1;
    }
}

static int isBD(char ch) { return ch>='b' && ch<='d'; }
static int is27(char ch) { return ch>='2' && ch<='7'; }

/* main tokenizer function */
TokenInfo getNextToken(TwinBuffer *B) {
    TokenInfo tok;
    tok.type=TK_ERROR;
    tok.lexeme[0]='\0';
    tok.lineNo=B->lineNo;
    tok.value.numVal=0;
    tok.errorMsg[0]='\0';

    char ch;
    int pos;

    while(1){
        ch=nextChar(B);
        if(ch==EOF){
            tok.type=TK_EOF;
            strcpy(tok.lexeme, "EOF");
            tok.lineNo=B->lineNo;
            return tok;
        }
        if(ch=='\n'){
            B->lineNo++;
            continue;
        }
        if(ch==' ' || ch=='\t' || ch=='\r')
            continue;
        break;
    }

    tok.lineNo=B->lineNo;

    if(ch=='%'){
        tok.type=TK_COMMENT;
        strcpy(tok.lexeme, "%");
        while(1){
            char nc=nextChar(B);
            if(nc=='\n'){ B->lineNo++; break; }
            if(nc==EOF) break;
        }
        return tok;
    }

    if(ch=='_'){
        pos=0;
        tok.lexeme[pos++]=ch;
        while(1){
            char nc=peekChar(B);
            if(isalnum(nc) || nc=='_'){
                tok.lexeme[pos++]=nextChar(B);
            } else break;
        }
        tok.lexeme[pos]='\0';
        if(strcmp(tok.lexeme, "_main")==0)
            tok.type=TK_MAIN;
        else
            tok.type=TK_FUNID;
        return tok;
    }

    if(ch=='#'){
        pos=0;
        tok.lexeme[pos++]=ch;
        while(islower(peekChar(B))){
            tok.lexeme[pos++]=nextChar(B);
        }
        tok.lexeme[pos]='\0';
        tok.type=TK_RUID;
        return tok;
    }

    if(islower(ch)){
        pos=0;
        tok.lexeme[pos++]=ch;

        if(isBD(ch) && is27(peekChar(B))){
            tok.lexeme[pos++]=nextChar(B);
            while(isBD(peekChar(B)) || is27(peekChar(B))){
                if(pos<MAX_LEXEME_LEN-1)
                    tok.lexeme[pos++]=nextChar(B);
                else
                    nextChar(B);
            }
            tok.lexeme[pos]='\0';

            if(pos>MAX_ID_LEN){
                tok.type=TK_ERROR;
                sprintf(tok.errorMsg, "Line No %d: Error :Variable Identifier is longer than the prescribed length of 20 characters.\n", tok.lineNo);
                printf("%s", tok.errorMsg);
                return tok;
            }
            tok.type=TK_ID;
            return tok;
        }

        while(islower(peekChar(B))){
            if(pos<MAX_LEXEME_LEN-1)
                tok.lexeme[pos++]=nextChar(B);
            else
                nextChar(B);
        }
        tok.lexeme[pos]='\0';

        TokenType kw=lookupKeyword(tok.lexeme);
        if(kw!=TK_ERROR){
            tok.type=kw;
        } else {
            tok.type=TK_FIELDID;
        }
        return tok;
    }

    if(isdigit(ch)){
        pos=0;
        tok.lexeme[pos++]=ch;

        while(isdigit(peekChar(B))){
            tok.lexeme[pos++]=nextChar(B);
        }
        tok.lexeme[pos]='\0';

        if(peekChar(B)=='.'){
            char dot=nextChar(B);
            tok.lexeme[pos++]=dot;

            int fracCount=0;
            while(isdigit(peekChar(B))){
                tok.lexeme[pos++]=nextChar(B);
                fracCount++;
            }
            tok.lexeme[pos]='\0';

            if(fracCount!=2){
                tok.type=TK_ERROR;
                sprintf(tok.errorMsg, "Line no: %d : Error: Unknown pattern <%s>\n", tok.lineNo, tok.lexeme);
                printf("%s", tok.errorMsg);
                return tok;
            }

            if(peekChar(B)=='E'){
                tok.lexeme[pos++]=nextChar(B);

                char pk=peekChar(B);
                if(pk=='+' || pk=='-'){
                    tok.lexeme[pos++]=nextChar(B);
                }

                int expCount=0;
                while(isdigit(peekChar(B)) && expCount<2){
                    tok.lexeme[pos++]=nextChar(B);
                    expCount++;
                }
                tok.lexeme[pos]='\0';

                if(expCount!=2){
                    tok.type=TK_ERROR;
                    sprintf(tok.errorMsg, "Line no: %d : Error: Unknown pattern <%s>\n", tok.lineNo, tok.lexeme);
                    printf("%s", tok.errorMsg);
                    return tok;
                }

                tok.type=TK_RNUM;
                tok.value.decVal=(float)atof(tok.lexeme);
                return tok;
            }

            tok.type=TK_RNUM;
            tok.value.decVal=(float)atof(tok.lexeme);
            return tok;
        }

        tok.type=TK_NUM;
        tok.value.numVal=atoi(tok.lexeme);
        return tok;
    }

    switch(ch){
        case '[':
            tok.type=TK_SQL;
            strcpy(tok.lexeme, "[");
            return tok;
        case ']':
            tok.type=TK_SQR;
            strcpy(tok.lexeme, "]");
            return tok;
        case ';':
            tok.type=TK_SEM;
            strcpy(tok.lexeme, ";");
            return tok;
        case ':':
            tok.type=TK_COLON;
            strcpy(tok.lexeme, ":");
            return tok;
        case '.':
            tok.type=TK_DOT;
            strcpy(tok.lexeme, ".");
            return tok;
        case ',':
            tok.type=TK_COMMA;
            strcpy(tok.lexeme, ",");
            return tok;
        case '(':
            tok.type=TK_OP;
            strcpy(tok.lexeme, "(");
            return tok;
        case ')':
            tok.type=TK_CL;
            strcpy(tok.lexeme, ")");
            return tok;
        case '+':
            tok.type=TK_PLUS;
            strcpy(tok.lexeme, "+");
            return tok;
        case '-':
            tok.type=TK_MINUS;
            strcpy(tok.lexeme, "-");
            return tok;
        case '*':
            tok.type=TK_MUL;
            strcpy(tok.lexeme, "*");
            return tok;
        case '/':
            tok.type=TK_DIV;
            strcpy(tok.lexeme, "/");
            return tok;
        case '~':
            tok.type=TK_NOT;
            strcpy(tok.lexeme, "~");
            return tok;

        case '<':
            if(peekChar(B)=='='){
                nextChar(B);
                tok.type=TK_LE;
                strcpy(tok.lexeme, "<=");
            } else if(peekChar(B)=='-'){
                nextChar(B);
                if(peekChar(B)=='-'){
                    nextChar(B);
                    if(peekChar(B)=='-'){
                        nextChar(B);
                        tok.type=TK_ASSIGNOP;
                        strcpy(tok.lexeme, "<---");
                    } else {
                        retract(B); retract(B);
                        tok.type=TK_LT;
                        strcpy(tok.lexeme, "<");
                    }
                } else {
                    retract(B);
                    tok.type=TK_LT;
                    strcpy(tok.lexeme, "<");
                }
            } else {
                tok.type=TK_LT;
                strcpy(tok.lexeme, "<");
            }
            return tok;

        case '>':
            if(peekChar(B)=='='){
                nextChar(B);
                tok.type=TK_GE;
                strcpy(tok.lexeme, ">=");
            } else {
                tok.type=TK_GT;
                strcpy(tok.lexeme, ">");
            }
            return tok;

        case '=':
            if(peekChar(B)=='='){
                nextChar(B);
                tok.type=TK_EQ;
                strcpy(tok.lexeme, "==");
            } else {
                tok.type=TK_ERROR;
                strcpy(tok.lexeme, "=");
                sprintf(tok.errorMsg, "Line No %d: Error : Unknown Symbol <%s>\n", tok.lineNo, tok.lexeme);
                printf("%s", tok.errorMsg);
            }
            return tok;

        case '!':
            if(peekChar(B)=='='){
                nextChar(B);
                tok.type=TK_NE;
                strcpy(tok.lexeme, "!=");
            } else {
                tok.type=TK_ERROR;
                strcpy(tok.lexeme, "!");
                sprintf(tok.errorMsg, "Line No %d: Error : Unknown Symbol <%s>\n", tok.lineNo, tok.lexeme);
                printf("%s", tok.errorMsg);
            }
            return tok;

        case '&':
            if(peekChar(B)=='&'){
                nextChar(B);
                if(peekChar(B)=='&'){
                    nextChar(B);
                    tok.type=TK_AND;
                    strcpy(tok.lexeme, "&&&");
                } else {
                    tok.type=TK_ERROR;
                    strcpy(tok.lexeme, "&&");
                    sprintf(tok.errorMsg, "Line no: %d : Error: Unknown pattern <%s>\n", tok.lineNo, tok.lexeme);
                    printf("%s", tok.errorMsg);
                }
            } else {
                tok.type=TK_ERROR;
                strcpy(tok.lexeme, "&");
                sprintf(tok.errorMsg, "Line No %d : Error: Unknown Symbol <%s>\n", tok.lineNo, tok.lexeme);
                printf("%s", tok.errorMsg);
            }
            return tok;

        case '@':
            if(peekChar(B)=='@'){
                nextChar(B);
                if(peekChar(B)=='@'){
                    nextChar(B);
                    tok.type=TK_OR;
                    strcpy(tok.lexeme, "@@@");
                } else {
                    tok.type=TK_ERROR;
                    strcpy(tok.lexeme, "@@");
                    sprintf(tok.errorMsg, "Line no: %d : Error: Unknown pattern <%s>\n", tok.lineNo, tok.lexeme);
                    printf("%s", tok.errorMsg);
                }
            } else {
                tok.type=TK_ERROR;
                strcpy(tok.lexeme, "@");
                sprintf(tok.errorMsg, "Line No %d : Error: Unknown Symbol <%s>\n", tok.lineNo, tok.lexeme);
                printf("%s", tok.errorMsg);
            }
            return tok;

        default:
            tok.type=TK_ERROR;
            tok.lexeme[0]=ch;
            tok.lexeme[1]='\0';
            sprintf(tok.errorMsg, "Line No %d : Error: Unknown Symbol <%s>\n", tok.lineNo, tok.lexeme);
            printf("%s", tok.errorMsg);
            return tok;
    }
}

/* strips out all comment lines from the file */
void removeComments(char *testcaseFile) {
    FILE *fin=fopen(testcaseFile, "r");

    if(!fin){
        printf("Error: Cannot open file %s\n", testcaseFile);
        return;
    }

    int ch;
    while((ch=fgetc(fin))!=EOF){
        if(ch=='%'){
            while(ch!='\n' && ch!=EOF)
                ch=fgetc(fin);
            if(ch=='\n'){
                printf("%c", ch);
            }
        } else {
            printf("%c", ch);
        }
    }

    fclose(fin);
}