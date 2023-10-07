#include <stdio.h>
#include <assert.h>
#include <sys/stat.h>
#include <malloc.h>
#include "include/printf_debug.h"
#include "include/text.h"


FILE* open_file(const char* const filename, const char* const modificator)
{
	FILE* inputfile = fopen(filename, modificator);

	assert(inputfile);

	return inputfile;
}


static size_t find_n_strings(const char* const buffer, const size_t buffer_size)  /// find count of string 
{
	ssize_t n_strings = 0;
	for (size_t char_number = 0; (char_number < buffer_size) && (buffer[char_number] != '\0'); char_number++)
	{
		if (buffer[char_number] == '\n')
		{
			n_strings++;
		}
	}
	return n_strings + 1; // последний элемент \0 - конец строки, а мы его не посчитали
}


static char* init_buffer_from_file(const char* const filename, size_t* buffer_size_ptr)
{
	struct stat buffer_info;

	FILE* inputfile = open_file(filename, "r");

	stat(filename, &buffer_info);

	char* buffer = (char*) calloc(buffer_info.st_size / sizeof(char) + 1, sizeof(char));

	size_t buffer_size = fread(buffer, sizeof(char), buffer_info.st_size, inputfile) + 1;

	buffer[buffer_size - 1] = '\0';

	fclose(inputfile);

	*buffer_size_ptr = buffer_size; 

	return buffer;
}


static void split_strings(char* const buffer, const size_t buffer_size) /// function, that replaces '\n' to '\0'
{
	for (size_t char_number = 0; char_number < buffer_size; char_number++)
	{
		if (buffer[char_number] == '\n')
		{
			buffer[char_number] = '\0';
		}
	}	
}


static void link_text_and_buf(char** const text,  char* const buffer, const size_t buffer_size)
{	
	split_strings(buffer, buffer_size);

	text[0] = buffer; // присваиваем первому указателю text ссылку на первый элемент buffer, то есть на первую строку

	size_t text_pos = 1; // позция в "массиве" text

	for (size_t char_number = 0; char_number < buffer_size - 1; char_number++) // buffer_size - 1, чтобы не считывало последний \0
	{
		DEBUG_EXEC(printf("pos %zu: %c(%d)\n", char_number, buffer[char_number], buffer[char_number]));

		if (buffer[char_number] == '\0')
		{
			text[text_pos] = buffer + char_number + 1; // находим конец строки и присваеваем адрес следующего элемента элементу массива 

			DEBUG_EXEC(printf("added link number %zu: read element %c(%d) next element %c(%d)\n", text_pos, buffer[char_number], buffer[char_number], buffer[char_number + 1], buffer[char_number + 1]));

			text_pos++;
		}
	}

	for (size_t i = 0; i < text_pos; i++)
	{
		DEBUG_EXEC(printf("addres number %zu is %p\n", i, text[i]));
	}
}


char** init_text(const char* const filename, ssize_t* n_strings_ptr)
{	
	size_t buffer_size = 0;

	char* buffer = init_buffer_from_file(filename, &buffer_size);

	ssize_t n_strings = find_n_strings(buffer, buffer_size);

	printf("the number of strings is: %zu\n", n_strings);

	char** text = (char**) calloc(n_strings, sizeof(char*));

	DEBUG_EXEC(printf("calloc text \n"));

	link_text_and_buf(text, buffer, buffer_size);

	*n_strings_ptr = n_strings;

	return text;
}


void print_text(const char* const * const text, const ssize_t n_strings, FILE* outputfile)
{
	for (ssize_t n_string = 0; n_string < n_strings; n_string++)
	{
		fputs(text[n_string], outputfile);
		fputs("\n", outputfile);
	}
}





void free_text(char** text)
{
	free(text[0]);
	free(text);

	text = 0;
}