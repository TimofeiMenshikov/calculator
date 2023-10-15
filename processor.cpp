#include <stdio.h>
#include <malloc.h>
#include "stack/include/stack.h"
#include "stack/include/print.h"
#include "include/processor.h"
#include "include/text.h"

const ssize_t SPU_STACK_START_SIZE = 5;
const ssize_t MAX_CODE_SIZE = 10;

const char* bin_filename = "txt/code.bin";

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
															   		  		
	return_code |= stack_verificator(&stk);      	 

	spu_ptr->stk = stk;

	for (ssize_t register_number = 0; register_number < REGISTER_COUNT; register_number++)
	{
		(spu_ptr->r_x)[register_number] = POISON_VALUE;
	}

	return_code |= init_spu_code(spu_ptr, filename);

	spu_ptr->ip = 0;

	return return_code;
}


unsigned int processor_dtor(struct Processor* spu_ptr)
{
	unsigned int return_code = NO_ERROR;

	return_code |= stack_dtor(&(spu_ptr->stk));

	for (ssize_t register_number = 0; register_number < REGISTER_COUNT; register_number++)
	{
		(spu_ptr->r_x)[register_number] = POISON_VALUE;
	}

	free(spu_ptr->code);

	spu_ptr->code = NULL;

	spu_ptr->ip = POISON_VALUE;

	return return_code;	
}


unsigned int init_spu_code(struct Processor* spu_ptr, const char* const bin_filename)
{
	elem_t code_size = 0;

	spu_ptr->code = (elem_t*) init_code_from_bin_file(&code_size);
	
	spu_ptr->code_capacity = (ssize_t) code_size;

	return NO_ERROR;
}


unsigned int processor_verificator(const struct Processor* const spu_ptr)
{
	unsigned int return_code = 0;

	return_code |= stack_verificator(&(spu_ptr->stk));

	if (spu_ptr->code == NULL) 		 return_code |= SPU_CODE_IS_NULL;

	if (spu_ptr->code_capacity <= 0) return_code |= INVALID_SPU_CODE_CAPACITY;

	if (spu_ptr->ip < 0)			 return_code |= INVALID_SPU_IP;

	if (spu_ptr->ip >= spu_ptr->code_capacity)      return_code |= SPU_IP_IS_NOT_SMALLER_THAN_CODE;

	return return_code;
}


unsigned int processor_print(const struct Processor* const spu_ptr, const ssize_t print_poison_stack_data_count, const ssize_t print_code_wide)
{
	printf("processor spu [%p]\n", spu_ptr);

	printf("{\n");
	printf("\trax = " STACK_ELEM_PRINTF_SPEC "\n", (spu_ptr->r_x)[RAX]);
	printf("\trbx = " STACK_ELEM_PRINTF_SPEC "\n", (spu_ptr->r_x)[RBX]);
	printf("\trcx = " STACK_ELEM_PRINTF_SPEC "\n", (spu_ptr->r_x)[RCX]);
	printf("\trdx = " STACK_ELEM_PRINTF_SPEC "\n", (spu_ptr->r_x)[RDX]);
	printf("\tcode capacity = %zd\n", spu_ptr->code_capacity);
	printf("\tcode ip       = %zd\n", spu_ptr->ip);
	printf("\tnumber");

	ssize_t left_code_number  = spu_ptr->ip - print_code_wide + 1;
	ssize_t right_code_number = spu_ptr->ip + print_code_wide;


	if (left_code_number < 0) left_code_number = 0;
	if (right_code_number > spu_ptr->code_capacity) right_code_number = spu_ptr->code_capacity;


	for (ssize_t code_number = left_code_number; code_number < right_code_number; code_number++)
	{
		printf("%*d", SPU_CODE_PRINTF_WIDE, code_number);
		putchar(' ');
	}
	#warning print code as fixed width table
	putchar('\n');
	printf("\tcode  ");
	

	for (ssize_t code_number = left_code_number; code_number < right_code_number; code_number++)
	{
		printf(PROCESSOR_CODE_PRINTF_SPEC, (spu_ptr->code)[code_number]);
		putchar(' ');
	}

	putchar('\n');

	printf("\t      ");
	for (ssize_t code_number = left_code_number; code_number < right_code_number; code_number++)
	{
		for (ssize_t space_number = 0; space_number < SPU_CODE_PRINTF_WIDE - 1; space_number++)
		{
			putchar(' ');
		}	

		if (spu_ptr->ip == code_number)
		{
			putchar('^');
			printf("ip = %zd", spu_ptr->ip);
			break;
		}
		else
		{
			putchar(' ');
		}

		putchar(' ');
	} 

	putchar('\n');

	printf("}\n");

	print_data(&(spu_ptr->stk), print_poison_stack_data_count);

	return NO_ERROR;
}


