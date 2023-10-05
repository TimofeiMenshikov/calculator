#ifndef STACK_H
#define STACK_H

typedef double elem_t;
#define STACK_ELEM_PRINTF_SPEC "%lf"
#define CANARY_PRINTF_SPEC "%llu"
#define HASH_PRINTF_SPEC "%lld"
typedef unsigned long long canary_t;
typedef long long hash_t;

const elem_t POISON_VALUE = 0;
const ssize_t START_STACK_SIZE = 5;


// define работает со стеком
#define GET_STACK_CALL_INFO()               \
	do                                      \
	{										\
		stk.stk_info.call_line = __LINE__;  \
		stk.stk_info.call_func = __func__;  \
		stk.stk_info.call_file = __FILE__;	\
	} 										\
	while (false); 				


// define работает с указателем на стек
#define GET_STACK_INIT_INFO()				\
    do     									\
    {    									\
	stk_ptr->stk_info.init_line = __LINE__; \
	stk_ptr->stk_info.init_func = __func__; \
	stk_ptr->stk_info.init_file = __FILE__; \
	}										\
	while (false);							


enum error_code
{
	NO_ERROR = 0,
	STACK_POINTER_IS_NULL 			= 1 << 0,
	STACK_DATA_IS_NULL 				= 1 << 1,
	INVALID_STACK_SIZE 				= 1 << 2,
	INVALID_STACK_CAPACITY          = 1 << 3,
	STACK_SIZE_LARGER_THAN_CAPACITY = 1 << 4,
	UNABLE_TO_INCREASE_STACK 		= 1 << 5,
	UNABLE_TO_DECREASE_STACK 		= 1 << 6

	#ifdef CANARY_PROTECTION
	,
	LEFT_CANARY_DATA_DIED 			= 1 << 7,
	RIGHT_CANARY_DATA_DIED 			= 1 << 8,
	LEFT_CANARY_STACK_DIED 			= 1 << 9,
	RIGHT_CANARY_STACK_DIED 		= 1 << 10

	#endif /* CANARY_PROTECTION */

	#ifdef HASH_PROTECTION
	,
	STACK_HASH_IS_WRONG 			= 1 << 11,
	DATA_HASH_IS_WRONG 				= 1 << 12

	#endif /* HASH_PROTECTION */
};

struct Stack_info
{
	int init_line;
	const char* init_func;
	const char* init_file;

	int call_line;
	const char* call_func;
	const char* call_file;
};

struct Stack
{
	#ifdef CANARY_PROTECTION
		canary_t left_canary;
	#endif /* CANARY_PROTECTION */

	elem_t* data;
	ssize_t size;
	ssize_t capacity;

	elem_t last_popped_value; 

	struct Stack_info stk_info;

	#ifdef HASH_PROTECTION 
		hash_t hash_stack;
		hash_t hash_data;
	#endif /* HASH_PROTECTION */

	#ifdef CANARY_PROTECTION
		canary_t right_canary;
	#endif /* CANARY_PROTECTION */
};

unsigned int stack_verificator(const struct Stack* const stk_ptr);

enum error_code stack_init(struct Stack* const stk_ptr, const ssize_t start_size);

unsigned int stack_push(struct Stack* const stk_ptr, const elem_t value);

enum error_code stack_dtor(struct Stack* stk_ptr);

unsigned int stack_pop(struct Stack* stk_ptr);

#endif /* STACK_H */



