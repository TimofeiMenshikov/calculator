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

DEF_CMD(PUSH,  1, NUM_ARG,
{
	return_code |= do_push_command(spu_ptr, spu_ptr->code[spu_ptr->ip]);
})
DEF_CMD(POP, -1, REG_ARG,
{
	return_code |= do_pop_command(spu_ptr);
})
DEF_CMD(ADD,   2, NO_ARGS,
{
	return_code |= do_bin_command(&(spu_ptr->stk), do_add_command);	
})
DEF_CMD(SUB,  -2, NO_ARGS, 
{
	return_code |= do_bin_command(&(spu_ptr->stk), do_sub_command);		
})
DEF_CMD(MUL,   3, NO_ARGS,
{
	return_code |= do_bin_command(&(spu_ptr->stk), do_mul_command);
}) 
DEF_CMD(DIV,  -3, NO_ARGS,
{
	return_code |= do_bin_command(&(spu_ptr->stk), do_div_command);
})
DEF_CMD(SQRT, -4, NO_ARGS,
{
	return_code |= do_unary_command(&(spu_ptr->stk), do_sqrt_command);
})
DEF_CMD(SIN,   5, NO_ARGS,
{
	return_code |= do_unary_command(&(spu_ptr->stk), do_cos_command);		
})
DEF_CMD(COS,  -5, NO_ARGS,
{
	return_code |= do_unary_command(&(spu_ptr->stk), do_sin_command);	
})
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
DEF_CMD(RPUSH, 9, REG_ARG, 
{
	return_code |= do_push_command(spu_ptr, spu_ptr->r_x[(ssize_t) spu_ptr->code[spu_ptr->ip]]);	
})
DEF_CMD(JMP, 10, NUM_OR_LABEL_ARG, 
{
	return_code |= do_jmp_command(spu_ptr);	
})
DEF_CMD(JBE, 12, NUM_OR_LABEL_ARG,
{
	return_code |= do_ifjmp_command(spu_ptr, is_below_or_equal);
})
DEF_CMD(JB, 11, NUM_OR_LABEL_ARG,
{
	return_code |= do_ifjmp_command(spu_ptr, is_below);
})
DEF_CMD(JAE, 14, NUM_OR_LABEL_ARG,
{
	return_code |= do_ifjmp_command(spu_ptr, is_above_or_equal);
})
DEF_CMD(JA, 13, NUM_OR_LABEL_ARG,
{
	return_code |= do_ifjmp_command(spu_ptr, is_above);
})
DEF_CMD(JE, 15, NUM_OR_LABEL_ARG,
{
	return_code |= do_ifjmp_command(spu_ptr, is_equal);
})
DEF_CMD(JNE, 16, NUM_OR_LABEL_ARG,
{
	return_code |= do_ifjmp_command(spu_ptr, is_not_equal);
})
DEF_CMD(RAM_PUSH, -6, NUM_ARG, 
{
	return_code |= do_ram_push_command(spu_ptr, spu_ptr->code[spu_ptr->ip]);
})
DEF_CMD(RAM_REG_PUSH, -7, REG_ARG, 
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