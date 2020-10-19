/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   constants.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: joppe <joppe@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/10/19 15:52:09 by joppe         #+#    #+#                 */
/*   Updated: 2020/10/19 21:03:45 by joppe         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONSTANTS_H
# define CONSTANTS_H

#define BLOCKSIZE 4096
#define DICTIONARY_PATH "data/en_alphabetically.txt"
#define WIKI_PATH "data/cleanWiki.txt"
#define LONGEST_EN_WORD 23

typedef	struct	s_word
{
	char *word;
	uint64_t len;
	uint64_t occurrences;
}				t_word;

#endif
