#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "Instructions.h"
#include "Compiler.h"

unsigned int compile(bool optimize, FILE *file, Program **prog)
{
	int c;
	unsigned int ic = 0, prog_size = 0, braces = 0;;
	unsigned int *index_to_match = NULL;

	/* Pass one, get the total length of the BrainFuck -- Ignore any non BF characters */
	while ((c = fgetc(file)) != EOF)
	{
		if (c == '+' || c == '-' || c == '>' || c == '<' || c == '[' || c == ']' || c == '.' || c == ',')
		{
			prog_size++;
		}

		if (c == '[')
		{
			braces++;
		}

	}

	c = 0;

	fseek(file, 0, SEEK_SET);

	if (*prog == NULL)
	{
		/* create an array of instructions to use */
		*prog = malloc(prog_size * sizeof(Program));
	}

	if (*prog == NULL)
	{
		fprintf(stderr, "Error allocating memory for program, exiting\n");
		return 3;
	}

	if (index_to_match == NULL)
	{
		index_to_match = malloc(braces * sizeof(int));
	}

	if (index_to_match == NULL)
	{
		fprintf(stderr, "Error allocating memory for brace stack, exiting\n");
		return 3;
	}

	while ((c = fgetc(file)) != EOF)
	{
		switch (c)
		{
		case '+':
			(*prog)[ic].instr = inc_byte;
			ic++;
			break;
		case '-':
			(*prog)[ic].instr = dec_byte;
			ic++;
			break;
		case '>':
			(*prog)[ic].instr = inc_pointer;
			ic++;
			break;
		case '<':
			(*prog)[ic].instr = dec_pointer;
			ic++;
			break;
		case '[':
			(*prog)[ic].instr = break_zero;
			(*prog)[ic].destination = -1;
			ic++;
			break;
		case ']':
			(*prog)[ic].instr = break_non_zero;
			(*prog)[ic].destination = -1;
			ic++;
			break;
		case '.':
			(*prog)[ic].instr = print;
			ic++;
			break;
		case ',':
			(*prog)[ic].instr = get;
			ic++;
			break;
		}
	}

	ic = 0;
	c = 0;

	/* Second pass -- Create links for the while loops */

	while (ic < prog_size)
	{
		if ((*prog)[ic].instr == break_zero)
		{
			index_to_match[c] = ic;
			c++;
		}
		else if ((*prog)[ic].instr == break_non_zero)
		{
			c--;
			(*prog)[ic].destination = index_to_match[c];
			(*prog)[index_to_match[c]].destination = ic;

			if (c < 0)
			{
				fprintf(stderr, "Error, unmatched braces on index %d\n", ic);
				free(*prog);
				free(index_to_match);
				fclose(file);
				exit(2);
			}
		}
		ic++;
	}

	fclose(file);
	free(index_to_match);
	return prog_size;
}