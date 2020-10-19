/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exit_error.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: joppe <joppe@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/10/19 15:54:21 by joppe         #+#    #+#                 */
/*   Updated: 2020/10/19 20:33:25 by joppe         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

void	exit_error(char *msg)
{
	printf("%s", msg);
	printf("%s\n", strerror(errno));
	exit(1);
}
