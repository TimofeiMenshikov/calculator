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

#define CHECK_ERROR()					\
	if (return_code != NO_ERROR)		\
	{									\
		PRINT_ERR_POS();				\
										\
		print_stack_error(return_code);	\
	}									\


static unsigned int do_pop_command(struct Processor* spu_ptr)
{																																
	elem_t pop_number = spu_ptr->code[spu_ptr->ip];

	spu_ptr->ip++;

	unsigned int return_code = stack_pop(&(spu_ptr->stk));

	spu_ptr->r_x[(ssize_t) pop_number] =  spu_ptr->stk.last_popped_value;

	CHECK_ERROR();

	return return_code;							
}


static unsigned int do_push_command(struct Processor* spu_ptr, elem_t push_number)
{
	spu_ptr->ip++;

	unsigned int return_code = stack_push(&(spu_ptr->stk), push_number);

	CHECK_ERROR();

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

	CHECK_ERROR();

	return return_code;							
}


static unsigned int do_out_command(struct Stack* stk_ptr)
{																														
	unsigned int return_code = stack_pop(stk_ptr);

	CHECK_ERROR();

	printf("out: " STACK_ELEM_PRINTF_SPEC "\n", stk_ptr->last_popped_value);

	return return_code;							
}


static unsigned int do_ram_push_command(struct Processor* spu_ptr, const elem_t ram_push_number)
{
	spu_ptr->ip++;

	unsigned int return_code = stack_push(&(spu_ptr->stk), (spu_ptr->ram)[(ssize_t) ram_push_number]);

	CHECK_ERROR();

	return return_code;		
}


static unsigned int do_ram_pop_command(struct Processor* spu_ptr, const elem_t ram_pop_number)
{
	spu_ptr->ip++;

	unsigned int return_code = stack_pop(&(spu_ptr->stk));

	spu_ptr->ram[(ssize_t) ram_pop_number] =  spu_ptr->stk.last_popped_value;

	CHECK_ERROR();

	return return_code;			
}


static unsigned int draw_one_cell(const elem_t number)
{
	/*if (number == 0) 	putchar('.');
	else 			 	putchar('*');*/

	printf("%d ", (int)number);

	return NO_ERROR;
}


static unsigned int do_draw_ram_command(const elem_t* const ram, const ssize_t n_cols, const ssize_t n_rows)
{
	ssize_t n_ram = 0;

	unsigned int return_code = NO_ERROR;

	printf("start draw ram command\n");

	for (ssize_t n_row = 0; n_row < n_rows; n_row++)
	{
		for (ssize_t n_col = 0; n_col < n_cols; n_col++)
		{
			return_code |= draw_one_cell(ram[n_ram]);
			n_ram++;
		}

		putchar('\n');
	}

	return return_code;
}



#define do_bin_command(function_value) 							\
{																\
	return_code |= stack_pop(&(spu_ptr->stk));					\
	print_stack(&(spu_ptr->stk), (spu_ptr->stk).capacity);		\
																\
	CHECK_ERROR();												\
                                                 				\
	elem_t last = (spu_ptr->stk).last_popped_value; 			\
												 				\
	return_code |= stack_pop(&(spu_ptr->stk));			 		\
	print_stack(&(spu_ptr->stk), (spu_ptr->stk).capacity);	    \
																\
	CHECK_ERROR();												\
																\
	elem_t penult = ((spu_ptr->stk)).last_popped_value; 		\
																\
	return_code |= stack_push(&(spu_ptr->stk), function_value);	\
																\
	CHECK_ERROR();												\
}



#define do_unary_command(function_value) 						\
{																\
	return_code |= stack_pop(&(spu_ptr->stk));					\
	print_stack(&(spu_ptr->stk), (spu_ptr->stk).capacity);		\
																\
	CHECK_ERROR();												\
                                                 				\
	elem_t last = (spu_ptr->stk).last_popped_value; 			\
																\
																\
	return_code |= stack_push(&(spu_ptr->stk), function_value);	\
																\
	CHECK_ERROR();												\
}



#define DO_IFJMP_COMMAND(bool_function_value) 					\
{																\
	return_code |= stack_pop(&(spu_ptr->stk));					\
	print_stack(&(spu_ptr->stk), (spu_ptr->stk).capacity);		\
																\
	CHECK_ERROR();												\
                                                 				\
	elem_t last = (spu_ptr->stk).last_popped_value; 			\
												 				\
	return_code |= stack_pop(&(spu_ptr->stk));			 		\
	print_stack(&(spu_ptr->stk), (spu_ptr->stk).capacity);	    \
																\
	CHECK_ERROR();												\
																\
	elem_t penult = ((spu_ptr->stk)).last_popped_value; 		\
																\
																\
	if (bool_function_value)									\
	{															\
		return_code |= do_jmp_command(spu_ptr);					\
	}															\
	else                                                        \
	{															\
		(spu_ptr->ip)++;										\
	}															\
}


static unsigned int do_jmp_command(struct Processor* spu_ptr)
{
	spu_ptr->ip = (ssize_t) spu_ptr->code[spu_ptr->ip];

	printf("spu_ptr->ip = %zd\n", spu_ptr->ip);

	return NO_ERROR;
}


static unsigned int do_call_command(struct Processor* spu_ptr)
{
	unsigned int return_code = NO_ERROR;

	return_code |= stack_push(&(spu_ptr->ret_positions), (spu_ptr->ip) + 1);

	return_code |= do_jmp_command(spu_ptr);

	return return_code;
}



static unsigned int do_ret_command(struct Processor* spu_ptr)
{
	unsigned int return_code = NO_ERROR;

	return_code |= stack_pop(&(spu_ptr->ret_positions));

	spu_ptr->ip = (ssize_t) (spu_ptr->ret_positions).last_popped_value;

	return return_code;
}


static unsigned int do_command(struct Processor* spu_ptr)
{		
	unsigned int return_code = NO_ERROR;

	command_t command = (command_t) spu_ptr->code[spu_ptr->ip];

	spu_ptr->ip++;

	#warning read arg here

	///////////////////////////////////////////////////////////////////////////////////
	#define DEF_CMD(cmd_name, number, arg_type, spu_func) 								\
		case cmd_name:																	\
		{																				\
			spu_func																	\
			break;																		\
		}																				\


	switch (command)
	{
		#include "include/commands.h"

		default:
		{
			fprintf(stderr, "can't do invalid command\n");
		}
	}

	
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

		processor_print(spu_ptr, spu_ptr->stk.capacity, 4, 10000, 5);
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