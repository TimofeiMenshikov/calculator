// DEF_CMD(COMMAND_NAME, COMMAND_NUMBER, ARG_TYPE, spu_func)

/*#define DEF_SIMPLE_CMD(name, num, spu_func) \
	DEF_CMD(name, num, 
	{
		code_arr[*code_ip_ptr] = num;
		(*code_ip_ptr)++;
	},
	{
		printed_numbers = fprintf(outputfile, #name);
	}, spu_func

)*/


#define DEF_BIN_CMD(cmd_name, number, func)										\
DEF_CMD(cmd_name, number, NO_ARGS, 												\
{																				\
	do_bin_command(func);														\
})																				\


#define DEF_UNARY_CMD(cmd_name, number, func)									\
DEF_CMD(cmd_name, number, NO_ARGS, 												\
{																				\
	do_unary_command(func);														\
})																				\


#define DEF_IFJMP_CMD(cmd_name, number, func)									\
DEF_CMD(cmd_name, number, NUM_OR_LABEL_ARG,										\
{																				\
	DO_IFJMP_COMMAND(func);														\
})																				\


DEF_CMD(PUSH,  33, NUM_ARG,
{
	return_code |= do_push_command(spu_ptr, spu_ptr->code[spu_ptr->ip]);
})
DEF_CMD(POP, -1, REG_ARG,
{
	return_code |= do_pop_command(spu_ptr);
})


DEF_BIN_CMD(ADD, 2, penult + last)
DEF_BIN_CMD(SUB, -2, penult - last)
DEF_BIN_CMD(MUL, 3, penult * last)
DEF_BIN_CMD(DIV, -3, penult / last)

DEF_UNARY_CMD(SQRT, -4, sqrt(last))
DEF_UNARY_CMD(SIN, 5, sin(last))
DEF_UNARY_CMD(COS, -5, cos(last))

DEF_IFJMP_CMD(JBE, 12, penult <= last)
DEF_IFJMP_CMD(JB, 11, penult <  last)
DEF_IFJMP_CMD(JAE, 14, penult >= last)
DEF_IFJMP_CMD(JA, 13, penult >  last)
DEF_IFJMP_CMD(JE, 15, penult == last)
DEF_IFJMP_CMD(JNE, 16, penult != last)


DEF_CMD(OUT,  6, NO_ARGS,  
{
	return_code |= do_out_command(&(spu_ptr->stk)); 	
})
DEF_CMD(HLT,  7, NO_ARGS, 
{
	printf("HLTHLTHLTHLTHLTHLTHLTHTLHTLTHTLHTLHTLTHTLHTLHTLHTHTLHT\n");
	return return_code;
})
DEF_CMD(IN,  8, NO_ARGS,  
{
	return_code |= do_in_command(&(spu_ptr->stk));	
})
DEF_CMD(RPUSH, 65, REG_ARG, 
{
	return_code |= do_push_command(spu_ptr, spu_ptr->r_x[(ssize_t) spu_ptr->code[spu_ptr->ip]]);	
})
DEF_CMD(JMP, 10, NUM_OR_LABEL_ARG, 
{
	return_code |= do_jmp_command(spu_ptr);	
})

DEF_CMD(RAM_PUSH, 289, NUM_ARG, 
{
	return_code |= do_ram_push_command(spu_ptr, spu_ptr->code[spu_ptr->ip]);
})
DEF_CMD(RAM_REG_PUSH, 321, REG_ARG, 
{
	return_code |= do_ram_push_command(spu_ptr, spu_ptr->r_x[(ssize_t) spu_ptr->code[spu_ptr->ip]]);
})
DEF_CMD(RAM_POP, -8, NUM_ARG,
{
	return_code |= do_ram_pop_command(spu_ptr, spu_ptr->code[spu_ptr->ip]);
})
DEF_CMD(RAM_REG_POP, -9, REG_ARG, 
{
	return_code |= do_ram_pop_command(spu_ptr, (spu_ptr->r_x)[(ssize_t) spu_ptr->code[spu_ptr->ip]]);
})
DEF_CMD(DRAW_RAM, -10, NO_ARGS,
{
	return_code |= do_draw_ram_command(spu_ptr->ram, RAM_SIZE / RAM_PRINTF_WIDE, RAM_PRINTF_WIDE);
})
DEF_CMD(CALL, -11, NUM_OR_LABEL_ARG,
{
	return_code |= do_call_command(spu_ptr);
})
DEF_CMD(RET, -12, NO_ARGS,
{
	return_code |= do_ret_command(spu_ptr);
})