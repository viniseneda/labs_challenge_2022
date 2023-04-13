/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_json.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvarussa <vvarussa@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/25 15:16:27 by vvarussa          #+#    #+#             */
/*   Updated: 2022/02/27 20:41:42 by vvarussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "api.h"

char *print_json_books(int rc, sqlite3_stmt *stmt, char *info)
{
	char *string = NULL;
	cJSON *books = NULL;
	cJSON *book = NULL;
    cJSON *id = NULL;
	cJSON *name = NULL;
    cJSON *title = NULL;
    cJSON *author = NULL;

	 cJSON *out = cJSON_CreateObject();
	if (out == NULL)
		exit(8);

	name = cJSON_CreateString(info);
	if (name == NULL)
		exit(8);

	cJSON_AddItemToObject(out, "data", name);

	books = cJSON_CreateArray();
	if (books == NULL)
		exit(8);

	cJSON_AddItemToObject(out, "books", books);

	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
	{
		int _id           = sqlite3_column_int (stmt, 0);
		const char *_title = sqlite3_column_text(stmt, 1);
		const char *_author = sqlite3_column_text(stmt, 2);

		book = cJSON_CreateObject();
		if (book == NULL)
			exit(8);
		cJSON_AddItemToArray(books, book);

		id = cJSON_CreateNumber(_id);
		if (id == NULL)
			exit(8);
		cJSON_AddItemToObject(book, "id", id);

		title = cJSON_CreateString(_title);
		if (title == NULL)
			exit(8);
		cJSON_AddItemToObject(book, "title", title);
	}
	string = cJSON_Print(out);
	cJSON_Delete(out);
	return(string);
}

char *print_json_locations(int rc, sqlite3_stmt *stmt, char *info)
{
	char *string = NULL;
	cJSON *locations = NULL;
	cJSON *location = NULL;
	cJSON *id = NULL;
	cJSON *name = NULL;
	cJSON *location_name = NULL;
	cJSON *adress = NULL;

	cJSON *out = cJSON_CreateObject();
	if (out == NULL)
		exit(8);

	name = cJSON_CreateString(info);
    if (name == NULL)
        exit(8);

	cJSON_AddItemToObject(out, "data", name);

	locations = cJSON_CreateArray();
    if (locations == NULL)
		exit(8);

	cJSON_AddItemToObject(out, "locations", locations);

	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
	{
		int _id           = sqlite3_column_int (stmt, 0);
		const char *_name = sqlite3_column_text(stmt, 1);
		const char *_adress = sqlite3_column_text(stmt, 2);

		location = cJSON_CreateObject();
        if (location == NULL)
			exit(8);
        cJSON_AddItemToArray(locations, location);

        id = cJSON_CreateNumber(_id);
        if (id == NULL)
			exit(8);
        cJSON_AddItemToObject(location, "id", id);

        location_name = cJSON_CreateString(_name);
        if (location_name == NULL)
			exit(8);
        cJSON_AddItemToObject(location, "location name", location_name);
	}
	string = cJSON_Print(out);
	cJSON_Delete(out);
	return(string);;
}

void	add_item(cJSON *host, cJSON **obj, char *label, char *content)
{
	*obj = cJSON_CreateString(content);
	if (obj == NULL)
		exit(8);

	cJSON_AddItemToObject(host, label, *obj);
}

char *print_basic_message()
{
	char *string = NULL;
	cJSON *obj;
	cJSON *procedure = NULL;

	cJSON *out = cJSON_CreateObject();
    if (out == NULL)
        exit(8);

	procedure = cJSON_CreateObject();
	if (procedure == NULL)
		exit(8);

	cJSON_AddItemToObject(out, "This API supports the following procedures", procedure);

	add_item(procedure, &obj, "GET /book", "get list of books");
	add_item(procedure, &obj, "GET /book/:id", "get list of location a given book is in");
	add_item(procedure, &obj, "GET /location", "get list of locations");
	add_item(procedure, &obj, "GET /location/:id", "get list of books in a given location");
	add_item(procedure, &obj, "POST /book", "add a book to the database");
	add_item(procedure, &obj, "POST /location", "add a location to the database");
	add_item(procedure, &obj, "PUT /location/:id/book/:id", "add a book to a location");

	string = cJSON_Print(out);
	cJSON_Delete(out);
	return(string);;
}

char *print_error_message(int status, char *error_message)
{
	char *string = NULL;
	cJSON *obj;
	cJSON *procedure = NULL;

	cJSON *out = cJSON_CreateObject();
    if (out == NULL)
        exit(8);

	procedure = cJSON_CreateObject();
	if (procedure == NULL)
		exit(8);

	cJSON_AddItemToObject(out, "Error", procedure);

	obj = cJSON_CreateNumber((double) status);
	if (obj == NULL)
		exit(8);

	cJSON_AddItemToObject(procedure, "status code", obj);

	add_item(procedure, &obj, "description", error_message);

	string = cJSON_Print(out);
	cJSON_Delete(out);
	return(string);
}
