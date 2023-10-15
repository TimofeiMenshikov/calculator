#include <stdio.h>
#include <assert.h>
#include <sys/stat.h>
#include <malloc.h>
#include "include/printf_debug.h"
#include "include/text.h"
#include "include/asm.h"
#include "include/asm_codes.h"
#include "stack/include/stack.h"

const char* inputfile_name = "txt/for_asm.txt";
const char* outputfile_name = "txt/for_disasm.txt";
const char* spu_code_filename = "txt/spu_code.txt";

const char* bin_filename = "txt/code.bin";


static size_t print_push_bytecode(const char* const string, const ssize_t n_string, FILE* outputfile);
static size_t print_bytecode(const char* const string, const ssize_t n_string, FILE* outputfile);

#define IS_COMMAND(command) strncmp(string, command, sizeof(command) - 1) == 0

int main()
{
	ssize_t n_strings = 0;

	char** text = init_text(inputfile_name, &n_strings);

	FILE* outputfile = open_file(outputfile_name, "wb");

	assembler(text, n_strings, outputfile);

	

	from_txt_to_bin(outputfile_name, bin_filename);

	

	read_bin_as_chars(bin_filename);

	free_text(text); 
}


static size_t print_push_bytecode(const char* const string, const ssize_t n_string, FILE* outputfile)																
{																							
	size_t printed_numbers = fprintf(outputfile, "%d ", PUSH);    									
	elem_t push_number = 0; 																
	int is_scanned = sscanf(string + sizeof("PUSH"), " " STACK_ELEM_PRINTF_SPEC " ", &push_number);	
	
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
	int is_scanned = sscanf(string + sizeof("POP"), "%s", register_name);	
	
	if (!is_scanned)
	{
		fprintf(stderr, "string %zu: invalid pop register\n", n_string + 1);
	}


	fprintf(outputfile, "%d", register_name[REGISTER_LETTER_POS] - 'A');

	return printed_numbers;							
}


static size_t print_rpush_bytecode(const char* const string, const ssize_t n_string, FILE* outputfile)																
{																							
	size_t printed_numbers = fprintf(outputfile, "%d ", RPUSH);    									
	char register_name[REGISTER_NAME_SIZE] = {}; 																
	int is_scanned = sscanf(string + sizeof("RPUSH"), "%s", register_name);	
	
	if (!is_scanned)
	{
		fprintf(stderr, "string %zu: invalid rpush register\n", n_string + 1);
	}


	fprintf(outputfile, "%d", register_name[REGISTER_LETTER_POS] - 'A');

	return printed_numbers;							
}




static size_t print_bytecode(const char* const string, const ssize_t n_string, FILE* outputfile)
{	
	size_t printed_numbers = 0;

	///////////////////////////////////////////////////////////////////////////////////
	#define DEF_CMD(cmd_name, number, asm_func, disasm_func, spu_func) 	\
		if (IS_COMMAND(#cmd_name)) asm_func								\



	#include "include/commands.h"

	/*if (IS_COMMAND("PUSH"))  printed_numbers = print_push_bytecode(string, n_string, outputfile);								    
	if (IS_COMMAND("sqrt"))  printed_numbers = fprintf(outputfile, "%d ", SQRT);  	
	if (IS_COMMAND("pop"))   printed_numbers = print_pop_bytecode(string, n_string, outputfile);    
	if (IS_COMMAND("add"))   printed_numbers = fprintf(outputfile, "%d ", ADD);    
	if (IS_COMMAND("sub"))   printed_numbers = fprintf(outputfile, "%d ", SUB);    
	if (IS_COMMAND("mul"))   printed_numbers = fprintf(outputfile, "%d ", MUL);    
	if (IS_COMMAND("div"))   printed_numbers = fprintf(outputfile, "%d ", DIV);    
	if (IS_COMMAND("sin"))   printed_numbers = fprintf(outputfile, "%d ", SIN);    
	if (IS_COMMAND("cos"))   printed_numbers = fprintf(outputfile, "%d ", COS);    
	if (IS_COMMAND("OUT"))   printed_numbers = fprintf(outputfile, "%d ", OUT);    
	if (IS_COMMAND("HLT"))   printed_numbers = fprintf(outputfile, "%d ", HLT); 
	if (IS_COMMAND("in"))    printed_numbers = fprintf(outputfile, "%d ", IN); 
	if (IS_COMMAND("rpush")) printed_numbers = print_rpush_bytecode(string, n_string, outputfile);
	*/
	#undef DEF_CMD
	////////////////////////////////////////////////////////////////////////////////////


	return printed_numbers;
}


static unsigned int write_spu_code_to_file(const char* const inputfile_name, const char* const spu_code_filename)
{
	size_t buffer_size = 0;

	char* buffer = init_buffer_from_file(inputfile_name, &buffer_size);

	printf("buffer: %s\n", buffer);
	
	FILE* spu_code_file = open_file(spu_code_filename, "wb");

	printf("spu_code_filename is opened\n");

	char* buffer_copy = buffer;

	elem_t elem_t_number = 0;

	ssize_t scanned_number = 0;

	while (true)
	{
		ssize_t is_scanned = sscanf(buffer, STACK_ELEM_PRINTF_SPEC, &elem_t_number);

		if (is_scanned <= 0)
		{
			break;
		}

		scanned_number++;

		while ((buffer[0] != ' ') && (buffer[0] != '\n') && (buffer[0] != '\0')) buffer++;

		while ((buffer[0] == ' ') || (buffer[0] == '\n') || (buffer[0] == '\0')) buffer++;
		
	}

	elem_t code_elem = 0;

	for (ssize_t code_number = 0; code_number < scanned_number; code_number++)
	{

		ssize_t is_scanned = sscanf(buffer_copy, STACK_ELEM_PRINTF_SPEC, &code_elem);

		fprintf(spu_code_file, STACK_ELEM_PRINTF_SPEC, code_elem);
		printf(STACK_ELEM_PRINTF_SPEC, code_elem);
		putchar(' ');

		while ((buffer_copy[0] != ' ') && (buffer_copy[0] != '\n') && (buffer_copy[0] != '\0'))
		{
			buffer_copy++;
		}

		while ((buffer_copy[0] == ' ') || (buffer_copy[0] == '\n') || (buffer_copy[0]) == '\0')
		{
			buffer_copy++;
		}
	}

	return NO_ERROR;
	

}





void assembler(const char* const *  const text, const ssize_t n_strings, FILE* outputfile)
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
	#warning outputfile should be double*
	fclose(outputfile);	
}








