#define _LARGE_FILE_API
#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "ft_string.h"

#define BLOCKSIZE 4096
#define DICTIONARY_PATH "data/en.txt"

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	char **words;

	words = ft_split_file(DICTIONARY_PATH, "\n");


	return (0);
}
