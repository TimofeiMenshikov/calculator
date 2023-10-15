#ifndef ASM_CODES_H
	#define ASM_CODES_H

	typedef ssize_t command_t;

	#define COMMAND_PRINTF_SPEC "%zd"


	////////////////////////////////////////////////////////////////////////
	#define DEF_CMD(cmd_name, number, asm_func, disasm_func, spu_func) \
		cmd_name = number, 											   \

	enum asm_codes
	{
		/*PUSH  =   1,
		POP   =  -1,
		ADD   =   2,
		SUB   =  -2, 
		MUL   =   3,
		DIV   =  -3,
		SQRT  =  -4,
		SIN   =   5,
		COS   =  -5,
		OUT   =   6,
		HLT   =   7,
		IN 	  =   8,
		RPUSH =   9 */

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