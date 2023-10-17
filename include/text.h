#ifndef TEXT_H
	#define TEXT_H

	#include "../stack/include/stack.h"

	struct Array 
	{
		void* code;
		ssize_t size;
	};

	enum array_err
	{
		CODE_IS_NULL = 1,
		INVALID_ARRAY_SIZE = 2
	};

	unsigned int array_verificator(const struct Array* const code_ptr);
	void print_arr_error(const unsigned int err_arr);


	char* init_buffer_from_file(const char* const filename, size_t* buffer_size_ptr);

	char** init_text(const char* const filename, ssize_t* n_strings_ptr);
	void print_text(const char* const * const text, const ssize_t n_strings, FILE* outputfile);
	FILE* open_file(const char* const filename, const char* const modificator);
	void free_text(char** text);

	struct Array init_code_from_bin_file();

#endif /* TEXT_H */