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

static size_t print_push_bytecode(const char* const string, const ssize_t n_string, FILE* outputfile);
static size_t print_bytecode(const char* const string, const ssize_t n_string, FILE* outputfile);
static void assembler(const char* const *  const text, const ssize_t n_strings, FILE* outputfile);

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



static size_t print_bytecode(const char* const string, const ssize_t n_string, FILE* outputfile)
{		
	size_t printed_numbers = 0;

	if (strncmp(string, "push", 4) == 0)  printed_numbers = print_push_bytecode(string, n_string, outputfile);								    
	if (strncmp(string, "sqrt", 4) == 0)  printed_numbers = fprintf(outputfile, "%d ", SQRT);  	
	if (strncmp(string, "pop", 3) == 0)   printed_numbers = fprintf(outputfile, "%d ", POP);    
	if (strncmp(string, "add", 3) == 0)   printed_numbers = fprintf(outputfile, "%d ", ADD);    
	if (strncmp(string, "sub", 3) == 0)   printed_numbers = fprintf(outputfile, "%d ", SUB);    
	if (strncmp(string, "mul", 3) == 0)   printed_numbers = fprintf(outputfile, "%d ", MUL);    
	if (strncmp(string, "div", 3) == 0)   printed_numbers = fprintf(outputfile, "%d ", DIV);    
	if (strncmp(string, "sin", 3) == 0)   printed_numbers = fprintf(outputfile, "%d ", SIN);    
	if (strncmp(string, "cos", 3) == 0)   printed_numbers = fprintf(outputfile, "%d ", COS);    
	if (strncmp(string, "out", 3) == 0)   printed_numbers = fprintf(outputfile, "%d ", OUT);    
	if (strncmp(string, "hlt", 3) == 0)   printed_numbers = fprintf(outputfile, "%d ", HLT); 
	if (strncmp(string, "in", 2) == 0)   printed_numbers = fprintf(outputfile, "%d ", IN); 

	#warning (strncmp(a , b, sizeof(b) - 1) == 0)

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

/*
static unsigned int init_spu_code(struct Processor* spu_ptr, const char* const * const text, const ssize_t n_strings)
{
		
}*/






