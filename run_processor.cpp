#include <stdio.h>
#include <assert.h>
#include <sys/stat.h>
#include <malloc.h>
#include <math.h>
#include "include/printf_debug.h"
#include "include/text.h"
#include "include/asm_codes.h"
#include "stack/include/stack.h"
#include "stack/include/print.h"
#include "include/dump.h"
#include "include/processor.h"



const double EPS = 0.0001;

const char* inputfile_name = "txt/for_disasm.txt";


static unsigned int do_push_command(struct Processor* spu_ptr, elem_t push_number);
static unsigned int do_in_command(struct Stack* stk_ptr);
static unsigned int do_command(struct Processor* spu_ptr);
static unsigned int run_processor(struct Processor* spu_ptr);
static unsigned int do_out_command(struct Stack* stk_ptr);

#define start_processor_stack()         				   	   		 \
	struct Stack stk;       								   		 \
															   		 \
	GET_STACK_CALL_INFO();  								   		 \
															   		 \
	enum error_code init_code =  stack_init(&stk, START_STACK_SIZE); \
															   		 \
	if (init_code != NO_ERROR)								    	 \
	{														   		 \
		print_stack_error(init_code);						   		 \
		return init_code;									    	 \
	}														   		 \
															   		 \
															   		 \
	unsigned int error_code = stack_verificator(&stk);         		 \
															   		 \
	print_stack_error(error_code);							   		 \
															    	 \
	print_stack(&stk, START_STACK_SIZE); 


static unsigned int do_pop_command(struct Processor* spu_ptr)
{																																
	elem_t pop_number = 0;

	pop_number = spu_ptr->code[spu_ptr->ip];

	spu_ptr->ip++;

	unsigned int return_code = stack_pop(&(spu_ptr->stk));

	spu_ptr->r_x[(ssize_t) pop_number] =  spu_ptr->stk.last_popped_value;

	if (return_code != NO_ERROR)
	{
		PRINT_ERR_POS();

		print_stack_error(return_code);
	}

	return return_code;							
}


static unsigned int do_push_command(struct Processor* spu_ptr, elem_t push_number)
{
	spu_ptr->ip++;

	unsigned int return_code = stack_push(&(spu_ptr->stk), push_number);

	if (return_code != NO_ERROR)
	{
		PRINT_ERR_POS();

		print_stack_error(return_code);
	}

	return return_code;							
}


static unsigned int do_in_command(struct Stack* stk_ptr)
{																																
	elem_t push_number = 0;

	int is_scanned = scanf(STACK_ELEM_PRINTF_SPEC, &push_number);	
	
	if (!is_scanned)
	{
		fprintf(stderr, "invalid in number\n");
	}

	unsigned int return_code = stack_push(stk_ptr, push_number);

	if (return_code != NO_ERROR)
	{
		PRINT_ERR_POS();

		print_stack_error(return_code);
	}

	return return_code;							
}



static unsigned int do_out_command(struct Stack* stk_ptr)
{																														
	unsigned int return_code = stack_pop(stk_ptr);

	if (return_code != NO_ERROR)
	{
		PRINT_ERR_POS();

		print_stack_error(return_code);
	}

	printf("out: " STACK_ELEM_PRINTF_SPEC "\n", stk_ptr->last_popped_value);

	return return_code;							
}


static elem_t do_add_command(elem_t a, elem_t b)
{
	return a + b;
}

static elem_t do_sub_command(elem_t a, elem_t b)
{
	return a - b;
}

static elem_t do_mul_command(elem_t a, elem_t b)
{
	return a * b;
}

static elem_t do_div_command(elem_t a, elem_t b)
{
	return a / b;
}


static unsigned int do_bin_command(struct Stack* stk_ptr, elem_t  (*command_name)(elem_t a, elem_t b))
{
	unsigned int return_code = NO_ERROR;

	return_code |= stack_pop(stk_ptr);			 			
	print_stack(stk_ptr, stk_ptr->capacity);
	if (return_code != NO_ERROR)				 			
	{											 			
		PRINT_ERR_POS()							 			
												 			
		print_stack_error(return_code);          			
	}											 			
                                                 			
	elem_t pop_number_1 = stk_ptr->last_popped_value; 		
												 			
	return_code |= stack_pop(stk_ptr);			 			
	print_stack(stk_ptr, stk_ptr->capacity);
	if (return_code != NO_ERROR)				 			
	{											 			
		PRINT_ERR_POS()							 			
												 			
		print_stack_error(return_code);			 			
	}											 			
												 			
	elem_t pop_number_2 = stk_ptr->last_popped_value; 		

	return_code |= stack_push(stk_ptr, command_name(pop_number_2, pop_number_1));

	if (return_code != NO_ERROR)				 			
	{											 			
		PRINT_ERR_POS()							 			
												 			
		print_stack_error(return_code);			 			
	}		

	return return_code; 	
}


static elem_t do_cos_command(elem_t a)
{
	return cos(a);
}

static elem_t do_sin_command(elem_t a)
{
	return sin(a);
}


static elem_t do_sqrt_command(elem_t a)
{
	return sqrt(a);
}


static unsigned int do_unary_command(struct Stack* stk_ptr, elem_t (*command_name)(elem_t a))
{
	unsigned int return_code = NO_ERROR;

	return_code |= stack_pop(stk_ptr);			 			
	print_stack(stk_ptr, stk_ptr->capacity);
	if (return_code != NO_ERROR)				 			
	{											 			
		PRINT_ERR_POS()							 			
												 			
		print_stack_error(return_code);          			
	}											 			
                                                 			
	elem_t pop_number_1 = stk_ptr->last_popped_value; 

	if (return_code != NO_ERROR)				 			
	{											 			
		PRINT_ERR_POS()							 			
												 			
		print_stack_error(return_code);			 			
	}

	return_code |= stack_push(stk_ptr, command_name(pop_number_1));

	if (return_code != NO_ERROR)				 			
	{											 			
		PRINT_ERR_POS()							 			
												 			
		print_stack_error(return_code);			 			
	}				

	return return_code;
}


static unsigned int do_command(struct Processor* spu_ptr)
{		
	unsigned int return_code = NO_ERROR;

	command_t command = (command_t) spu_ptr->code[spu_ptr->ip];

	spu_ptr->ip++;

	///////////////////////////////////////////////////////////////////////////////////
	#define DEF_CMD(cmd_name, number, arg_type, asm_func, disasm_func, spu_func) 	\
		if (command == cmd_name) spu_func								\

	#include "include/commands.h"

	#undef DEF_CMD
	//////////////////////////////////////////////////////////////////////////////////

	return return_code;
} 	


static unsigned int run_processor(struct Processor* spu_ptr)
{
	unsigned int return_code = 0;

	while ((spu_ptr->code)[spu_ptr->ip] != HLT)
	{
		return_code |= do_command(spu_ptr);

		processor_print(spu_ptr, spu_ptr->stk.capacity, 100000, 7);
	}

	return return_code;
}


int main()
{
	struct Processor spu;

	processor_init(&spu, inputfile_name);

	run_processor(&spu);

	processor_dtor(&spu);
}