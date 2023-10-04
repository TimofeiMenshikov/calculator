#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include "include/canary.h"
#include "include/stack.h"
#include "include/print.h"


#ifdef CANARY_PROTECTION

	canary_t* get_left_canary_ptr(void* const data)
	{
		canary_t* const canary_left_ptr = (canary_t*)data - 1;
		return canary_left_ptr;
	}

	canary_t* get_right_canary_ptr(void* const data, ssize_t capacity)
	{
		canary_t* const canary_right_ptr = (canary_t*)((elem_t*)data + capacity);
		return canary_right_ptr;
	}

#endif /* CANARY_PROTECTION */


unsigned int free_stack_data(elem_t* data)
{
	#ifdef CANARY_PROTECTION
		free(get_left_canary_ptr(data));
	#else /* CANARY_PROTECTION */
		free(data);
	#endif /* CANARY_PROTECTION */

	data = 0;

	return NO_ERROR;
}


void* alloc_canary_data(const ssize_t old_capacity, const ssize_t new_capacity, void* data)
{


	#ifdef CANARY_PROTECTION
		
		size_t n_bytes = new_capacity * sizeof(elem_t) + 2 * sizeof(canary_t);

		STDOUT_PRINT(printf("%zu bytes to calloc\n", n_bytes));
		
		if (data == NULL)  // проверка существует кусок памяти с массивом или нет
		{			
			data = (void*)malloc(n_bytes);
			STDOUT_PRINT(printf("data after calloc:  [%p]\n", data));
		}
		else
		{
			data = (void*)realloc(get_left_canary_ptr(data), n_bytes);

			STDOUT_PRINT(printf("data after realloc:  [%p]\n", data));
		}

		if (data == NULL)  // проверка, удачно ли произошла аллокация памяти
		{
			PRINT_STACK_ERR("unable to alloc_canary_data\n");
			return NULL;
		}

		assert(data);

		data = (void*)((canary_t*)data + 1);

		STDOUT_PRINT(printf("data after offset   [%p]\n", data));

		canary_t* canary_left_ptr = get_left_canary_ptr(data);

		STDOUT_PRINT(printf("canary_left_ptr is  [%p]\n", canary_left_ptr));

		*canary_left_ptr = CANARY_LEFT_CONSTANT;

		canary_t* canary_right_ptr = get_right_canary_ptr(data, new_capacity);

		STDOUT_PRINT(printf("canary_right_ptr is [%p]\n", canary_right_ptr));

		*canary_right_ptr = CANARY_RIGHT_CONSTANT;
	
	#else /* CANARY_PROTECTION */

		size_t n_bytes = new_capacity * sizeof(elem_t);

		STDOUT_PRINT(("%zu bytes to calloc\n", n_bytes));

		if (data == NULL) // проверка существует кусок памяти с массивом или нет
		{
			data = (void*) malloc(n_bytes);
		} 
		else
		{
			data = (void*) realloc(data, n_bytes);  
		}


		if (data == NULL) // проверка, удачно ли произошла аллокация памяти
		{
			PRINT_STACK_ERR("unable to alloc_canary_data\n");
			return NULL;
		}

		assert(data);

	#endif /* CANARY_PROTECTION */	

	for (ssize_t n_elem = old_capacity; n_elem < new_capacity; n_elem++)
	{
		*((elem_t*)data + n_elem) = POISON_VALUE;
	}

	return data;	
}


void* new_canary_data(const ssize_t capacity)
{
	elem_t* data = NULL;

	data = (elem_t*)alloc_canary_data(0, capacity, data);

	if (data == NULL)
	{
		PRINT_STACK_ERR("unable to init stack");
	}

	return data;
}


