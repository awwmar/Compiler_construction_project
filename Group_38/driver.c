/*
	driver.c

	Batch No: 38

	Members:
	Amar Kumar 2023A7PS0518P
	Krishna Saraf 2023A7PS0606P
	Ritik Kumar 2023A7PS0553P
	Shivansh Saxena 2023A7PS0637P
	Hitashi Raizada 2023A7PS0500P
	Arnav Sahoo 2023A7PS0560P
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "lexer.h"
#include "parser.h"

int main(int argc, char *argv[])
{
	if (argc < 3) {
		printf("Usage: %s <testcase.txt> <parsetreeOutFile.txt>\n", argv[0]);
		return 1;
	}

	char *testcaseFile = argv[1];
	char *outFile = argv[2];
	(void)outFile; /* Parse tree output file - used in Stage 2 */
	int choice;

	printf("\n========================================\n");
	printf("  COMPILER PROJECT - Stage 1\n");
	printf("  Batch No: 38\n");
	printf("========================================\n");
	printf("Implementation Status:\n");
	printf("  (b) Only Lexical analyzer module developed\n");
	printf("========================================\n");

	while (1) {
		printf("\nSelect Option:\n");
		printf("  0: Exit\n");
		printf("  1: Remove Comments (print comment-free code)\n");
		printf("  2: Print Token List (lexical analysis)\n");
		printf("  3: Parse Input (not yet implemented)\n");
		printf("  4: Measure Time\n");
		printf("Choice: ");
		scanf("%d", &choice);

		if (choice == 0) {
			printf("Exiting.\n");
			break;
		}

		if (choice == 1) {
			printf("\n--- Comment-Free Source Code ---\n\n");
			removeComments(testcaseFile, "cleanOutput.txt");
			printf("\n--- End ---\n");
		}
		else if (choice == 2) {
			printf("\n--- Token List ---\n");
			printf("%-12s %-25s %s\n", "Line no.", "Lexeme", "Token");
			printf("-----------------------------------------------------\n");

			FILE *fp = fopen(testcaseFile, "r");
			if (!fp) {
				printf("Error: Cannot open file %s\n", testcaseFile);
				continue;
			}

			FILE *outfp = fopen("output.txt", "w");

			TwinBuffer B;
			getStream(fp, &B);

			TokenInfo tok;
			do {
				tok = getNextToken(&B);
				if (tok.type == TK_EOF) break;
				if (tok.type == TK_ERROR) {
					if (outfp && tok.errorMsg[0] != '\0')
						fprintf(outfp, "%s", tok.errorMsg);
					continue;
				}
				printf("Line no. %d\t Lexeme %-20s\tToken %s\n",
					tok.lineNo, tok.lexeme, tokenTypeToString(tok.type));
				if (outfp)
					fprintf(outfp, "Line no. %d\t Lexeme %-20s\tToken %s\n",
						tok.lineNo, tok.lexeme, tokenTypeToString(tok.type));
			} while (1);

			fclose(fp);
			if (outfp) fclose(outfp);
			printf("--- End of Token List ---\n");
			printf("Token list also written to output.txt\n");
		}
		else if (choice == 3) {
			printf("\nParser not yet implemented in Stage 1.\n");
		}
		else if (choice == 4) {
			clock_t start_time, end_time;
			double total_CPU_time, total_CPU_time_in_seconds;

			start_time = clock();

			FILE *fp = fopen(testcaseFile, "r");
			if (fp) {
				TwinBuffer B;
				getStream(fp, &B);
				TokenInfo tok;
				do {
					tok = getNextToken(&B);
				} while (tok.type != TK_EOF);
				fclose(fp);
			}

			end_time = clock();
			total_CPU_time = (double)(end_time - start_time);
			total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;

			printf("\nTotal CPU Time (clocks): %f\n", total_CPU_time);
			printf("Total CPU Time (seconds): %f\n", total_CPU_time_in_seconds);
		}
		else {
			printf("Invalid choice.\n");
		}
	}

	return 0;
}
