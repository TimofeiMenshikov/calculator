DEF_CMD(PUSH,  1, 
{
	printed_numbers = print_push_bytecode(string, n_string, code_arr, code_ip_ptr);
},
{
	printed_numbers = print_push_command(code_arr, code_ip_ptr, outputfile);
},
{
	return_code |= do_push_command(spu_ptr, spu_ptr->code[spu_ptr->ip]);
})
DEF_CMD(POP,   -1,  
{
	printed_numbers = print_pop_bytecode(string, n_string, code_arr, code_ip_ptr);
},
{
	printed_numbers = print_pop_command(code_arr, code_ip_ptr, outputfile);
},
{
	return_code |= do_pop_command(spu_ptr);
})
DEF_CMD(ADD,   2,  
{
	code_arr[*code_ip_ptr] = ADD;
	(*code_ip_ptr)++;
	printf("code_ip_ptr++\n");
},
{
	printed_numbers = fprintf(outputfile, "ADD");
},
{
	return_code |= do_bin_command(&(spu_ptr->stk), do_add_command);	
})
DEF_CMD(SUB,  -2,  
{
	code_arr[*code_ip_ptr] = SUB;
	(*code_ip_ptr)++;
	printf("code_ip_ptr++\n");
},
{
	printed_numbers = fprintf(outputfile, "SUB");
},
{
	return_code |= do_bin_command(&(spu_ptr->stk), do_sub_command);		
})
DEF_CMD(MUL,   3,  
{
	code_arr[*code_ip_ptr] = MUL;
	(*code_ip_ptr)++;
	printf("code_ip_ptr++\n");
},
{
	printed_numbers = fprintf(outputfile, "MUL");
},
{
	return_code |= do_bin_command(&(spu_ptr->stk), do_mul_command);
}) 
DEF_CMD(DIV,  -3,  
{
	code_arr[*code_ip_ptr] = DIV;
	(*code_ip_ptr)++;
	printf("code_ip_ptr++\n");
},
{
	printed_numbers = fprintf(outputfile, "DIV");
},
{
	return_code |= do_bin_command(&(spu_ptr->stk), do_div_command);
})
DEF_CMD(SQRT, -4,  
{
	code_arr[*code_ip_ptr] = SQRT;
	(*code_ip_ptr)++;
	printf("code_ip_ptr++\n");
},
{
	printed_numbers = fprintf(outputfile, "SQRT");
},
{
	return_code |= do_unary_command(&(spu_ptr->stk), do_sqrt_command);
})
DEF_CMD(SIN,   5,  
{
	code_arr[*code_ip_ptr] = SIN;
	(*code_ip_ptr)++;
},
{
	printed_numbers = fprintf(outputfile, "SIN");
},
{
	return_code |= do_unary_command(&(spu_ptr->stk), do_cos_command);		
})
DEF_CMD(COS,  -5,  
{
	code_arr[*code_ip_ptr] = COS;
	(*code_ip_ptr)++;
},
{
	printed_numbers = fprintf(outputfile, "COS");
},
{
	return_code |= do_unary_command(&(spu_ptr->stk), do_sin_command);	
})
DEF_CMD(OUT,   6,  
{
	code_arr[*code_ip_ptr] = OUT;
	(*code_ip_ptr)++;
},
{
	printed_numbers = fprintf(outputfile, "OUT");
},
{
	return_code |= do_out_command(&(spu_ptr->stk)); 	
})
DEF_CMD(HLT,   7, 
{
	code_arr[*code_ip_ptr] = HLT;
	(*code_ip_ptr)++;
},
{
	printed_numbers = fprintf(outputfile, "HLT");
},
{
	return return_code;
})
DEF_CMD(IN,    8,  
{
	code_arr[*code_ip_ptr] = IN;
	(*code_ip_ptr)++;
},
{
	printed_numbers = fprintf(outputfile, "IN");
},
{
	return_code |= do_in_command(&(spu_ptr->stk));	
})
DEF_CMD(RPUSH, 9,  
{
	printed_numbers = print_rpush_bytecode(string, n_string, code_arr, code_ip_ptr);
},
{
	printed_numbers = print_rpush_command(code_arr, code_ip_ptr, outputfile);
},
{
	return_code |= do_push_command(spu_ptr, spu_ptr->r_x[(ssize_t) spu_ptr->code[spu_ptr->ip]]);	
})