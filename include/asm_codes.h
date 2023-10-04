#ifndef ASM_CODES_H
	#define ASM_CODES_H

	typedef ssize_t command_t;

	#define COMMAND_PRINTF_SPEC "%zd"

	enum asm_codes
	{
		PUSH =  1,
		POP  = -1,
		ADD  =  2,
		SUB  = -2, 
		MUL  =  3,
		DIV  = -3,
		SQRT = -4,
		SIN  =  5,
		COS  =  -5,
		OUT  =  6,
		HLT  =  7
	};

#endif /* ASM_CODES_H */