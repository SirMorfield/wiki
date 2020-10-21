/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   helpers.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: joppe <joppe@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/10/19 15:44:18 by joppe         #+#    #+#                 */
/*   Updated: 2020/10/21 17:42:59 by joppe         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#define _LARGE_FILE_API
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "types.h"
#include "helpers.h"
#include "constants.h"
#include "ft_string.h"
#include "exit_error.h"

int64_t	last_seperator(char *str, uint64_t len, char *seperators)
{
	uint64_t	seperator_i;

	while (len > 0)
	{
		len--;
		seperator_i = 0;
		while (seperators[seperator_i] != '\0')
		{
			if (str[len] == seperators[seperator_i])
				return (len);
			seperator_i++;
		}
	}
	return (-1);
}

clock_t g_prev_print = 0;

void	print_progress(uint64_t offset)
{
	clock_t now = clock();
	double time_spent = (double)(now - g_prev_print) / CLOCKS_PER_SEC;
	if (time_spent > 3)
	{
		printf("Progress %lu MiB \n", offset / 1024 / 1024);
		g_prev_print = now;
	}
}

void	read_until(int32_t fd, char *buf, uint64_t buf_size, uint64_t *offset)
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
		last_space_i = last_seperator(buf, (uint64_t)bytes_read, " ,.\n)-");
	if (last_space_i == -1)
	{
		buf[0] = '\0';
		return;
	}
	*offset += last_space_i + 1;
	buf[last_space_i] = '\0';
	print_progress(*offset);
}

bool	is_lowercase_letter(char c)
{
	return (c >= 'a' && c <= 'z');
}

char	*get_word(char *str, uint64_t *len, uint64_t *buf_offset)
{
	char	*start;

	while (!is_lowercase_letter(*str))
	{
		(*buf_offset)++;
		if (*str == '\0')
		{
			*len = 0;
			return (NULL);
		}
		str++;
	}
	start = str;
	*len = 1;
	while (is_lowercase_letter(str[*len]))
		(*len)++;
	(*buf_offset) += *len;
	return (start);
}

void	search_word(t_word *words, uint64_t num_words, char *potential_word, uint64_t potential_word_len)
{
	int64_t 	low;
	int64_t 	high;
	int64_t		mid;
	int8_t		cmp;

	if (potential_word_len == 0)
		return ;
	low = 0;
	high = (int64_t)num_words - 1;
	while (low <= high)
	{
		mid = (low + high) / 2;
		cmp = ft_strncmp(potential_word, words[mid].word, potential_word_len);
		if (cmp == 0 && words[mid].word[potential_word_len] != '\0')
			cmp = -((int8_t)(words[mid].word[potential_word_len]));
		if (cmp < 0)
			high = mid - 1;
		else if (cmp > 0)
			low = mid + 1;
		else
		{
			words[mid].occurrences += 1;
			return ;
		}
	}
}

void	occurrences(int32_t fd, t_word *words, uint64_t num_words)
{
	char		*buf;
	uint64_t	file_offset;
	uint64_t	buf_offset;
	char		*potential_word;
	uint64_t	potential_word_len;

	buf = malloc(BLOCKSIZE * sizeof(char));
	buf[0] = 'x';
	file_offset = 0;
	while (buf[0] != '\0')
	{
		read_until(fd, buf, BLOCKSIZE, &file_offset);
		buf_offset = 0;
		while (true)
		{
			potential_word = get_word(buf + buf_offset, &potential_word_len, &buf_offset);
			if (potential_word == NULL)
				break ;
			search_word(words, num_words, potential_word, potential_word_len);
		}
	}
	free(buf);
}
