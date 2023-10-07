#include <stdio.h>
#include <malloc.h>
#include "stack/include/stack.h"
#include "stack/include/print.h"
#include "include/processor.h"

const ssize_t SPU_STACK_START_SIZE = 5;
const ssize_t MAX_CODE_SIZE = 10;

unsigned int processor_init(struct Processor* spu_ptr)
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

	spu_ptr->rax = POISON_VALUE;
	spu_ptr->rbx = POISON_VALUE;
	spu_ptr->rcx = POISON_VALUE;
	spu_ptr->rdx = POISON_VALUE;

	spu_ptr->code = (elem_t*) calloc(MAX_CODE_SIZE, sizeof(elem_t));

	spu_ptr->code_capacity = MAX_CODE_SIZE;
	spu_ptr->ip = 0;

	ssize_t ip = 0;


	return return_code;
}


unsigned int processor_dtor(struct Processor* spu_ptr)
{
	unsigned int return_code = NO_ERROR;

	return_code |= stack_dtor(&(spu_ptr->stk));

	spu_ptr->rax = POISON_VALUE;
	spu_ptr->rbx = POISON_VALUE;
	spu_ptr->rcx = POISON_VALUE;
	spu_ptr->rdx = POISON_VALUE;

	free(spu_ptr->code);

	spu_ptr->code = NULL;

	ssize_t ip = POISON_VALUE;

	return return_code;	
}


unsigned int processor_verificator(struct Processor* spu_ptr)
{
	unsigned int return_code = 0;

	return_code |= stack_verificator(&(spu_ptr->stk));

	if (spu_ptr->code == NULL) 		 return_code |= SPU_CODE_IS_NULL;

	if (spu_ptr->code_capacity <= 0) return_code |= INVALID_SPU_CODE_CAPACITY;

	if (spu_ptr->ip < 0)			 return_code |= INVALID_SPU_IP;

	if (spu_ptr->ip >= spu_ptr->code_capacity)      return_code |= SPU_IP_IS_NOT_SMALLER_THAN_CODE;

	return return_code;
}


unsigned int processor_print(struct Processor* spu_ptr, ssize_t print_poison_stack_data_count)
{
	printf("processor spu [%p]\n", spu_ptr);

	printf("{\n");
	printf("\trax = " STACK_ELEM_PRINTF_SPEC "\n", spu_ptr->rax);
	printf("\trbx = " STACK_ELEM_PRINTF_SPEC "\n", spu_ptr->rbx);
	printf("\trcx = " STACK_ELEM_PRINTF_SPEC "\n", spu_ptr->rcx);
	printf("\trdx = " STACK_ELEM_PRINTF_SPEC "\n", spu_ptr->rdx);
	printf("\tcode capacity = %zd\n", spu_ptr->code_capacity);
	printf("\tcode ip       = %zd\n", spu_ptr->ip);
	printf("\tnumber");


	for (ssize_t code_number = 0; code_number < spu_ptr->code_capacity; code_number++)
	{
		printf("%*zd", SPU_CODE_PRINTF_WIDE, code_number);
		putchar(' ');
	}

	putchar('\n');
	printf("\tcode  ");
	

	for (ssize_t code_number = 0; code_number < spu_ptr->code_capacity; code_number++)
	{
		printf(PROCESSOR_CODE_PRINTF_SPEC, SPU_CODE_PRINTF_WIDE, code_number);
		putchar(' ');
	}

	putchar('\n');

	printf("\t      ");
	for (ssize_t code_number = 0; code_number < spu_ptr->code_capacity; code_number++)
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


