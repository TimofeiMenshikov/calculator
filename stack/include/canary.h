#ifndef CANARY_H
	#define CANARY_H

	#include "stack.h"

	#ifdef CANARY_PROTECTION

		const elem_t CANARY_LEFT_CONSTANT = 1253;
		const elem_t CANARY_RIGHT_CONSTANT = 3521;

		canary_t* get_right_canary_ptr(void* const data, ssize_t capacity);
		canary_t* get_left_canary_ptr(void* const data);

	#endif /* CANARY_PROTECTION */

	void* alloc_canary_data(const ssize_t old_capacity, const ssize_t new_capacity, void* data);

	void* new_canary_data(const ssize_t capacity);
	unsigned int free_stack_data(elem_t* data);

#endif /* CANARY_H */		