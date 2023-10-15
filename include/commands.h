DEF_CMD(PUSH,  1, 
{
	printed_numbers = print_push_bytecode(string, n_string, outputfile);
},
{
	printed_numbers = print_push_command(string, n_string, outputfile);
},
{
	return_code |= do_push_command(spu_ptr, spu_ptr->code[spu_ptr->ip]);
})
DEF_CMD(POP,   -1,  
{
	printed_numbers = print_pop_bytecode(string, n_string, outputfile);
},
{
	printed_numbers = print_pop_command(string, n_string, outputfile);
},
{
	return_code |= do_pop_command(spu_ptr);
})
DEF_CMD(ADD,   2,  
{
	printed_numbers = fprintf(outputfile, "%d ", ADD);
},
{
	printed_numbers = fprintf(outputfile, "ADD");
},
{
	return_code |= do_bin_command(&(spu_ptr->stk), do_add_command);	
})
DEF_CMD(SUB,  -2,  
{
	printed_numbers = fprintf(outputfile, "%d ", SUB);
},
{
	printed_numbers = fprintf(outputfile, "ADD");
},
{
	return_code |= do_bin_command(&(spu_ptr->stk), do_sub_command);		
})
DEF_CMD(MUL,   3,  
{
	printed_numbers = fprintf(outputfile, "%d ", MUL);
},
{
	printed_numbers = fprintf(outputfile, "MUL");
},
{
	return_code |= do_bin_command(&(spu_ptr->stk), do_mul_command);
}) 
DEF_CMD(DIV,  -3,  
{
	printed_numbers = fprintf(outputfile, "%d ", DIV);
},
{
	printed_numbers = fprintf(outputfile, "DIV");
},
{
	return_code |= do_bin_command(&(spu_ptr->stk), do_div_command);
})
DEF_CMD(SQRT, -4,  
{
	printed_numbers = fprintf(outputfile, "%d ", SQRT);
},
{
	printed_numbers = fprintf(outputfile, "SQRT");
},
{
	return_code |= do_unary_command(&(spu_ptr->stk), do_sqrt_command);
})
DEF_CMD(SIN,   5,  
{
	printed_numbers = fprintf(outputfile, "%d ", SIN);
},
{
	printed_numbers = fprintf(outputfile, "SIN");
},
{
	return_code |= do_unary_command(&(spu_ptr->stk), do_cos_command);		
})
DEF_CMD(COS,  -5,  
{
	printed_numbers = fprintf(outputfile, "%d ", COS);
},
{
	printed_numbers = fprintf(outputfile, "COS");
},
{
	return_code |= do_unary_command(&(spu_ptr->stk), do_sin_command);	
})
DEF_CMD(OUT,   6,  
{
	printed_numbers = fprintf(outputfile, "%d ", OUT);
},
{
	printed_numbers = fprintf(outputfile, "OUT");
},
{
	return_code |= do_out_command(&(spu_ptr->stk)); 	
})
DEF_CMD(HLT,   7, 
{
	printed_numbers = fprintf(outputfile, "%d ", HLT);
},
{
	printed_numbers = fprintf(outputfile, "HLT");
},
{
	return return_code;
})
DEF_CMD(IN,    8,  
{
	printed_numbers = fprintf(outputfile, "%d ", IN);
},
{
	printed_numbers = fprintf(outputfile, "IN");
},
{
	return_code |= do_in_command(&(spu_ptr->stk));	
})
DEF_CMD(RPUSH, 9,  
{
	printed_numbers = print_rpush_bytecode(string, n_string, outputfile);
},
{
	printed_numbers = print_rpush_command(string, n_string, outputfile);
},
{
	return_code |= do_push_command(spu_ptr, spu_ptr->r_x[(ssize_t) spu_ptr->code[spu_ptr->ip]]);	
})