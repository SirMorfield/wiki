/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   helpers.h                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: joppe <joppe@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/10/19 15:44:32 by joppe         #+#    #+#                 */
/*   Updated: 2020/10/19 20:29:09 by joppe         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef HELPERS_H
# define HELPERS_H
# include "constants.h"

void	read_until(int32_t fd, char *buf, uint64_t buf_size, uint64_t *offset);
char	*get_word(char *str, uint64_t *len);
void	occurrences(int32_t fd, t_word *words, uint64_t num_words);

#endif
