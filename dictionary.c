#define _LARGE_FILE_API
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "helpers.c"
#include "types.h"

#define BLOCKSIZE 4096
#define LONGEST_EN_WORD 21

void read_until(int32_t fd, char *buf, uint64_t buf_size, uint64_t *offset)
{
	int32_t bytes_read = pread(fd, buf, buf_size, *offset);
	if (bytes_read <= 0)
	{
		buf[0] = '\0';
		return;
	}
	int64_t last_space_i;
	if ((uint64_t)bytes_read < buf_size)
		last_space_i = (uint64_t)bytes_read;
	else
		last_space_i = ft_last_occurrence_i(buf, (uint64_t)bytes_read, ' '); // MORE
	if (last_space_i == -1)
	{
		buf[0] = '\0';
		return;
	}
	*offset += last_space_i + 1;
	buf[last_space_i] = '\0';
}

char *get_word(char *str, uint64_t *len)
{
	uint64_t i;

	while (!ft_is_letter(*str))
	{
		if (*str == '\0')
			return (NULL);
		*str++;
	}
	i = 1;
	while (ft_is_letter(str[i]))
		i++;
	*len = i + 1;
	return (res);
}

uint64_t *occurences(int32_t fd, char **words, uint64_t n_words)
{
	char **words = malloc(n_words * sizeof(char *));
	char *buf = malloc(BLOCKSIZE * sizeof(char));
	uint64_t offset_file = 0;
	uint64_t offset_block = 0;

	while (true)
	{
		read_until(fd, buf, BLOCKSIZE, &offset);
		if (buf[0] == '\0')
			break;
		word_offset = 0;
		while (true)
	}

	uint64_t i = 0;
	while (i < n_words)
	{
		// printf("%lu\t\t<%s>\n", word_usages[i], words[i]);
		free(words[i]);
		i++;
	}
	free(words);
	free(word_usages);

	close(fd);
	free(buf);
	return (word_usages);
}

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	char *dictionary_path = "data/en.txt";
	char **words = ft_split()

	return (val);
}
