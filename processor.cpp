#include <stdio.h>
#include <malloc.h>
#include "stack/include/stack.h"
#include "stack/include/print.h"
#include "include/processor.h"
#include "include/text.h"

const ssize_t SPU_STACK_START_SIZE = 5;
const ssize_t MAX_CODE_SIZE = 10;


static void print_one_str(const struct Processor* const spu_ptr, const ssize_t print_code_wide, ssize_t* code_number_ptr);
static void print_ip(const struct Processor* const spu_ptr, const ssize_t n_cols, const ssize_t left_code_number, ssize_t ip_row);

unsigned int processor_init(struct Processor* spu_ptr, const char* const filename)
{
	unsigned int return_code = NO_ERROR;

	struct Stack stk;

	GET_STACK_CALL_INFO(); 

	return_code |= stack_init(&stk, SPU_STACK_START_SIZE);

	if (return_code != NO_ERROR)								    	 
	{														   		 
		print_stack_error(return_code);						   		 
		return return_code;									    	 
	}	

	return_code |= stack_init(&(spu_ptr->ret_positions), SPU_STACK_START_SIZE);

	return_code |= processor_verificator(spu_ptr);

	spu_ptr->stk = stk;      	 

	for (ssize_t register_number = 0; register_number < REGISTER_COUNT; register_number++)
	{
		(spu_ptr->r_x)[register_number] = POISON_VALUE;
	}

	return_code |= init_spu_code(spu_ptr);

	return_code |= init_spu_ram(spu_ptr);

	spu_ptr->ip = 0;

	return return_code;
}


unsigned int processor_dtor(struct Processor* spu_ptr)
{
	unsigned int return_code = NO_ERROR;

	return_code |= stack_dtor(&(spu_ptr->stk));

	return_code |= stack_dtor(&(spu_ptr->ret_positions));

	for (ssize_t register_number = 0; register_number < REGISTER_COUNT; register_number++)
	{
		(spu_ptr->r_x)[register_number] = POISON_VALUE;
	}

	free(spu_ptr->code);



	spu_ptr->code = NULL;

	spu_ptr->ip = POISON_VALUE;

	return return_code;	
}


unsigned int init_spu_ram(struct Processor* spu_ptr)
{
	unsigned int return_code = NO_ERROR;

	for (ssize_t n_elem = 0; n_elem < RAM_SIZE; n_elem++)
	{
		*(spu_ptr->ram + n_elem) = POISON_VALUE;
	}

	return return_code;
}


#warning generic init function, take struct, not file
unsigned int init_spu_code(struct Processor* spu_ptr)
{
	struct Array code = init_code_from_bin_file("txt/code.bin");

	spu_ptr->code = (elem_t*) code.code;
	
	spu_ptr->code_capacity = code.size;

	return NO_ERROR;
}


unsigned int processor_verificator(const struct Processor* const spu_ptr)
{
	unsigned int return_code = 0;

	return_code |= stack_verificator(&(spu_ptr->stk));

	return_code |= stack_verificator(&(spu_ptr->ret_positions));

	if (spu_ptr->code == NULL) 		 return_code |= SPU_CODE_IS_NULL;

	if (spu_ptr->code_capacity <= 0) return_code |= INVALID_SPU_CODE_CAPACITY;

	if (spu_ptr->ip < 0)			 return_code |= INVALID_SPU_IP;

	if (spu_ptr->ip >= spu_ptr->code_capacity)      return_code |= SPU_IP_IS_NOT_SMALLER_THAN_CODE;

	if (spu_ptr->ram == NULL)                       return_code |= SPU_RAM_IS_NULL;

	return return_code;
}


unsigned int processor_print(const struct Processor* const spu_ptr, const ssize_t print_poison_stack_data_count, const ssize_t print_code_wide, const ssize_t n_cols, const ssize_t print_poison_ret_positions_stack_data_count)
{
	printf("processor spu [%p]\n", spu_ptr);

	printf("{\n");
	printf("\trax = " STACK_ELEM_PRINTF_SPEC "\n", (spu_ptr->r_x)[RAX]);
	printf("\trbx = " STACK_ELEM_PRINTF_SPEC "\n", (spu_ptr->r_x)[RBX]);
	printf("\trcx = " STACK_ELEM_PRINTF_SPEC "\n", (spu_ptr->r_x)[RCX]);
	printf("\trdx = " STACK_ELEM_PRINTF_SPEC "\n", (spu_ptr->r_x)[RDX]);
	printf("\tcode capacity = %zd\n", spu_ptr->code_capacity);
	printf("\tcode ip       = %zd\n", spu_ptr->ip);

	ssize_t left_code_number  = spu_ptr->ip - print_code_wide + 1;
	ssize_t right_code_number = spu_ptr->ip + print_code_wide;


	if (left_code_number < 0) left_code_number = 0;
	if (right_code_number > spu_ptr->code_capacity) right_code_number = spu_ptr->code_capacity;

	ssize_t code_number = left_code_number;

	ssize_t rows = (right_code_number - left_code_number) / n_cols;

	ssize_t ip_row = spu_ptr->ip / n_cols;

	if (ip_row < rows)
	{
		for (ssize_t row = 0; row <= ip_row; row++)
		{
			print_one_str(spu_ptr, n_cols, &code_number);
		} 

		print_ip(spu_ptr, n_cols, left_code_number, ip_row);

		for (ssize_t row = ip_row + 1; row < rows; row++)
		{
			print_one_str(spu_ptr, n_cols, &code_number);
		}

		print_one_str(spu_ptr, (right_code_number - left_code_number) % n_cols, &code_number);
	}
	else 
	{
		for (ssize_t row = 0; row < rows; row++)
		{
			print_one_str(spu_ptr, n_cols, &code_number);
		}

		print_one_str(spu_ptr, (right_code_number - left_code_number) % n_cols, &code_number);

		print_ip(spu_ptr, n_cols, left_code_number, ip_row);
	}

	printf("\tstack:\n");
	print_data(&(spu_ptr->stk), print_poison_stack_data_count);

	printf("\treturn positions:\n");
	print_data(&(spu_ptr->ret_positions), print_poison_ret_positions_stack_data_count);

	printf("}\n");

	return NO_ERROR;
}


static void print_ip(const struct Processor* const spu_ptr, const ssize_t n_cols, const ssize_t left_code_number, ssize_t ip_row)
{
	printf("\t           ");

	for (ssize_t row = 0; row < (spu_ptr->ip - left_code_number) % n_cols; row++)
	{
		for (ssize_t space_number = 0; space_number < SPU_CODE_PRINTF_WIDE + 1; space_number++)
		{
			putchar(' ');
		}	
	}

	putchar('^');

	putchar('\n');

	printf("\t           ");

	for (ssize_t row = 0; row < (spu_ptr->ip - left_code_number) % n_cols; row++)
	{
		for (ssize_t space_number = 0; space_number < SPU_CODE_PRINTF_WIDE + 1; space_number++)
		{
			putchar(' ');
		}	
	}

	printf("ip = %zd", spu_ptr->ip);

	putchar('\n');
}

static void print_one_str(const struct Processor* const spu_ptr, const ssize_t n_cols, ssize_t*  code_number_ptr)
{
	printf("\tnumber");
	for (ssize_t col = 0; col < n_cols; col++)
	{
		printf("%*d ", SPU_CODE_PRINTF_WIDE, *code_number_ptr);
		(*code_number_ptr)++;
	}

	putchar('\n');
	printf("\tcode  ");

	*code_number_ptr -= n_cols;

	for (ssize_t col = 0; col < n_cols; col++)
	{
		printf(PROCESSOR_CODE_PRINTF_SPEC, (spu_ptr->code)[*code_number_ptr]);
		putchar(' ');
		(*code_number_ptr)++;
	}

	putchar('\n');
}
