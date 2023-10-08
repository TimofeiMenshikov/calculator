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


const unsigned int OUT_EXIT_CODE = 1 << 13;
const unsigned int HLT_EXIT_CODE = 1 << 14;

const char* inputfile_name = "txt/for_disasm.txt";


static unsigned int do_push_command(struct Processor* spu_ptr);
static unsigned int do_command(struct Processor* spu_ptr);
static unsigned int run_processor(struct Processor* spu_ptr);

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

	//command_t command = 0; // фиктивная переменная для sscanf чтобы сканировало push_number, как вторую переменную

	//int is_scanned = sscanf(string, " " COMMAND_PRINTF_SPEC " " STACK_ELEM_PRINTF_SPEC " ", &command , &push_number);	
	
	//printf("command: " COMMAND_PRINTF_SPEC "\n", command);

	pop_number = spu_ptr->code[spu_ptr->ip];

	//printf("spu_ptr->ip  %zu \n", spu_ptr->ip);
	//printf("spu_ptr->code[spu_ptr->ip] " STACK_ELEM_PRINTF_SPEC "\n", spu_ptr->code[spu_ptr->ip]);

	spu_ptr->ip++;

	/*if (!is_scanned)
	{
		fprintf(stderr, "string %zu: invalid push number\n", n_string + 1);
	}*/

	unsigned int return_code = stack_pop(&(spu_ptr->stk));

	//printf("(ssize_t) pop_number %zu \n", (ssize_t)pop_number);

	spu_ptr->r_x[(ssize_t) pop_number] =  spu_ptr->stk.last_popped_value;

	//printf("last popped value: " STACK_ELEM_PRINTF_SPEC "\n", spu_ptr->stk.last_popped_value);



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
	//int is_scanned = sscanf(string, "" COMMAND_PRINTF_SPEC "", &command);

	unsigned int return_code = NO_ERROR;

	/*if (!is_scanned)
	{
		fprintf(stderr, "string %zu: invalid command number\n", n_string);
	}*/

	command_t command = spu_ptr->code[spu_ptr->ip];

	spu_ptr->ip++;

	if (command == PUSH) 	return_code |= do_push_command(spu_ptr, spu_ptr->code[spu_ptr->ip]);
	if (command == RPUSH)   return_code |= do_push_command(spu_ptr, spu_ptr->r_x[(ssize_t) spu_ptr->code[spu_ptr->ip]]);
	if (command == POP)		return_code |= do_pop_command(spu_ptr);

	if (command == SQRT) 	return_code |= do_unary_command(&(spu_ptr->stk), do_sqrt_command);
	if (command == ADD)		return_code |= do_bin_command(&(spu_ptr->stk), do_add_command);
	if (command == SUB)		return_code |= do_bin_command(&(spu_ptr->stk), do_sub_command);
	if (command == MUL)		return_code |= do_bin_command(&(spu_ptr->stk), do_mul_command);
	if (command == DIV)		return_code |= do_bin_command(&(spu_ptr->stk), do_div_command);
	if (command == SIN)		return_code |= do_unary_command(&(spu_ptr->stk), do_cos_command);
	if (command == COS)		return_code |= do_unary_command(&(spu_ptr->stk), do_sin_command);
	if (command == IN)      return_code |= do_in_command(&(spu_ptr->stk));

	if (command == OUT)
	{
		return_code |= OUT_EXIT_CODE;
		return return_code;
	} 
	if (command == HLT)	
	{
		return_code |= HLT_EXIT_CODE;
		return return_code;
	}	

	return return_code;
} 	


static unsigned int run_processor(struct Processor* spu_ptr)
{
	unsigned int return_code = 0;

	while ((spu_ptr->code)[spu_ptr->ip] != OUT)
	{
		return_code |= do_command(spu_ptr);

		/*if ((return_code != NO_ERROR) || (return_code != HLT_EXIT_CODE) || (return_code != OUT_EXIT_CODE) || (return_code != HLT_EXIT_CODE | OUT_EXIT_CODE))
		{
			return return_code;
		}

		if ((return_code & HLT_EXIT_CODE != 0) || (return_code & OUT_EXIT_CODE != 0))
		{
			return NO_ERROR;
		}*/

		processor_print(spu_ptr, spu_ptr->stk.capacity, 1000);
	}


	while ((spu_ptr->code)[spu_ptr->ip] != HLT)
	{
		spu_ptr->ip++;
	}

	return return_code;
}


int main()
{
	struct Processor spu;

	processor_init(&spu, inputfile_name);

	//init_spu_code(&spu, inputfile_name);

	//printf("spu_ptr->code_capacity %zd", spu.code_capacity);

	/*for (ssize_t code_number = 0; code_number < spu.code_capacity; code_number++)
	{
		printf("" STACK_ELEM_PRINTF_SPEC "\n", (spu.code)[code_number]);
	}*/

	run_processor(&spu);


	processor_dtor(&spu);
}