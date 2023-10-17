// DEF_CMD(COMMAND_NAME, COMMAND_NUMBER, ARG_TYPE, disasm_func, spu_func)

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
	printed_numbers = print_push_command(code_arr, code_ip_ptr, outputfile);
},
{
	return_code |= do_push_command(spu_ptr, spu_ptr->code[spu_ptr->ip]);
})
DEF_CMD(POP, -1, REG_ARG,
{
	printed_numbers = print_pop_command(code_arr, code_ip_ptr, outputfile);
},
{
	return_code |= do_pop_command(spu_ptr);
})
DEF_CMD(ADD,   2, NO_ARGS,
{
	printed_numbers = fprintf(outputfile, "ADD");
},
{
	return_code |= do_bin_command(&(spu_ptr->stk), do_add_command);	
})
DEF_CMD(SUB,  -2, NO_ARGS, 
{
	printed_numbers = fprintf(outputfile, "SUB");
},
{
	return_code |= do_bin_command(&(spu_ptr->stk), do_sub_command);		
})
DEF_CMD(MUL,   3, NO_ARGS,
{
	printed_numbers = fprintf(outputfile, "MUL");
},
{
	return_code |= do_bin_command(&(spu_ptr->stk), do_mul_command);
}) 
DEF_CMD(DIV,  -3, NO_ARGS,
{
	printed_numbers = fprintf(outputfile, "DIV");
},
{
	return_code |= do_bin_command(&(spu_ptr->stk), do_div_command);
})
DEF_CMD(SQRT, -4, NO_ARGS,
{
	printed_numbers = fprintf(outputfile, "SQRT");
},
{
	return_code |= do_unary_command(&(spu_ptr->stk), do_sqrt_command);
})
DEF_CMD(SIN,   5, NO_ARGS,
{
	printed_numbers = fprintf(outputfile, "SIN");
},
{
	return_code |= do_unary_command(&(spu_ptr->stk), do_cos_command);		
})
DEF_CMD(COS,  -5, NO_ARGS,
{
	printed_numbers = fprintf(outputfile, "COS");
},
{
	return_code |= do_unary_command(&(spu_ptr->stk), do_sin_command);	
})
DEF_CMD(OUT,  6, NO_ARGS,  
{
	printed_numbers = fprintf(outputfile, "OUT");
},
{
	return_code |= do_out_command(&(spu_ptr->stk)); 	
})
DEF_CMD(HLT,  7, NO_ARGS, 
{
	printed_numbers = fprintf(outputfile, "HLT");
},
{
	return return_code;
})
DEF_CMD(IN,  8, NO_ARGS,  
{
	printed_numbers = fprintf(outputfile, "IN");
},
{
	return_code |= do_in_command(&(spu_ptr->stk));	
})
DEF_CMD(RPUSH, 9, REG_ARG, 
{
	printed_numbers = print_rpush_command(code_arr, code_ip_ptr, outputfile);
},
{
	return_code |= do_push_command(spu_ptr, spu_ptr->r_x[(ssize_t) spu_ptr->code[spu_ptr->ip]]);	
})