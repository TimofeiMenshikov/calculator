#ifndef ASM_H
	#define ASM_H
	


	const ssize_t REGISTER_NAME_SIZE = 3;
	const ssize_t REGISTER_LETTER_POS = 1; // местонахождение буквы, имеющей порядковый смысл напр: rAx, rBx, rCx	
	
	void assembler(const char* const *  const text, const ssize_t n_strings, FILE* outputfile);



#endif /* ASM_H */