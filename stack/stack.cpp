#include <stdio.h>
#include <malloc.h>
#include <assert.h>

#include "include/stack.h"
#include "include/print.h"
#include "include/canary.h"
#include "include/hash.h"

const ssize_t increase_coef = 2;
const ssize_t decrease_coef = 2;

static unsigned int increase_stack(struct Stack* const stk_ptr);
static unsigned int decrease_stack(struct Stack* const stk_ptr);

static unsigned int increase_stack(struct Stack* const stk_ptr) 
{

	elem_t* old_data_ptr = stk_ptr->data;

	stk_ptr->data = (elem_t*)alloc_canary_data(stk_ptr->capacity, stk_ptr->capacity * increase_coef, stk_ptr->data);

	if (stk_ptr->data == NULL)
	{
		stk_ptr->data = old_data_ptr;
		PRINT_STACK_ERR("unable to increase stack\n");

		return UNABLE_TO_INCREASE_STACK;
	}

	stk_ptr->capacity *= increase_coef;

	for (ssize_t elem_number = stk_ptr->capacity / 2; elem_number < stk_ptr->capacity; elem_number++)
	{
		stk_ptr->data[elem_number] = POISON_VALUE;
	}

	return NO_ERROR;
}


static unsigned int decrease_stack(struct Stack* const stk_ptr)
{
	if (stk_ptr->capacity / decrease_coef < stk_ptr->size)
	{
		PRINT_STACK_ERR("unable to decrease stack, size bigger than capacity to be decreased\n");

		return UNABLE_TO_DECREASE_STACK | STACK_SIZE_LARGER_THAN_CAPACITY;
	}
	
	elem_t* old_data_ptr = stk_ptr->data;

	stk_ptr->data = (elem_t*)alloc_canary_data(stk_ptr->capacity, stk_ptr->capacity / decrease_coef, stk_ptr->data);

	if ((stk_ptr->data) == NULL)
	{
		stk_ptr->data = old_data_ptr;
		PRINT_STACK_ERR("unable to decrease stack\n");

		return UNABLE_TO_DECREASE_STACK;
	}	

	stk_ptr->capacity /= decrease_coef;

	return NO_ERROR;
}


unsigned int stack_push(struct Stack* const stk_ptr, const elem_t value)
{
	unsigned int return_code = stack_verificator(stk_ptr);

	if (return_code != 0)
	{
		PRINT_STACK_ERR("unable to push, invalid stack\n");

		print_stack_error(return_code);

		return return_code;
	}

	if (stk_ptr->size == stk_ptr->capacity)
	{
		return_code |= increase_stack(stk_ptr);

		if (return_code != NO_ERROR)
		{
			print_stack_error(return_code);
			return return_code;
		}
	}

	stk_ptr->data[stk_ptr->size] = value;

	stk_ptr->size++;

	#ifdef HASH_PROTECTION

		stk_ptr->hash_stack = calc_hash_stack(*stk_ptr);
		stk_ptr->hash_data = calc_hash_stack_data(stk_ptr);

	#endif /* HASH_PROTECTION */

	return NO_ERROR;
}


unsigned int stack_pop(struct Stack* const stk_ptr)
{
	unsigned int return_code = stack_verificator(stk_ptr);

	if (return_code != 0)
	{
		PRINT_STACK_ERR("unable to pop, invalid stack\n");

		print_stack_error(return_code);

		return return_code;
	}

	if (stk_ptr->size == 0)
	{
		PRINT_STACK_ERR("unable to pop, empty stack\n");

		return INVALID_STACK_SIZE;
	}

	ssize_t stack_decrease_lag = stk_ptr->capacity / (increase_coef * decrease_coef); 


	if (stack_decrease_lag > stk_ptr->size) 
	{
		return_code |= decrease_stack(stk_ptr);

		if (return_code != NO_ERROR)
		{
			PRINT_STACK_ERR("unable to decrease stack, stack works with the previos size\n");
		}
	}

	stk_ptr->last_popped_value = stk_ptr->data[stk_ptr->size - 1];

	stk_ptr->data[stk_ptr->size - 1] = POISON_VALUE;

	stk_ptr->size--;

	#ifdef HASH_PROTECTION

		stk_ptr->hash_stack = calc_hash_stack(*stk_ptr);
		stk_ptr->hash_data = calc_hash_stack_data(stk_ptr);

	#endif /* HASH_PROTECTION */

	return return_code;
}


enum error_code stack_init(struct Stack* const stk_ptr, const ssize_t start_size)
{
	STDOUT_PRINT(printf("start stack init\n"));

	if (start_size < 0)
	{
		return INVALID_STACK_CAPACITY;
	}

	stk_ptr->capacity = start_size;

	stk_ptr->last_popped_value = POISON_VALUE;

	stk_ptr->size = 0;

	#ifdef CANARY_PROTECTION

		stk_ptr->left_canary = CANARY_LEFT_CONSTANT;

		stk_ptr->right_canary = CANARY_RIGHT_CONSTANT;

	#endif /* CANARY_PROTECTION */

	stk_ptr->data = (elem_t*) new_canary_data(stk_ptr->capacity);

	if ((stk_ptr->data) == NULL)
	{
		return STACK_DATA_IS_NULL;
	}

	#ifdef HASH_PROTECTION

		stk_ptr->hash_stack =  calc_hash_stack(*stk_ptr);

		stk_ptr->hash_data  =  calc_hash_stack_data(stk_ptr);

	#endif /* HASH_PROTECTION */

	STDOUT_PRINT(printf("calloc data\n"));

	GET_STACK_INIT_INFO(); 

	return NO_ERROR;
}


enum error_code stack_dtor(struct Stack* stk_ptr)
{
	stk_ptr->capacity = -1;
	stk_ptr->size = -1;

	STDOUT_PRINT(printf("nulled size and capacity\n"));

	free_stack_data(stk_ptr->data);

	stk_ptr->data = 0;

	STDOUT_PRINT(printf("nulled and free data\n"));

	stk_ptr = 0;

	STDOUT_PRINT(printf("nulled stack pointer\n"));

	return NO_ERROR;
}


unsigned int stack_verificator(const struct Stack* const stk_ptr) 
{
	unsigned int return_code = 0;

	if (stk_ptr == NULL)
	{
		return_code |= STACK_POINTER_IS_NULL;

		return return_code;
	}

	if (stk_ptr->data == NULL)    			return_code |= STACK_DATA_IS_NULL;

	if (stk_ptr->size < 0) 					return_code |= INVALID_STACK_SIZE;

	if (stk_ptr->capacity < 0)              return_code |= INVALID_STACK_CAPACITY;

	if (stk_ptr->size > stk_ptr->capacity)  return_code |= STACK_SIZE_LARGER_THAN_CAPACITY;
	

	#ifdef CANARY_PROTECTION

		if (*get_left_canary_ptr(stk_ptr->data)  != CANARY_LEFT_CONSTANT)  					  return_code |= LEFT_CANARY_DATA_DIED;

		if (*get_right_canary_ptr(stk_ptr->data, stk_ptr->capacity) != CANARY_RIGHT_CONSTANT) return_code |= RIGHT_CANARY_DATA_DIED; 

		if (stk_ptr->left_canary != CANARY_LEFT_CONSTANT)        							  return_code |= LEFT_CANARY_STACK_DIED;

		if (stk_ptr->right_canary != CANARY_RIGHT_CONSTANT)    								  return_code |= RIGHT_CANARY_STACK_DIED;

	#endif /* CANARY_PROTECTION */

	#ifdef HASH_PROTECTION

		if (stk_ptr->hash_stack != calc_hash_stack(*stk_ptr))  		return_code |= STACK_HASH_IS_WRONG;

		if (stk_ptr->hash_data  != calc_hash_stack_data(stk_ptr))   return_code |= DATA_HASH_IS_WRONG;

	#endif /* HASH_PROTECTION */

	return return_code;
}

