/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvarussa <vvarussa@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/27 20:37:30 by vvarussa          #+#    #+#             */
/*   Updated: 2022/02/27 20:37:32 by vvarussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "log_reader.h"

void	print_str_array(char **args)
{
	int i;

	i = 0;
	while(args[i] != NULL)
	{
		printf("%s, ", args[i]);
		i++;
	}
	printf ("\n");
}

void	free_str_array(char **args)
{
	int i;

	i = 0;
	while(args[i] != NULL)
	{
		free(args[i]);
		i++;
	}
	free(args);
}

int	check_status(int status, t_parse_flags flags)
{
	if (flags.status[0] == 0)
		return (1);
	for (int i = 0; flags.status[i] != 0; i++)
	{
		if (flags.status[i] == status)
			return (1);
	}
	return (0);
}

void	count_entry(char **split_line, int *routes)
{
	int n;
	n =	atoi(split_line[5]) - 1;
	if (n >= 0)
		routes[n]++;
}

int		count_number_columns(t_parse_flags flags)
{
	int n = 0;

	for (int i = 0; i < 6; i++)
		if(flags.fields[i] > 0)
			n++;
	return (n);
}
