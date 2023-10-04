#ifndef PRINT_H
    #define PRINT_H

    #include "stack.h"

    #ifdef STACK_DUMP
    	#define PRINT_STACK_ERR(text)     \
        do                                \
        {                                 \
            fprintf(stderr, "%s\n", text);\
        }                                 \
        while (false);                  

    #else /* STACK_DUMP */
    	#define PRINT_STACK_ERR(text) 
    #endif /* STACK_DUMP */

    #ifdef STDOUT_DUMP
        #define STDOUT_PRINT(function)  \
            do                          \
            {                           \
                function;               \
            }                           \
            while(0);

    #else /* STDOUT_DUMP */

        #define STDOUT_PRINT(function)

    #endif /* STDOUT_DUMP */ 

    unsigned int print_stack(const struct Stack* const stk_ptr, ssize_t print_data_count);

    unsigned int print_stack_error(const unsigned int error_code);

    unsigned int print_data(const Stack*  const stk_ptr, ssize_t print_poison_data_count);

#endif /* PRINT_H */