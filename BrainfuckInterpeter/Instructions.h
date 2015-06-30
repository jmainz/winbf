#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

typedef enum Instructions {
	inc_pointer,
	dec_pointer,
	inc_byte,
	dec_byte,
	print,
	get,
	break_zero,
	break_non_zero
} Instructions;

typedef struct Program {
	Instructions instr;
	unsigned int destination;
} Program;

#endif