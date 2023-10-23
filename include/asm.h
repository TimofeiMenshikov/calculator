#ifndef ASM_H
	#define ASM_H
	
	#include "../stack/include/stack.h"

	const ssize_t REGISTER_NAME_SIZE = 3;
	const ssize_t REGISTER_LETTER_POS = 1; // местонахождение буквы, имеющей порядковый смысл напр: rAx, rBx, rCx	

	const ssize_t LABELS_SIZE = 30;
	const ssize_t LABEL_NAME_SIZE = 30;

	struct Labels
	{
		char* labels[LABELS_SIZE] = {};

		ssize_t labels_pos[LABELS_SIZE] = {};

		ssize_t scanned_labels = 0;
	};
	
	ssize_t assembler(const char* const *  const text, const ssize_t n_strings, elem_t* code_arr);



#endif /* ASM_H */