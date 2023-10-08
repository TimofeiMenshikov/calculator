#include <stdio.h>
#include <assert.h>
#include <sys/stat.h>
#include <malloc.h>
#include "include/printf_debug.h"
#include "include/text.h"
#include "include/asm_codes.h"
#include "stack/include/stack.h"

const char* inputfile_name = "txt/for_asm.txt";
const char* outputfile_name = "txt/for_disasm.txt";

const ssize_t REGISTER_NAME_SIZE = 3;
const ssize_t REGISTER_LETTER_POS = 1; // местонахождение буквы, имеющей порядковый смысл напр: rAx, rBx, rCx

static size_t print_push_bytecode(const char* const string, const ssize_t n_string, FILE* outputfile);
static size_t print_bytecode(const char* const string, const ssize_t n_string, FILE* outputfile);
static void assembler(const char* const *  const text, const ssize_t n_strings, FILE* outputfile);

#define IS_COMMAND(command) strncmp(string, command, sizeof(command) - 1) == 0


int main()
{
	ssize_t n_strings = 0;

	char** text = init_text(inputfile_name, &n_strings);


	FILE* outputfile = open_file(outputfile_name, "w");

	assembler(text, n_strings, outputfile);

	free_text(text);
}


static size_t print_push_bytecode(const char* const string, const ssize_t n_string, FILE* outputfile)																
{																							
	size_t printed_numbers = fprintf(outputfile, "%d ", PUSH);    									
	elem_t push_number = 0; 																
	int is_scanned = sscanf(string + sizeof("push"), " " STACK_ELEM_PRINTF_SPEC " ", &push_number);	
	
	if (!is_scanned)
	{
		fprintf(stderr, "string %zu: invalid push number\n", n_string + 1);
	}

	fprintf(outputfile, "" STACK_ELEM_PRINTF_SPEC "", push_number);

	return printed_numbers;							
}


static size_t print_pop_bytecode(const char* const string, const ssize_t n_string, FILE* outputfile)																
{																							
	size_t printed_numbers = fprintf(outputfile, "%d ", POP);    									
	char register_name[REGISTER_NAME_SIZE] = {}; 																
	int is_scanned = sscanf(string + sizeof("pop"), "%s", register_name);	
	
	if (!is_scanned)
	{
		fprintf(stderr, "string %zu: invalid pop register\n", n_string + 1);
	}


	fprintf(outputfile, "%d", register_name[REGISTER_LETTER_POS] - 'a');

	return printed_numbers;							
}


static size_t print_rpush_bytecode(const char* const string, const ssize_t n_string, FILE* outputfile)																
{																							
	size_t printed_numbers = fprintf(outputfile, "%d ", RPUSH);    									
	char register_name[REGISTER_NAME_SIZE] = {}; 																
	int is_scanned = sscanf(string + sizeof("rpush"), "%s", register_name);	
	
	if (!is_scanned)
	{
		fprintf(stderr, "string %zu: invalid rpush register\n", n_string + 1);
	}


	fprintf(outputfile, "%d", register_name[REGISTER_LETTER_POS] - 'a');

	return printed_numbers;							
}




static size_t print_bytecode(const char* const string, const ssize_t n_string, FILE* outputfile)
{		
	size_t printed_numbers = 0;

	if (IS_COMMAND("push"))  printed_numbers = print_push_bytecode(string, n_string, outputfile);								    
	if (IS_COMMAND("sqrt"))  printed_numbers = fprintf(outputfile, "%d ", SQRT);  	
	if (IS_COMMAND("pop"))   printed_numbers = print_pop_bytecode(string, n_string, outputfile);    
	if (IS_COMMAND("add"))   printed_numbers = fprintf(outputfile, "%d ", ADD);    
	if (IS_COMMAND("sub"))   printed_numbers = fprintf(outputfile, "%d ", SUB);    
	if (IS_COMMAND("mul"))   printed_numbers = fprintf(outputfile, "%d ", MUL);    
	if (IS_COMMAND("div"))   printed_numbers = fprintf(outputfile, "%d ", DIV);    
	if (IS_COMMAND("sin"))   printed_numbers = fprintf(outputfile, "%d ", SIN);    
	if (IS_COMMAND("cos"))   printed_numbers = fprintf(outputfile, "%d ", COS);    
	if (IS_COMMAND("out"))   printed_numbers = fprintf(outputfile, "%d ", OUT);    
	if (IS_COMMAND("hlt"))   printed_numbers = fprintf(outputfile, "%d ", HLT); 
	if (IS_COMMAND("in"))    printed_numbers = fprintf(outputfile, "%d ", IN); 
	if (IS_COMMAND("rpush")) printed_numbers = print_rpush_bytecode(string, n_string, outputfile);

	return printed_numbers;
}



static void assembler(const char* const *  const text, const ssize_t n_strings, FILE* outputfile)
{
	ssize_t printed_numbers = 0;

	for (ssize_t n_string = 0; n_string < n_strings; n_string++)
	{
		printed_numbers = print_bytecode(text[n_string], n_string, outputfile);

		if (printed_numbers > 0) 
		{
			fputs("\n", outputfile);
		}
		else
		{
			fprintf(stderr, "string %zu: invalid command\n", n_string + 1);
		}
	}
}








