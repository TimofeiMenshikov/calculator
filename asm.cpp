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

static size_t print_push_bytecode(const char* const string, const ssize_t n_string, elem_t* code_arr, ssize_t* code_ip_ptr);
static size_t print_bytecode(const char* const string, const ssize_t n_string, elem_t* code_arr, ssize_t* code_ip_ptr);

#define IS_COMMAND(command) strncmp(string, command, sizeof(command) - 1) == 0

const ssize_t max_code_size = 1000;

int main()
{
	ssize_t n_strings = 0;

	char** text = init_text(inputfile_name, &n_strings);

	FILE* outputfile = open_file(bin_filename, "wb");

	elem_t* code_arr = (double*) calloc(max_code_size, sizeof(elem_t));

	ssize_t last_code_ip = assembler(text, n_strings, code_arr + 1);

	code_arr[0] = (double) last_code_ip;

	printf("last_code_ip: %zd\n", last_code_ip);

	fwrite((void*) code_arr, sizeof(elem_t), last_code_ip + 2, outputfile);

	free(code_arr);

	free_text(text); 
}


static size_t print_push_bytecode(const char* const string, const ssize_t n_string, elem_t* code_arr, ssize_t* code_ip_ptr)																
{																							
	code_arr[*code_ip_ptr] = PUSH;
	(*code_ip_ptr)++;  

	elem_t push_number = 0; 																
	int is_scanned = sscanf(string + sizeof("PUSH"), " " STACK_ELEM_PRINTF_SPEC " ", &push_number);	
	
	if (!is_scanned)
	{
		fprintf(stderr, "string %zu: invalid push number\n", n_string + 1);
	}

	code_arr[*code_ip_ptr] = push_number;
	(*code_ip_ptr)++;

	return NO_ERROR;							
}


static size_t print_pop_bytecode(const char* const string, const ssize_t n_string, elem_t* code_arr, ssize_t* code_ip_ptr)																
{																							
	code_arr[*code_ip_ptr] = POP;
	(*code_ip_ptr)++;  

	char register_name[REGISTER_NAME_SIZE] = {}; 																
	int is_scanned = sscanf(string + sizeof("POP"), "%s", register_name);	
	
	if (!is_scanned)
	{
		fprintf(stderr, "string %zu: invalid pop register\n", n_string + 1);
	}

	code_arr[*code_ip_ptr] = register_name[REGISTER_LETTER_POS] - 'A';
	(*code_ip_ptr)++;

	return NO_ERROR;							
}


static size_t print_rpush_bytecode(const char* const string, const ssize_t n_string, elem_t* code_arr, ssize_t* code_ip_ptr)																
{																							
	code_arr[*code_ip_ptr] = RPUSH;
	(*code_ip_ptr)++;  

	char register_name[REGISTER_NAME_SIZE] = {}; 																
	int is_scanned = sscanf(string + sizeof("RPUSH"), "%s", register_name);	
	
	if (!is_scanned)
	{
		fprintf(stderr, "string %zu: invalid rpush register\n", n_string + 1);
	}

	code_arr[*code_ip_ptr] = register_name[REGISTER_LETTER_POS] - 'A';
	(*code_ip_ptr)++;

	return NO_ERROR;							
}


static size_t print_bytecode(const char* const string, const ssize_t n_string, elem_t* code_arr, ssize_t* code_ip_ptr)
{	
	size_t printed_numbers = 0;

	///////////////////////////////////////////////////////////////////////////////////
	#define DEF_CMD(cmd_name, number, asm_func, disasm_func, spu_func) 	\
		if (IS_COMMAND(#cmd_name)) asm_func								\

	#include "include/commands.h"

	#undef DEF_CMD
	////////////////////////////////////////////////////////////////////////////////////


	return printed_numbers;
}


ssize_t assembler(const char* const *  const text, const ssize_t n_strings, elem_t* code_arr)
{
	ssize_t code_ip = 0;

	for (ssize_t n_string = 0; n_string < n_strings; n_string++)
	{
		print_bytecode(text[n_string], n_string, code_arr, &code_ip);
	}	

	return code_ip;
}








