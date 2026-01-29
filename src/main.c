/*

	main.c

	Batch No: 38

	Members:

	Amar Kumar 2023A7PS0518P
	Krishna Saraf 2023A7PS0606P
	Ritik Kumar 2023A7PS0553P
	Shivansh Saxena 2023A7PS0637P
	Hitashi Raizada 2023A7PS0500P
	Arnav Sahoo 2023A7PS0560P

*/

/* Standard C library includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer/lexer.h"
#include "parser/parser.h"

/* **************************************************************************** */

/* START Main Function */

int main(int argc, char** argv)
{
	// Check command line arguments
	if (argc < 2)
	{
		printf("Error: Provide input file name as command line argument\n");
		printf("Usage: %s <input_file>\n", argv[0]);
		return 1;
	}

	// Open the input file for reading
	FILE* input_file = fopen(argv[1], "r");
	if (input_file == NULL)
	{
		printf("Error: Cannot open input file '%s'\n", argv[1]);
		return 1;
	}

	// Read the entire file into a buffer
	fseek(input_file, 0, SEEK_END);
	long file_size = ftell(input_file);
	fseek(input_file, 0, SEEK_SET);

	char* source_code = (char*)malloc(file_size + 1);
	if (source_code == NULL)
	{
		printf("Error: Memory allocation failed\n");
		fclose(input_file);
		return 1;
	}

	size_t bytes_read = fread(source_code, 1, file_size, input_file);
	source_code[bytes_read] = '\0'; // Null terminate the string

	fclose(input_file);

	// Initialize the lexer with the source code
	lexer_init(source_code);

	// Tokenize the source code
	// In our implementation, we'll collect tokens in an array
	Token** tokens = NULL;
	size_t token_count = 0;
	size_t token_capacity = 10;

	tokens = (Token**)malloc(token_capacity * sizeof(Token*));
	if (tokens == NULL)
	{
		printf("Error: Memory allocation failed for tokens\n");
		free(source_code);
		return 1;
	}

	// Get tokens one by one using our tokenization function
	Token* current_token;
	while ((current_token = lexer_tokenize()) != NULL)
	{
		// Check if we need to resize the tokens array
		if (token_count >= token_capacity)
		{
			token_capacity *= 2;
			Token** new_tokens = (Token**)realloc(tokens, token_capacity * sizeof(Token*));
			if (new_tokens == NULL)
			{
				printf("Error: Memory allocation failed for token array resize\n");
				// Free previously allocated tokens
				for (size_t i = 0; i < token_count; i++)
				{
					free(tokens[i]->lexeme);
					free(tokens[i]);
				}
				free(tokens);
				free(source_code);
				return 1;
			}
			tokens = new_tokens;
		}

		tokens[token_count++] = current_token;

		// Stop if we reach EOF
		if (current_token->type == TK_EOF)
		{
			break;
		}
	}

	// Create the parser with the collected tokens
	Parser* parser = parser_create(tokens, token_count);
	if (parser == NULL)
	{
		printf("Error: Failed to create parser\n");
		// Free tokens
		for (size_t i = 0; i < token_count; i++)
		{
			free(tokens[i]->lexeme);
			free(tokens[i]);
		}
		free(tokens);
		free(source_code);
		return 1;
	}

	// Parse the tokens
	parser_parse(parser);

	// Print success message
	printf("Compilation completed successfully!\n");

	// Clean up memory
	parser_destroy(parser);
	for (size_t i = 0; i < token_count; i++)
	{
		free(tokens[i]->lexeme);
		free(tokens[i]);
	}
	free(tokens);
	free(source_code);

	return 0;
}

/* END Main Function */

/* **************************************************************************** */