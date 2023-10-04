#ifndef HASH_H
	#define HASH_H

	#ifdef HASH_PROTECTION

		hash_t calc_hash_stack(struct Stack stk);
		hash_t calc_hash_stack_data (const struct Stack* const stk_ptr);
		hash_t calc_hash_sum(const void* mem_block, const size_t mem_size);

		#ifdef HASH_DEBUG

			#define HASH_DUMP(function) \
			do 							\
			{							\
				function;				\
			} 							\
			while (false);				

		#else /* HASH_DEBUG */

			#define HASH_DUMP(text)

		#endif /* HASH_DEBUG */

	#endif /* HASH_PROTECTION */



#endif /* HASH_H */ 