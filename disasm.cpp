#include <stdio.h>
#include <assert.h>
#include <sys/stat.h>
#include <malloc.h>
#include "include/printf_debug.h"
#include "include/text.h"
#include "include/asm_codes.h"
#include "stack/include/stack.h"


static void disassembler(const char* const *  const text, const ssize_t n_strings, FILE* outputfile);
static size_t print_push_command(const char* const string, const ssize_t n_string, FILE* outputfile);
static size_t print_command(const char* const string, ssize_t n_string, FILE* outputfile);

const char* inputfile_name = "txt/for_disasm.txt";
const char* outputfile_name = "txt/output.txt";


int main()
{
	ssize_t n_strings = 0;

	char** text = init_text(inputfile_name, &n_strings);

	FILE* outputfile = open_file(outputfile_name, "w");

	disassembler(text, n_strings, outputfile);

	free_text(text);
}


static size_t print_push_command(const char* const string, const ssize_t n_string, FILE* outputfile)																
{																							
	size_t printed_numbers = fprintf(outputfile, "PUSH ");    									
	elem_t push_number = 0;

	command_t command = 0;

	int is_scanned = sscanf(string, " " COMMAND_PRINTF_SPEC " " STACK_ELEM_PRINTF_SPEC " ", &command , &push_number);	
	
	if (!is_scanned)
	{
		fprintf(stderr, "string %zu: invalid push number\n", n_string + 1);
	}

	fprintf(outputfile, "" STACK_ELEM_PRINTF_SPEC "", push_number);

	return printed_numbers;							
}


static size_t print_pop_command(const char* const string, const ssize_t n_string, FILE* outputfile)																
{																							
	size_t printed_numbers = fprintf(outputfile, "POP ");    									
	elem_t pop_number = 0;

	command_t command = 0;

	int is_scanned = sscanf(string, " " COMMAND_PRINTF_SPEC " " STACK_ELEM_PRINTF_SPEC " ", &command , &pop_number);	
	
	if (!is_scanned)
	{
		fprintf(stderr, "string %zu: invalid pop number\n", n_string + 1);
	}

	putc('R', outputfile);
	putc('A' + pop_number, outputfile);
	putc('X', outputfile);

	return printed_numbers;							
}


static size_t print_rpush_command(const char* const string, const ssize_t n_string, FILE* outputfile)																
{																							
	size_t printed_numbers = fprintf(outputfile, "RPUSH ");    									
	elem_t rpush_number = 0;

	command_t command = 0;

	int is_scanned = sscanf(string, " " COMMAND_PRINTF_SPEC " " STACK_ELEM_PRINTF_SPEC " ", &command , &rpush_number);	
	
	if (!is_scanned)
	{
		fprintf(stderr, "string %zu: invalid rpush number\n", n_string + 1);
	}

	putc('R', outputfile);
	putc('A' + rpush_number, outputfile);
	putc('X', outputfile);

	return printed_numbers;							
}


static size_t print_command(const char* const string, ssize_t n_string, FILE* outputfile)                                                              					
{		
	size_t printed_numbers = 0;

	command_t command = 0;

	int is_scanned = sscanf(string, "" COMMAND_PRINTF_SPEC "", &command);

	if (!is_scanned)
	{
		fprintf(stderr, "string %zu: invalid command number\n", n_string);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	#define DEF_CMD(cmd_name, number, asm_func, disasm_func, spu_func) 	\
	if (command == cmd_name) disasm_func								\

	#include "include/commands.h"

	
	/*if (command == PUSH)	printed_numbers = print_push_command(string, n_string, outputfile);
	if (command == SQRT) 	printed_numbers = fprintf(outputfile, "sqrt");
	if (command == POP)		printed_numbers = print_pop_command(string, n_string, outputfile);
	if (command == ADD)		printed_numbers = fprintf(outputfile, "add");
	if (command == SUB)		printed_numbers = fprintf(outputfile, "sub");
	if (command == MUL)		printed_numbers = fprintf(outputfile, "mul");
	if (command == DIV)		printed_numbers = fprintf(outputfile, "div");
	if (command == SIN)		printed_numbers = fprintf(outputfile, "sin");
	if (command == COS)		printed_numbers = fprintf(outputfile, "cos");
	if (command == OUT)		printed_numbers = fprintf(outputfile, "out");
	if (command == HLT)		printed_numbers = fprintf(outputfile, "hlt");
	if (command == IN)		printed_numbers = fprintf(outputfile, "in");
	if (command == RPUSH)   printed_numbers = print_rpush_command(string, n_string, outputfile);
	*/


	#undef DEF_CMD
	////////////////////////////////////////////////////////////////////////////////////////////////////

	return printed_numbers;
}


static void disassembler(const char* const *  const text, const ssize_t n_strings, FILE* outputfile)
{
	ssize_t printed_numbers = 0;

	for (ssize_t n_string = 0; n_string < n_strings; n_string++)
	{
		printed_numbers = print_command(text[n_string], n_string, outputfile);

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