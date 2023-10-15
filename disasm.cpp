#include <stdio.h>
#include <assert.h>
#include <sys/stat.h>
#include <malloc.h>
#include "include/printf_debug.h"
#include "include/text.h"
#include "include/asm_codes.h"
#include "stack/include/stack.h"


static void disassembler(elem_t* code_arr, ssize_t code_size, FILE* outputfile);
static size_t print_push_command(elem_t* code_arr, ssize_t* code_ip_ptr, FILE* outputfile);
static size_t print_command(elem_t* code_arr, ssize_t* code_ip_ptr, FILE* outputfile);

const char* inputfile_name = "txt/for_disasm.txt";
const char* outputfile_name = "txt/output.txt";
const char* bin_filename = "txt/code.bin";


int main()
{
	elem_t code_size = 0;

	elem_t* code_arr = (elem_t*) init_code_from_bin_file(&code_size);

	FILE* outputfile = open_file(outputfile_name, "w");

	disassembler(code_arr, (ssize_t)code_size, outputfile);
}


static size_t print_push_command(elem_t* code_arr, ssize_t* code_ip_ptr, FILE* outputfile)																
{																							
	size_t printed_numbers = fprintf(outputfile, "PUSH ");    									
	
	elem_t push_number = code_arr[*code_ip_ptr];

	(*code_ip_ptr)++;

	fprintf(outputfile, "" STACK_ELEM_PRINTF_SPEC "", push_number);

	return printed_numbers;							
}


static size_t print_pop_command(elem_t* code_arr, ssize_t* code_ip_ptr, FILE* outputfile)																
{																							
	size_t printed_numbers = fprintf(outputfile, "POP ");    									

	elem_t pop_number = code_arr[*code_ip_ptr];

	(*code_ip_ptr)++;

	putc('R', outputfile);
	putc('A' + (int) pop_number, outputfile);
	putc('X', outputfile);

	return printed_numbers;							
}


static size_t print_rpush_command(elem_t* code_arr, ssize_t* code_ip_ptr, FILE* outputfile)																
{																							
	size_t printed_numbers = fprintf(outputfile, "RPUSH ");

	elem_t rpush_number = code_arr[*code_ip_ptr];

	(*code_ip_ptr)++;

	putc('R', outputfile);
	putc('A' + (int) rpush_number, outputfile);
	putc('X', outputfile);

	return printed_numbers;							
}


static size_t print_command(elem_t* code_arr, ssize_t* code_ip_ptr, FILE* outputfile)                                                              					
{		
	size_t printed_numbers = 0;

	command_t command = (command_t) code_arr[*code_ip_ptr];
	(*code_ip_ptr)++;

	///////////////////////////////////////////////////////////////////////////////////////////////////
	#define DEF_CMD(cmd_name, number, asm_func, disasm_func, spu_func) 	\
	if (command == cmd_name) disasm_func								\

	#include "include/commands.h"

	#undef DEF_CMD
	////////////////////////////////////////////////////////////////////////////////////////////////////

	return printed_numbers;
}


static void disassembler(elem_t* code_arr, ssize_t code_size, FILE* outputfile)
{
	ssize_t printed_numbers = 0;

	ssize_t n_string = 0;

	ssize_t code_ip = 0;

	while (code_ip < code_size)
	{
		printed_numbers = print_command(code_arr, &code_ip, outputfile);

		if (printed_numbers > 0) 
		{
			fputs("\n", outputfile);
		}
		else
		{
			fprintf(stderr, "string %zu: invalid command\n", n_string + 1);
		}

		n_string++;
	}
}