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

	ssize_t last_code_ip = assembler(text, n_strings, code_arr + 1); // в массиве кода первый элемент - размер массива с кодом 

	code_arr[0] = (double) last_code_ip;

	fwrite((void*) code_arr, sizeof(elem_t), last_code_ip + 2, outputfile);

	free(code_arr);

	free_text(text); 
}


static size_t print_bytecode(const char* const string, const ssize_t n_string, elem_t* code_arr, ssize_t* code_ip_ptr)
{	
	size_t printed_numbers = 0;

	enum arg_types_command argument_type = NO_ARGS;

	///////////////////////////////////////////////////////////////////////////////////
	#define DEF_CMD(cmd_name, number, arg_type, disasm_func, spu_func)									\
		if (IS_COMMAND(#cmd_name)) 																					\
		{																											\
			code_arr[*code_ip_ptr] = number;																		\
																													\
			(*code_ip_ptr)++;																						\
			argument_type = arg_type;    																			\
																													\
			if (arg_type == NUM_ARG)																				\
			{																										\
				elem_t arg_number = 0; 																				\
				int is_scanned = sscanf(string + sizeof(#cmd_name), " " STACK_ELEM_PRINTF_SPEC " ", &arg_number);	\
																													\
				if (!is_scanned)																					\
				{																									\
					fprintf(stderr, "string %zu: invalid push number\n", n_string + 1);								\
				}																									\
																													\
				code_arr[*code_ip_ptr] = arg_number;																\
				(*code_ip_ptr)++;																					\
			}																										\
																													\
			if (arg_type == REG_ARG)																				\
			{																										\
				char register_name[REGISTER_NAME_SIZE] = {}; 														\
				int is_scanned = sscanf(string + sizeof(#cmd_name), "%s", register_name);							\
																													\
				if (!is_scanned)																					\
				{																									\
					fprintf(stderr, "string %zu: invalid pop register\n", n_string + 1);							\
				}																									\
																													\
				code_arr[*code_ip_ptr] = register_name[REGISTER_LETTER_POS] - 'A';									\
				(*code_ip_ptr)++;																					\
			}																										\
																													\
		}																											\




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








