#include <stdio.h>
#include <assert.h>
#include "include/print.h"
#include "include/stack.h"
#include "include/canary.h"


unsigned int print_stack(const Stack* const stk_ptr, ssize_t print_poison_data_count)
{
	printf("stack stk [%p]\n", stk_ptr);

	printf("{\n");
	printf("\tstack information\n");

	printf("\t{\n");
	printf("\t\twhere init:\n");
	printf("\t\t  line: %d\n", stk_ptr->stk_info.init_line);
	printf("\t\t  func: %s\n", stk_ptr->stk_info.init_func);
	printf("\t\t  file: %s\n", stk_ptr->stk_info.init_file);

	putchar('\n');

	printf("\t\twhere called:\n");
	printf("\t\t  line: %d\n", stk_ptr->stk_info.call_line);
	printf("\t\t  func: %s\n", stk_ptr->stk_info.call_func);
	printf("\t\t  file: %s\n", stk_ptr->stk_info.call_file);
	
	printf("\t}\n");

	printf("\tsize     = %zd\n", stk_ptr->size);
	printf("\tcapacity = %zd\n", stk_ptr->capacity);

	#ifdef HASH_PROTECTION

		printf("\tstack  hash = " HASH_PRINTF_SPEC "\n", stk_ptr->hash_stack);      
		printf("\tdata   hash = " HASH_PRINTF_SPEC "\n", stk_ptr->hash_data); 

	#endif /* HASH_PROTECTION */

	#ifdef CANARY_PROTECTION

		printf("\tstack canary left  = " CANARY_PRINTF_SPEC "\n", stk_ptr->left_canary);   
		printf("\tstack canary right = " CANARY_PRINTF_SPEC "\n", stk_ptr->right_canary);   

	#endif /* CANARY_PROTECTION */ 

	printf("\tlast popped value = " STACK_ELEM_PRINTF_SPEC "\n", stk_ptr->last_popped_value); 

	unsigned int return_code = print_data(stk_ptr, print_poison_data_count);

	printf("}\n");

	return return_code;
}


unsigned int print_data(const struct Stack* const stk_ptr, ssize_t print_poison_data_count)
{
	unsigned int return_code = stack_verificator(stk_ptr);

	if (return_code != 0)
	{
		PRINT_STACK_ERR("stack is not work correct\n");

		print_stack_error(return_code);

		return return_code;
	}


	printf("\tdata [%p]\n", stk_ptr->data);
	printf("\t{\n");

	#ifdef CANARY_PROTECTION

		printf("\t\tleft canary:  " CANARY_PRINTF_SPEC "\n", *(get_left_canary_ptr(stk_ptr->data)));      						
		printf("\t\tright canary: " CANARY_PRINTF_SPEC "\n", *(get_right_canary_ptr(stk_ptr->data, stk_ptr->capacity)));		

	#endif /* CANARY_PROTECTION */


	if (print_poison_data_count > (stk_ptr->capacity - stk_ptr->size)) // print_poison_data_count - сколько элементов, не находящихся в стеке нужно вывести. Если таких элементов меньше, то выводится весь массив
	{
		print_poison_data_count = stk_ptr->capacity - stk_ptr->size;
	}

	assert((print_poison_data_count + stk_ptr->size) <= stk_ptr->capacity);

	for (ssize_t element_number = 0; element_number < stk_ptr->size; element_number++)
	{
		printf("\t\t*[%zd] = %d\n", element_number, stk_ptr->data[element_number]);
	}

	for (ssize_t element_number = stk_ptr->size; element_number < (print_poison_data_count +  stk_ptr->size); element_number++)
	{
		printf("\t\t [%zd] = " STACK_ELEM_PRINTF_SPEC "\n", element_number, stk_ptr->data[element_number]);       
	}

	printf("\t}\n");

	return NO_ERROR;
}


unsigned int print_stack_error(const unsigned int error_code)
{
	if (error_code == NO_ERROR)
	{
		PRINT_STACK_ERR("NO ERROR\n");
		return error_code;
	}

	PRINT_STACK_ERR("errors:\n");

	if ((error_code & STACK_POINTER_IS_NULL) 			 != 0) PRINT_STACK_ERR("stack pointer is NULL\n");

	if ((error_code & STACK_DATA_IS_NULL)     		     != 0) PRINT_STACK_ERR("stack data is NULL\n");

	if ((error_code & INVALID_STACK_SIZE)      		     != 0) PRINT_STACK_ERR("stack size is < 0\n");

	if ((error_code & INVALID_STACK_CAPACITY) 		     != 0) PRINT_STACK_ERR("stack capacity is < 0\n");

	if ((error_code & STACK_SIZE_LARGER_THAN_CAPACITY)   != 0) PRINT_STACK_ERR("stack size is larger than stack capacity\n");

	if ((error_code & UNABLE_TO_INCREASE_STACK)          != 0) PRINT_STACK_ERR("unable to increase stack\n");

	if ((error_code & UNABLE_TO_DECREASE_STACK)   		 != 0) PRINT_STACK_ERR("unable to decrease stack\n");

	#ifdef CANARY_PROTECTION

		if ((error_code & LEFT_CANARY_DATA_DIED)         != 0) PRINT_STACK_ERR("left canary in data died\n");

		if ((error_code & RIGHT_CANARY_DATA_DIED)  		 != 0) PRINT_STACK_ERR("right canary in data died\n");

		if ((error_code & LEFT_CANARY_STACK_DIED)  		 != 0) PRINT_STACK_ERR("left canary in stack died\n");

		if ((error_code & RIGHT_CANARY_STACK_DIED) 		 != 0) PRINT_STACK_ERR("right canary in stack died\n");

	#endif /* CANARY_PROTECTION */

	#ifdef HASH_PROTECTION

		if ((error_code & DATA_HASH_IS_WRONG)     		 != 0) PRINT_STACK_ERR("wrong data hash\n");

		if ((error_code & STACK_HASH_IS_WRONG)     		 != 0) PRINT_STACK_ERR("wrong stack hash\n");


	#endif /* HASH_PROTECTION */


	return error_code;
}