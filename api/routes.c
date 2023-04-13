/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvarussa <vvarussa@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/25 14:57:14 by vvarussa          #+#    #+#             */
/*   Updated: 2022/02/27 20:42:26 by vvarussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include		"api.h"
extern sqlite3	*g_db;
extern struct	mg_connection *g_c;

t_status route_get_books(struct mg_http_message *hm)
{
	t_status status;
	char *json_reply;

	json_reply = get_books(g_db);
	status.size_sent = strlen(json_reply);
	mg_http_reply(g_c, 200, "Content-type: application/json\r\n", "%s", json_reply);
	status.status = 200;
	free(json_reply);
	status.path_code = 1;
	return (status);
}

t_status route_get_locations(struct mg_http_message *hm)
{
	t_status status;
	char *json_reply;

	json_reply = get_locations(g_db);
	status.size_sent = strlen(json_reply);
	mg_http_reply(g_c, 200, "Content-type: application/json\r\n", "%s", json_reply);
	status.status = 200;
	free(json_reply);
	status.path_code = 2;
	return (status);
}

t_status route_get_book_locations(struct mg_http_message *hm, struct mg_str caps[3])
{
	t_status status;
	char	temp[100];
	char	*json_reply;
	int		book_id;

	sprintf(temp, "%.*s", (int) caps[0].len, caps[0].ptr);
	book_id = atoi(temp);
	if(book_id > 0 && (json_reply = get_book_locations(g_db, book_id)))
	{
		status.size_sent = strlen(json_reply);
		mg_http_reply(g_c, 200, "Content-type: application/json\r\n", "%s", json_reply);
		free(json_reply);
		status.status = 200;
	}
	else
	{
		json_reply = print_error_message(400, "invalid book id");
		mg_http_reply(g_c, 400, "Content-type: application/json\r\n", "%s", json_reply);
		free(json_reply);
		status.size_sent = strlen(json_reply);
		status.status = 400;
	}
	status.path_code = 3;
	return (status);
}

t_status route_get_location_books(struct mg_http_message *hm, struct mg_str caps[3])
{
	t_status	status;
	char		temp[100];
	char		*json_reply;
	int			location_id;

	sprintf(temp, "%.*s", (int) caps[0].len, caps[0].ptr);
	location_id = atoi(temp);
	if(location_id > 0 && (json_reply = get_location_books(g_db, location_id)))
	{
		status.size_sent = strlen(json_reply);
		mg_http_reply(g_c, 200, "Content-type: application/json\r\n", "%s", json_reply);
		free(json_reply);
		status.status = 200;
	}
	else
	{
		json_reply = print_error_message(400, "invalid location id");
		status.size_sent = strlen(json_reply);
		mg_http_reply(g_c, 400, "Content-type: application/json\r\n", "%s", json_reply);
		free(json_reply);
		status.status = 400;
	}
	status.path_code = 4;
	return (status);
}

t_status route_post_book(struct mg_http_message *hm, struct mg_str caps[3])
{
	t_status	status;
	char		*json_reply;
	char		title[200];
	char		author[200];

	if (mjson_get_string(hm->body.ptr, (int) hm->body.len, "$.title", title, sizeof(title)) >= 0 &&
	mjson_get_string(hm->body.ptr, (int) hm->body.len, "$.author", author, sizeof(author)) >= 0)
	{
		post_book(g_db, title, author);
		mg_http_reply(g_c, 201, "Content-type: text/plain\r\n", "");
		status.status = 201;
		status.size_sent = 0;
	}
	else
	{
		json_reply = print_error_message(400, "wrong format. format as {\"title\":data,\"author\":data}");
		status.size_sent = strlen(json_reply);
		mg_http_reply(g_c, 400, "Content-type: text/plain\r\n", "%s", json_reply);
		free(json_reply);
		status.status = 400;
	}
	status.path_code = 5;
	return (status);
}

t_status route_post_location(struct mg_http_message *hm, struct mg_str caps[3])
{
	t_status	status;
	char		*json_reply;
	char		name[200];
	char		address[200];

	if (mjson_get_string(hm->body.ptr, (int) hm->body.len, "$.name", name, sizeof(name)) >= 0 &&
	mjson_get_string(hm->body.ptr, (int) hm->body.len, "$.address", address, sizeof(address)) >= 0)
	{
		post_location(g_db, name, address);
		mg_http_reply(g_c, 201, "Content-type: text/plain\r\n", "");
		status.size_sent = 0;
		status.status = 201;
	}
	else
	{
		json_reply = print_error_message(400, "wrong format. format as {\"name\":data,\"address\":data}");
		status.size_sent = strlen(json_reply);
		mg_http_reply(g_c, 400, "Content-type: text/plain\r\n", "%s", json_reply);
		free(json_reply);
		status.status = 400;
	}
	status.path_code = 6;
	return(status);
}

t_status route_put_book_location(struct mg_http_message *hm, struct mg_str caps[3])
{
	t_status	status;
	char		a[200];
	char		b[200];
	char		*json_reply;
	int			book_id, location_id;

	*a = '\0';
	*b = '\0';
	sprintf(a, "%.*s", (int) caps[0].len, caps[0].ptr);
	sprintf(b, "%.*s", (int) caps[1].len, caps[1].ptr);
	location_id = atoi(a);
	book_id = atoi(b);
	if(location_id != 0 && book_id != 0)
	{
		put_book_location(g_db, location_id, book_id);
		mg_http_reply(g_c, 201, "Content-type: text/plain\r\n", "");
		status.size_sent = 0;
		status.status = 201;
	}
	else
	{
		json_reply =  print_error_message(400, "wrong or imposible ids");
		status.size_sent = strlen(json_reply);
		mg_http_reply(g_c, 400, "Content-type: text/plain\r\n", "%s", json_reply);
		free(json_reply);
		status.status = 400;
	}
	status.path_code = 7;
	return(status);
}

t_status route_std(struct mg_http_message *hm)
{
	t_status status;
	char *json_reply;

	json_reply = print_basic_message();
	status.size_sent = strlen(json_reply);
	mg_http_reply(g_c, 200, "Content-type: application/json\r\n", "%s", json_reply);
	free(json_reply);
	status.status = 200;
	status.path_code = 8;
	return (status);
}

t_status route_std_error(struct mg_http_message *hm)
{
	t_status status;
	char *json_reply;

	json_reply =  print_error_message(400, "invalid request");
	status.size_sent = strlen(json_reply);
	mg_http_reply(g_c, 400, "Content-type: text/plain\r\n", "%s", json_reply);
	free(json_reply);
	status.status = 400;
	status.path_code = 0;
	return (status);
}
