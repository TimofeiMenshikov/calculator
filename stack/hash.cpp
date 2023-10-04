#include <stdio.h>
#include "include/stack.h"
#include "include/hash.h"
#include "include/canary.h"


#ifdef HASH_PROTECTION
	hash_t calc_hash_stack(struct Stack stk)
	{

		#ifdef a349282kflacXX

		hash_t hash_stack = (hash_t)stk.size; 

		#else

		stk.hash_stack = 0;

		hash_t hash_stack = calc_hash_sum(&stk, 8);

		HASH_DUMP(printf("sizeof stk: %zu \n", sizeof(stk))); 
		HASH_DUMP(printf("hash stack is " HASH_PRINTF_SPEC "\n", hash_stack));

		#endif 

		return hash_stack;
	}

	hash_t calc_hash_sum(const void* mem_block, const size_t mem_size)
	{
		const char* mem_str = (char*) mem_block;

		hash_t mem_sum = 0;

		for (size_t mem_char_number = 0; mem_char_number < mem_size; mem_char_number++)
		{
			//printf("%d(%c)\n", mem_str[mem_char_number], mem_str[mem_char_number]);
			mem_sum += mem_str[mem_char_number] * (mem_char_number + 1);
		}

		return mem_sum;
	}

	
	hash_t calc_hash_stack_data(const struct Stack* const stk_ptr)
	{
		#ifdef    CANARY_PROTECTION

			hash_t hash_data = calc_hash_sum(get_left_canary_ptr(stk_ptr->data), stk_ptr->capacity * sizeof(elem_t) + 2 * sizeof(canary_t));

		#else  /* CANARY_PROTECTION */

			hash_t hash_data = calc_hash_sum(stk_ptr->data, stk_ptr->capacity * sizeof(elem_t));

		#endif /* CANARY_PROTECTION */

		HASH_DUMP(printf("hash data is " HASH_PRINTF_SPEC "\n", hash_data));

		return hash_data;
	}


#endif /* HASH_PROTECTION */
