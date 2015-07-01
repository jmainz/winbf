#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "Instructions.h"
#include "Compiler.h"

/*  > : increment the data pointer
 *  < : decrement the data pointer
 *  + : increment the byte pointed at by 1
 *  - : decrement the byte pointed at by 1
 *  . : output the byte pointed at
 *  , : accept one byte of input
 *  [ : if the byte at the data pointer is 0 jump the pointer to the matching ]
 *  ] : if the byte at the data pointer is nonzero, jump the pointer back to the corresponding [
 */

unsigned char data[30000];
unsigned int data_pointer;
unsigned int program_counter;
Program *prog = NULL;

Program fetch()
{
	return prog[program_counter];
}


void eval(Program step)
{
	switch (step.instr)
	{
	case inc_pointer:
		data_pointer += 1;

		if (data_pointer >= 30000)
		{
			data_pointer = 0;
		}
		break;
	case dec_pointer:
		data_pointer -= 1;

		if (data_pointer < 0)
		{
			data_pointer = 29999;
		}
		break;
	case inc_byte:
		data[data_pointer] += 1;
		break;
	case dec_byte:
		data[data_pointer] -= 1;
		break;
	case print:
		putchar(data[data_pointer]);
		break;
	case get:
		data[data_pointer] = getchar();
		break;
	case break_zero:
		if (data[data_pointer] == 0)
		{
			program_counter = step.destination;
		}
		break;
	case break_non_zero:
		if (data[data_pointer])
		{
			program_counter = step.destination - 1;
		}
		break;
	}
}

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		//REPL mode
	}
	else
	{
		program_counter = data_pointer = 0;

		FILE *file = fopen(argv[1], "rb");
		unsigned int prog_size = 0;

		if (file == NULL)
		{
			fprintf(stderr, "Error loading file: %s\n", argv[1]);
			exit(1);
		}

		prog_size = compile(false, file, &prog);

		if (prog == NULL)
		{
			fprintf(stderr, "Error in Compilation!\n");
			return 1;
		}

		while (program_counter < prog_size)
		{
			Program step = fetch();
			eval(step);
			program_counter++;
		}

		free(prog);

		return 0;
	}

	return 0;
}
