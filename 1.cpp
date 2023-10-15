#include <stdio.h>
#include <malloc.h>

const ssize_t ARR_SIZE = 10;

int main()
{
	double* arr = (double*) calloc(ARR_SIZE, sizeof(double));

	for (ssize_t i = 0; i < ARR_SIZE; i++)
	{
		arr[i] = (double) i;
	}

	/*
	FILE* outputfile = fopen("123.txt", "wb");

	fwrite((void*) arr, sizeof(double), ARR_SIZE, outputfile);

	fclose(outputfile);

	*/
	FILE* inputfile = fopen("txt/code.bin", "rb");

	double input_arr_size = 0;

	fread((void*) &input_arr_size, sizeof(double), 1, inputfile);

	double* input_arr = (double*) calloc(input_arr_size, sizeof(double)); 

	

	fread((void*) input_arr, sizeof(double), input_arr_size, inputfile);

	for (ssize_t i = 0; i < input_arr_size; i++)
	{
		printf("%lf\n", input_arr[i]);
	}
}