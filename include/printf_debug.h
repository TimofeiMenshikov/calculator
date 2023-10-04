#ifndef PRINTF_DEBUG_H

	#define PRINTF_DEBUG_H

	#ifdef DUMP
	    #define DEBUG_EXEC(function)    \
	        do                          \
	        {                           \
	            function;               \
	        }                           \
	        while(0);

	#else

	    #define DEBUG_EXEC(function) 

	#endif /* DUMP */         

	#ifdef STRCMP_DEBUG

	    #define STRCMP_DUMP(function)    \
        do                          	 \
        {                           	 \
            function;               	 \
        }                           	 \
        while(0);

    #else

        #define STRCMP_DUMP(function) 

	#endif /* STRCMP_DEBUG */


    #ifdef SORT_DEBUG

	    #define SORT_DUMP(function)      \
        do                          	 \
        {                           	 \
            function;               	 \
        }                           	 \
        while(0);

    #else

        #define SORT_DUMP(function)

    #endif /* SORT_DEBUG */


#endif /* PRINTF_DEBUG_H */