/* Group No: 38
 * Members: Amar Kumar(2023A7PS0518P), Krishna Saraf(2023A7PS0606P), Ritik Kumar(2023A7PS0553P)
 *          Shivansh Saxena(2023A7PS0637P), Hitashi Raizada(2023A7PS0500P), Arnav Sahoo(2023A7PS0560P)
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "lexer.h"
#include "parser.h"

int main(int argc, char *argv[])
{
    if(argc<3){
        printf("Usage: %s <testcase.txt> <parsetreeOutFile.txt>\n", argv[0]);
        return 1;
    }

    char *srcFile=argv[1];
    char *outFile=argv[2];
    int choice;

    printf("\n========================================\n");
    printf("           COMPILER PROJECT\n");
    printf("             Group No: 38\n");
    printf("========================================\n");

    Grammar gram=getGrammar();
    FirstAndFollow ff=ComputeFirstAndFollowSets(gram);
    ParseTable ptable=createParseTable(ff, ptable, gram);

    while(1)
    {
        printf("\nSelect Option:\n");
        printf("  0: Exit\n");
        printf("  1: Remove Comments\n");
        printf("  2: Print Token List\n");
        printf("  3: Parse Input Source Code\n");
        printf("  4: Measure Time\n");
        printf("Choice: ");
        scanf("%d", &choice);

        if(choice==0){
            printf("Exiting.\n");
            break;
        }

        else if(choice==1){
            printf("\n--- Comment-Free Source Code ---\n\n");
            removeComments(srcFile);
            printf("\n--- End ---\n");
        }

        else if(choice==2){
            printf("\n--- Token List ---\n\n");

            FILE *f=fopen(srcFile, "r");
            if(!f){
                printf("Error: Cannot open file %s\n", srcFile);
                continue;
            }

            TwinBuffer buf;
            getStream(f, &buf);

            TokenInfo tk;

            while(1){
                tk=getNextToken(&buf);

                if(tk.type==TK_EOF)
                    break;

                if(tk.type==TK_ERROR)
                    continue;

                printf("Line no. %-3d\tLexeme %-20s\tToken %s\n",
                       tk.lineNo,
                       tk.lexeme,
                       tokenTypeToString(tk.type));
            }

            fclose(f);
            printf("\n--- End of Token List ---\n");
        }

        else if(choice==3){
            printf("\n--- Parsing Source Code ---\n");

            parseInputSourceCode(srcFile, ptable, gram);

            printf("\n--- Writing Parse Tree ---\n");
            ParseTree *head=returnhead();
            printParseTree(head, outFile);

            printf("Parse tree written to %s\n", outFile);
        }

        else if(choice==4){
            clock_t begin, finish;
            double ticks, secs;

            begin=clock();

            parseInputSourceCode(srcFile, ptable, gram);

            finish=clock();

            ticks=(double)(finish-begin);
            secs=ticks/CLOCKS_PER_SEC;

            printf("\nTotal CPU Time (clocks): %f\n", ticks);
            printf("Total CPU Time (seconds): %f\n", secs);
        }

        else{
            printf("Invalid choice.\n");
        }
    }

    return 0;
}