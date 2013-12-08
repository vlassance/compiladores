int main() {
	strg=0;
	ptr=&memory[strg];
	while(*ptr) {
		strg=0;
		++strg;
		++strg;
		++strg;
		++strg;
		++strg;
		++strg;
		ptr=&memory[strg];
		strg=0;
		*ptr+=memory[strg];
		
		strg=0;
		++strg;
		++strg;
		++strg;
		++strg;
		ptr=&memory[strg];
		*ptr=*ptr==memory[strg]?1:0;
		*ptr-=memory[strg];
		++strg;
		++strg;
		*ptr+=memory[strg];
		strg=*ptr;
		*ptr+=memory[strg];
		
		strg=0;
		++strg;
		++strg;
		++strg;
		++strg;
		++strg;
		++strg;
		*ptr-=memory[strg];
		
		strg=0;
		++strg;
		++strg;
		*ptr=*ptr==memory[strg]?1:0;
		
		strg=0;
		++strg;
		*ptr=*ptr==memory[strg]?1:0;
		
		strg=0;
		++strg;
		++strg;
		++strg;
		++strg;
		++strg;
		ptr=&memory[strg];
		*ptr=*ptr==memory[strg]?1:0;
		*ptr-=memory[strg];
		++strg;
		*ptr+=memory[strg];
		strg=*ptr;
		*ptr+=memory[strg];
		
		strg=0;
		++strg;
		++strg;
		++strg;
		++strg;
		++strg;
		++strg;
		*ptr-=memory[strg];
		
		strg=0;
		++strg;
		*ptr=*ptr==memory[strg]?1:0;
		
		strg=0;
		++strg;
		*ptr=*ptr==memory[strg]?1:0;
		
		strg=0;
		++strg;
		++strg;
		++strg;
		ptr=&memory[strg];
		*ptr=*ptr==memory[strg]?1:0;
		*ptr-=memory[strg];
		++strg;
		*ptr+=memory[strg];
		++strg;
		*ptr*=memory[strg];
	} end_while
	strg=0;
	++strg;
	++strg;
	++strg;
	++strg;
	ptr=&memory[strg];
	strg=0;
	++strg;
	*ptr=*ptr==memory[strg]?1:0;
} end_main
