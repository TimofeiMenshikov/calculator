#ifndef TEXT_H
	#define TEXT_H

	char* init_buffer_from_file(const char* const filename, size_t* buffer_size_ptr);
	char** init_text(const char* const filename, ssize_t* n_strings_ptr);
	void print_text(const char* const * const text, const ssize_t n_strings, FILE* outputfile);
	FILE* open_file(const char* const filename, const char* const modificator);
	void free_text(char** text);

#endif /* TEXT_H */