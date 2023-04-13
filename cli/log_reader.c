/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log_reader.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvarussa <vvarussa@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/25 09:16:36 by vvarussa          #+#    #+#             */
/*   Updated: 2022/02/27 20:37:03 by vvarussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "log_reader.h"

void	process_data(int fd, t_parse_flags flags)
{
	int		x;

	x = count_number_columns(flags);

	if(x == 0)
		make_std_table(fd, flags);
	else
		make_table(fd, flags, x);
}

int	main(int argc, char **argv)
{
	t_parse_flags	flags;
	int				fd;

	if (argc < 2)
	{
		printf("use ./cli file.log + optional flags + optional filters\n");
		exit (3);
	}

	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
	{
		printf("couldn't open log file\n");
		printf("use ./cli file.log + optional flags\n");
		exit(3);
	}

	flags = parse_flags(argv);
	process_data(fd, flags);
	return (0);
}
