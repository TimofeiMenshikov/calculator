#ifndef PROCESSOR_H
	#define PROCESSOR_H

	#define PROCESSOR_CODE_PRINTF_SPEC "%6.1lf"

	#include "../stack/include/stack.h"

	const ssize_t SPU_CODE_PRINTF_WIDE = 6;
	const ssize_t REGISTER_COUNT = 4;

	enum register_name
	{
		RAX = 0,
		RBX = 1,
		RCX = 2,
		RDX = 3
	};

	struct Processor 
	{
		struct Stack stk;

		elem_t r_x[REGISTER_COUNT];

		elem_t* code;

		ssize_t code_capacity;

		ssize_t ip;
	};

	unsigned int processor_init(struct Processor* spu_ptr, const char* const filename);
	unsigned int processor_dtor(struct Processor* spu_ptr);
	unsigned int processor_verificator(const struct Processor* const spu_ptr);
	unsigned int processor_print(const struct Processor* const spu_ptr, const ssize_t print_poison_stack_data_count, const ssize_t print_code_wide, const ssize_t n_cols);
	unsigned int init_spu_code(struct Processor* spu_ptr, const char* const filename);


#endif /* PROCESSOR_H */