#ifndef PROCESSOR_H
	#define PROCESSOR_H

	#define PROCESSOR_CODE_PRINTF_SPEC "%*.1lf"

	#include "../stack/include/stack.h"

	const ssize_t SPU_CODE_PRINTF_WIDE = 6;

	struct Processor 
	{
		struct Stack stk;

		elem_t rax;
		elem_t rbx;
		elem_t rcx;
		elem_t rdx;

		elem_t* code;

		ssize_t code_capacity;

		ssize_t ip;
	};

	unsigned int processor_init(struct Processor* spu_ptr);
	unsigned int processor_dtor(struct Processor* spu_ptr);
	unsigned int processor_verificator(struct Processor* spu_ptr);
	unsigned int processor_print(struct Processor* spu_ptr, ssize_t print_poison_stack_data_count);

#endif /* PROCESSOR_H */