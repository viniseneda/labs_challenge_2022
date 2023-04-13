/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tables.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvarussa <vvarussa@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/27 20:37:14 by vvarussa          #+#    #+#             */
/*   Updated: 2022/02/27 20:37:17 by vvarussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "log_reader.h"

void	print_column_line(char **split_line, int x, t_parse_flags flags, int is_header)
{
	static char *table_headers[] = {"IP", "DATE", "METHOD", NULL, "URI", "aaa", "STATUS", "SIZE SENT"};
	static int padding[] = {9, 22, 6, 0, 20, 0, 10, 10};
	int n = 0;
	int index;
	int i = 0;

	while (n < x)
	{
		while (flags.fields[i] == 0)
			i++;
		index = flags.fields[i] - 1;
		if (is_header)
			printf("%-*s ", padding[index], table_headers[index]);
		else
			printf("%-*s ", padding[index], split_line[index]);
		i++;
		n++;
	}
	printf("\n");
}

int	is_line_selected(char **line, t_parse_flags flags)
{
	int a = 0;
	int b = 0;


	if (!flags.filters[0] && !flags.filters[1] && !flags.filters[2])
		a = 1;
	if (flags.filters[0] && strcmp(line[2], "GET") == 0)
		a = 1;
	if (flags.filters[1] && strcmp(line[2], "POST") == 0)
		a = 1;
	if (flags.filters[2] && strcmp(line[2], "PUT") == 0)
		a = 1;
	if (line[6] == NULL)
		b = 0;
	else
		if (check_status(atoi(line[6]), flags))
			b = 1;
	return (a && b);
}

void	make_std_table(int fd, t_parse_flags flags)
{
	int		routes[8];
	char	*line;
	char	**split_line;
	int		n;

	for(int i = 0; i < 8; i++)
		routes[i] = 0;

	n = get_next_line(fd, &line);
	while(n > 0)
	{
		split_line = ft_split(line, ' ');
		if (is_line_selected(split_line, flags))
			count_entry(split_line, routes);
		free(line);

		free_str_array(split_line);

		n = get_next_line(fd, &line);
	}
	free(line);
	print_std_table(routes);
}

void	print_std_table(int *routes)
{
	ft_table_t *table = ft_create_table();

	ft_set_border_style(table, FT_SOLID_STYLE);
	ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
	ft_write_ln(table, "TOTAL REQUESTS", "METHOD", "PATH");
	if (routes[7] > 0)
		ft_printf_ln(table, "%d|GET|/", routes[7]);
	if (routes[0] > 0)
		ft_printf_ln(table, "%d|GET|/books", routes[0]);
	if (routes[1] > 0)
		ft_printf_ln(table, "%d|GET|/location", routes[1]);
	if (routes[2] > 0)
		ft_printf_ln(table, "%d|GET|/book/:id", routes[2]);
	if (routes[3] > 0)
		ft_printf_ln(table, "%d|GET|/location/:id", routes[3]);
	if (routes[4] > 0)
		ft_printf_ln(table, "%d|POST|/book", routes[4]);
	if (routes[5] > 0)
		ft_printf_ln(table, "%d|POST|/location", routes[5]);
	if (routes[6] > 0)
		ft_printf_ln(table, "%d|PUT|/location/:id/book/:id", routes[6]);
	printf("%s", ft_to_string(table));
	ft_destroy_table(table);
}

void	make_table(int fd, t_parse_flags flags, int x)
{
	char	*line;
	char	**split_line;
	int		n;
	n = 0;

	n = get_next_line(fd, &line);
	split_line = ft_split(line, ' ');
	print_column_line(split_line, x, flags, 1);
	free_str_array(split_line);
	while(n > 0)
	{
		split_line = ft_split(line, ' ');
		if (strlen(line) != 0 && is_line_selected(split_line, flags))
			print_column_line(split_line, x, flags, 0);
		free_str_array(split_line);
		free(line);
		n = get_next_line(fd, &line);
	}
	free(line);
}
