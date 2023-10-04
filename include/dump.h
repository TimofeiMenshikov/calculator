#ifndef DUMP_H
	#define DUMP_H
	
	#ifdef DUMP

		#define PRINT_ERR_POS()                                                             \
		do                     																\
		{																					\
			printf("error. file: %s, func: %s, line: %d\n", __FILE__, __func__, __LINE__);	\
		}																					\
		while (false);																		

	#else  /* DUMP */

		#define PRINT_ERR_POS()

	#endif /* DUMP */

#endif /* DUMP_H */