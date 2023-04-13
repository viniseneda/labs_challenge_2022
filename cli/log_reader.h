/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log_reader.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvarussa <vvarussa@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/27 20:37:22 by vvarussa          #+#    #+#             */
/*   Updated: 2022/02/27 20:37:24 by vvarussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOG_READER_H
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <fcntl.h>
#include "libft.h"
#include "fort.h"

typedef struct s_parse_flags
{
	int fields[6];
	int filters[3];
	int	status[50];
} t_parse_flags;

/*parsing*/
t_parse_flags	init_flags(t_parse_flags flags);
void			print_flags(t_parse_flags flags);
t_parse_flags	parse_fields(char *str, t_parse_flags flags);
t_parse_flags	parse_filter(char *str, t_parse_flags flags);
t_parse_flags	parse_flags(char **argv);

/*tables*/
int				is_line_selected(char **line, t_parse_flags flags);
void			make_std_table(int fd, t_parse_flags flags);
void			print_std_table(int *routes);
void			make_table(int fd, t_parse_flags flags, int x);

/*utils*/
void			free_str_array(char **args);
int				check_status(int status, t_parse_flags flags);
void			count_entry(char **split_line, int *routes);
int				count_number_columns(t_parse_flags flags);


#endif
