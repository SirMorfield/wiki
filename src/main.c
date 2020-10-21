#define _LARGE_FILE_API
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include "types.h"
#include "ft_string.h"
#include "exit_error.h"
#include "ft_file.h"
#include "helpers.h"
#include "constants.h"

t_word	*get_words(uint64_t *num_words)
{
	char		**words_dictionary;
	t_word		*words;
	uint64_t	i;

	words_dictionary = ft_split_file(DICTIONARY_PATH, "\n");
	if (words_dictionary == NULL)
		exit_error("Dictionary not found\n");
	*num_words = 0;
	while (words_dictionary[*num_words] != NULL)
		(*num_words)++;
	words = malloc(*num_words * sizeof(t_word));
	i = 0;
	while (i < *num_words)
	{
		(words[i]).word = ft_strdup(words_dictionary[i]);
		(words[i]).len = ft_strlen(words_dictionary[i]);
		(words[i]).occurrences = 0;
		free(words_dictionary[i]);
		i++;
	}
	free(words_dictionary);
	return (words);
}

void	print_results(t_word *words, uint64_t num_words)
{
	uint64_t	i;
	t_word		word;
	uint64_t	spaces;

	i = 0;
	(void)num_words;
	while (i < num_words)
	{
		word = words[i];
		if (word.occurrences >= 1)
		{
			printf("%s", word.word);
			spaces = LONGEST_EN_WORD - word.len;
			while (spaces > 0)
			{
				spaces--;
				printf(" ");
			}
			printf("%lu\n", word.occurrences);
		}
		i++;
	}
}

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	t_word *words;
	uint64_t num_words;
	int32_t	fd;

	fd = open(WIKI_PATH, O_RDONLY);
	words = get_words(&num_words);
	occurrences(fd, words, num_words);
	print_results(words, num_words);
	return (0);
}
