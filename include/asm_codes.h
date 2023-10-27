#ifndef ASM_CODES_H
	#define ASM_CODES_H

	typedef ssize_t command_t;

	#define COMMAND_PRINTF_SPEC "%zd"


	////////////////////////////////////////////////////////////////////////
	#define DEF_CMD(cmd_name, number, arg_type, spu_func) 			\
		cmd_name = number, 											\


	enum arg_types_command
	{
		NO_ARGS = 0,
		NUM_ARG = 1,
		REG_ARG = 2,
		NUM_OR_LABEL_ARG = 4,
		RAM_ARG = 8,
		RAM_REG_ARG = 16
	};

	enum arg_bytes
	{
		NUM_ARGUMENT 		  = 1 << 5,
		REG_ARGUMENT 		  = 1 << 6,
		NUM_OR_LABEL_ARGUMENT = 1 << 7,
		RAM_ARGUMENT 		  = 1 << 8
	};


	enum asm_codes
	{
		#include "commands.h"
		END_CODE
	};

	#undef DEF_CMD
	/////////////////////////////////////////////////////////////////////////



#endif /* ASM_CODES_H */




/*
DEF_CMD(PUSH, 1, do_push_command(SPU);)


#define DEF_CMD(a, b, ...) \
	case a:
		__VA_ARGS__
		break;

#include ""

#undef DEF_CMD
*/