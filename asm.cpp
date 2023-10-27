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
static size_t print_command_bytecode(const char* const string, const ssize_t n_string, elem_t* code_arr, ssize_t* code_ip_ptr);

#define IS_COMMAND(command) strncmp(string, command, sizeof(command) - 1) == 0 //не работает с командами, которые являются частями других команд

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


static size_t print_command_bytecode(const char*  string, const ssize_t n_string, elem_t* code_arr, ssize_t* code_ip_ptr, struct Labels* labels_ptr)
{	
	size_t printed_numbers = 0;

	enum arg_types_command argument_type = NO_ARGS;

	while(string[0] == ' ' || string[0] == '\t') string++;

	if ((string[0] == '\n') or (string[0] == '\0'))
	{
		return 0;
	}

	if (string[0] == ':')
	{
		strncpy((labels_ptr->labels)[labels_ptr->scanned_labels], string + 1, LABEL_NAME_SIZE);

		(labels_ptr->labels_pos)[labels_ptr->scanned_labels] = *code_ip_ptr;

		(labels_ptr->scanned_labels)++;

		printf("label <%s> %zd\n", string + 1, *code_ip_ptr);	
	}
	else
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		#define PRINT_CMD_WITH_NUM_ARG(cmd_name, number)															\
																													\
				elem_t arg_number = 0; 																				\
				int is_scanned = sscanf(string + sizeof(#cmd_name), " " STACK_ELEM_PRINTF_SPEC " ", &arg_number);	\
																													\
				if (!is_scanned)																					\
				{																									\
					fprintf(stderr, "string %zu: invalid arg number\n", n_string + 1);								\
				}																									\
																													\
				code_arr[*code_ip_ptr] = arg_number;																\
				(*code_ip_ptr)++;																					\


		#define PRINT_CMD_WITH_REG_ARG(cmd_name, number)																\
					char register_name[REGISTER_NAME_SIZE] = {}; 														\
					int is_scanned = sscanf(string + sizeof(#cmd_name), "%s", register_name);							\
																														\
					if (!is_scanned)																					\
					{																									\
						fprintf(stderr, "string %zu: invalid register\n", n_string + 1);								\
					}																									\
																														\
					code_arr[*code_ip_ptr] = register_name[REGISTER_LETTER_POS] - 'A';									\
					(*code_ip_ptr)++;																					\


		#warning lable and num is the same
		#define PRINT_CMD_WITH_NUM_OR_LABEL_ARG(cmd_name, number)														\
					char label_name[LABEL_NAME_SIZE] = {};		 														\
					int is_scanned = sscanf(string + sizeof(#cmd_name), ":%s", label_name);								\
																														\
					if (!is_scanned)																					\
					{																									\
						fprintf(stderr, "string %zu: invalid label name\n", n_string + 1);								\
					}																									\
																														\
					for (ssize_t n_label = 0; n_label < labels_ptr->scanned_labels ; n_label++)							\
					{																									\
						if (strncmp(label_name, (labels_ptr->labels)[n_label], LABEL_NAME_SIZE) == 0)					\
						{																								\
							code_arr[*code_ip_ptr] = (elem_t)(labels_ptr->labels_pos)[n_label];							\
							break;																						\
						}																								\
					}																									\
																														\
					(*code_ip_ptr)++;																					\


		/////////////////////////////////////////////////////////////////
		#define PRINT_CMD_WITH_ARG(cmd_name, number, arg_type)			\
			switch (arg_type)											\
			{															\
				case NUM_ARG: 											\
				{														\
					PRINT_CMD_WITH_NUM_ARG(cmd_name, number);			\
            		break;												\
				}														\
				case REG_ARG:											\
				{														\
					PRINT_CMD_WITH_REG_ARG(cmd_name, number);			\
					break;												\
				}														\
				case NUM_OR_LABEL_ARG:									\
				{														\
					PRINT_CMD_WITH_NUM_OR_LABEL_ARG(cmd_name, number);	\
					break;												\
				}														\
				case RAM_ARG:											\
				{														\
					PRINT_CMD_WITH_NUM_ARG(cmd_name, number);			\
					break;												\
				}														\
				case RAM_REG_ARG:										\
				{														\
					PRINT_CMD_WITH_REG_ARG(cmd_name, number);			\
					break;												\
				}														\
				case NO_ARGS:											\
				{														\
					break;												\
				}														\
				default: 												\
				{														\
					fprintf(stderr, "invalid argument type\n");			\
				}														\
			}
		/////////////////////////////////////////////////////////////////

		///////////////////////////////////////////////////////////////////////////////////
		#define DEF_CMD(cmd_name, number, arg_type, spu_func)								\
			if (IS_COMMAND(#cmd_name)) 														\
			{																				\
				code_arr[*code_ip_ptr] = number;											\
																							\
				(*code_ip_ptr)++;															\
				argument_type = arg_type;    												\
																							\
                PRINT_CMD_WITH_ARG(cmd_name, number, arg_type)								\
			}																				\
			else   // для следюющего if   													\

		#include "include/commands.h"

		/* else */ fprintf(stderr, "invalid command\n");


		#undef PRINT_CMD_WITH_NUM_OR_LABEL_ARG
		#undef PRINT_CMD_WITH_NUM_ARG
		#undef PRINT_CMD_WITH_REG_ARG
		#undef PRINT_CMD_WITH_ARG
		#undef DEF_CMD
		////////////////////////////////////////////////////////////////////////////////////

	}

	return printed_numbers;
}


static ssize_t print_all_bytecode(const char* const * const text, const ssize_t n_strings,  elem_t* code_arr, struct Labels* labels_ptr)
{
	ssize_t code_ip = 0;

	for (ssize_t n_string = 0; n_string < n_strings; n_string++)
	{
		print_command_bytecode(text[n_string], n_string, code_arr, &code_ip, labels_ptr);
	}	

	return code_ip;	
}


static unsigned int print_labels(const struct Labels* const labels_ptr)
{
	for (ssize_t n_label = 0; n_label < LABELS_SIZE; n_label++)
	{
		printf("position %zd: <%s>\n", (labels_ptr->labels_pos)[n_label],  (labels_ptr->labels)[n_label]);
	}	

	return NO_ERROR;
}


ssize_t assembler(const char* const *  const text, const ssize_t n_strings, elem_t* code_arr)
{
	struct Labels labels;

	//char* labels_buffer[LABELS_SIZE * LABEL_NAME_SIZE] = {};

	#warning one calloc should be
	for (ssize_t n_label = 0; n_label < LABELS_SIZE; n_label++)
	{
		labels.labels[n_label] = (char*) calloc(LABEL_NAME_SIZE, sizeof(char));
	}

	print_all_bytecode(text, n_strings, code_arr, &labels);

	ssize_t code_ip = print_all_bytecode(text, n_strings, code_arr, &labels);

	return code_ip;
}
