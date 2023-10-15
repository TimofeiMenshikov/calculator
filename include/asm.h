#ifndef ASM_H
	#define ASM_H
	
	#include "../stack/include/stack.h"

	const ssize_t REGISTER_NAME_SIZE = 3;
	const ssize_t REGISTER_LETTER_POS = 1; // местонахождение буквы, имеющей порядковый смысл напр: rAx, rBx, rCx	
	
	ssize_t assembler(const char* const *  const text, const ssize_t n_strings, elem_t* code_arr);



#endif /* ASM_H */