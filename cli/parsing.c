/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvarussa <vvarussa@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/27 20:37:38 by vvarussa          #+#    #+#             */
/*   Updated: 2022/02/27 20:37:40 by vvarussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "log_reader.h"

t_parse_flags	init_flags(t_parse_flags flags)
{
	for (int i = 0; i < 6; i++)
		flags.fields[i] = 0;
	for (int i = 0; i < 3; i++)
		flags.filters[i] = 0;
	for (int i = 0; i < 50; i++)
		flags.status[i] = 0;
	return (flags);
}

void	print_flags(t_parse_flags flags)
{
	for (int i = 0; i < 6; i++)
		printf("(%d, %d)", i, flags.fields[i]);
	printf("\n");
	for (int i = 0; i < 3; i++)
		printf("(%d, %d)", i, flags.filters[i]);
	for (int i = 0; flags.status[i] != 0; i++)
		printf("(%d, %d)", i, flags.status[i]);
	printf("\n");
}

t_parse_flags	parse_fields(char *str, t_parse_flags flags)
{
	while(*str)
	{
		if (*str == 'i')
			flags.fields[0] = 1;
		else if (*str == 'd')
			flags.fields[1] = 2;
		else if (*str == 'm')
			flags.fields[2] = 3;
		else if (*str == 'p')
			flags.fields[3] = 5;
		else if (*str == 's')
			flags.fields[4] = 7;
		else if (*str == 'r')
			flags.fields[5] = 8;
		str++;
	}
	return (flags);
}

t_parse_flags	parse_filter(char *str, t_parse_flags flags)
{
	static int	st_num;

	str++;
	if(strcmp(str, "get") == 0)
		flags.filters[0] = 1;
	else if(strcmp(str, "post") == 0)
		flags.filters[1] = 1;
	else if(strcmp(str, "put") == 0)
		flags.filters[2] = 1;
	else if(isdigit(*str))
	{
		flags.status[st_num] = atoi(str);
		st_num++;
	}
	return (flags);
}

t_parse_flags parse_flags(char **argv)
{
	t_parse_flags flags;

	flags = init_flags(flags);
	argv++;
	argv++;
	while (*argv)
	{
		if (**argv == '-')
		{
			(*argv)++;
			if (**argv == '-')
				flags = parse_filter(*argv, flags);
			else
				flags = parse_fields(*argv, flags);
		}
		argv++;
	}
	return (flags);
}
