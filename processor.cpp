#include <stdio.h>
#include <malloc.h>
#include "stack/include/stack.h"
#include "stack/include/print.h"
#include "include/processor.h"
#include "include/text.h"

const ssize_t SPU_STACK_START_SIZE = 5;
const ssize_t MAX_CODE_SIZE = 10;



unsigned int processor_init(struct Processor* spu_ptr, const char* const filename)
{
	unsigned int return_code = NO_ERROR;

	struct Stack stk;

	GET_STACK_CALL_INFO(); 

	return_code |= stack_init(&stk, SPU_STACK_START_SIZE);

	printf("stack init\n");

	if (return_code != NO_ERROR)								    	 
	{														   		 
		print_stack_error(return_code);						   		 
		return return_code;									    	 
	}														  
															   		  		
	return_code |= stack_verificator(&stk);      

	printf("stack stack_verificator\n"); 		 

	spu_ptr->stk = stk;

	for (ssize_t register_number = 0; register_number < REGISTER_COUNT; register_number++)
	{
		(spu_ptr->r_x)[register_number] = POISON_VALUE;
	}

	printf("before init spu code\n");

	return_code |= init_spu_code(spu_ptr, filename);

	printf("init_spu_code\n");

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


unsigned int init_spu_code(struct Processor* spu_ptr, const char* const filename)
{
	size_t buffer_size = 0;

	char* buffer = init_buffer_from_file(filename, &buffer_size);
	
	/*for (ssize_t char_number = 0; char_number < buffer_size; char_number++)
	{
		printf("%c(%d)\n", buffer[char_number], buffer[char_number]);
	}*/

	char* buffer_copy = buffer;

	//printf("%s\n", buffer);

	elem_t elem_t_number = 0;

	ssize_t scanned_number = 0;

	while (true)
	{
		#warning %n
		ssize_t is_scanned = sscanf(buffer, "" STACK_ELEM_PRINTF_SPEC "", &elem_t_number);

		//printf(" scanned: "STACK_ELEM_PRINTF_SPEC"\n", elem_t_number);

		if (is_scanned <= 0)
		{
			break;
		}

		scanned_number++;

		while ((buffer[0] != ' ') && (buffer[0] != '\n') && (buffer[0] != '\0'))
		{
			buffer++;
		}

		while ((buffer[0] == ' ') || (buffer[0] == '\n') || (buffer[0]) == '\0')
		{
			buffer++;
		}

		//printf("scanned_number: %zd\n", scanned_number);

		//printf("----------------------------\n");

		//printf("%s\n", buffer);

		//printf("-----------------------------\n");
	}

	//printf("scanned_number: %zd\n", scanned_number);	

	spu_ptr->code = (elem_t*) calloc(scanned_number, sizeof(elem_t));

	spu_ptr->code_capacity = scanned_number;

	//printf("spu_ptr->code_capacity %zd\n", spu_ptr->code_capacity);

	for (ssize_t code_number = 0; code_number < scanned_number; code_number++)
	{
		ssize_t is_scanned = sscanf(buffer_copy, STACK_ELEM_PRINTF_SPEC, &((spu_ptr->code)[code_number]));

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
		printf("%*zd", SPU_CODE_PRINTF_WIDE, code_number);
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


