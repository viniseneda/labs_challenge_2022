/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   api.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvarussa <vvarussa@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/25 15:16:44 by vvarussa          #+#    #+#             */
/*   Updated: 2022/02/27 20:39:31 by vvarussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef API_H
#define API_H
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include "mjson.h"
#include "mongoose.h"
#include "cJSON.h"
#define tables_info "CREATE TABLE location (\
	location_id INTEGER PRIMARY KEY,\
	location_name TEXT NOT NULL,\
	location_address TEXT NOT NULL);\
CREATE TABLE books (\
	book_id INTEGER PRIMARY KEY,\
	title TEXT NOT NULL,\
	author TEXT NOT NULL);\
CREATE TABLE book_locations(\
	book_id INTEGER,\
	location_id INTEGER,\
	PRIMARY KEY (book_id, location_id),\
	FOREIGN KEY (book_id)\
	REFERENCES location (location_id),\
	FOREIGN KEY (location_id)\
	REFERENCES books (book_id));"

typedef struct s_status
{
	int				status;
	unsigned int	size_sent;
	int				path_code;
} t_status;

/*print to json*/
char	*print_json_books(int rc, sqlite3_stmt *stmt, char *info);
char	*print_json_locations(int rc, sqlite3_stmt *stmt, char *info);
char	*print_basic_message();
char	*print_error_message(int status, char *error_message);

/*SQL actions*/
char	*get_books(sqlite3 *db);
char	*get_locations(sqlite3 *db);
char	*get_location_books(sqlite3 *db, int location_id);
char	*get_book_locations(sqlite3 *db, int book_id);
int		post_book(sqlite3 *db, char *title, char *author);
int		post_location(sqlite3 *db, char *location_name, char *location_address);
int		put_book_location(sqlite3 *db, int location_id, int book_id);

/*routes*/
t_status	route_get_books(struct mg_http_message *hm);
t_status	route_get_locations(struct mg_http_message *hm);
t_status	route_get_book_locations(struct mg_http_message *hm, struct mg_str caps[3]);
t_status	route_get_location_books(struct mg_http_message *hm, struct mg_str caps[3]);
t_status	route_post_book(struct mg_http_message *hm, struct mg_str caps[3]);
t_status	route_post_location(struct mg_http_message *hm, struct mg_str caps[3]);
t_status	route_put_book_location(struct mg_http_message *hm, struct mg_str caps[3]);
t_status	route_std_error(struct mg_http_message *hm);
t_status	route_std(struct mg_http_message *hm);

#endif
