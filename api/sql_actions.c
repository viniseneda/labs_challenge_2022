#include "api.h"

char	*get_books(sqlite3 *db)
{
	char *sql;
	sqlite3_stmt *stmt;
	char *json_str;
	int rc;

	sql = "SELECT * FROM books;";

	/* Execute SQL statement */
	rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	if (rc != SQLITE_OK)
		return (NULL);
	json_str = print_json_books(rc, stmt, "list of books");
	sqlite3_finalize(stmt);
	return (json_str);
}

char	*get_locations(sqlite3 *db)
{
	char *sql;
	sqlite3_stmt *stmt;
	char *json_str;
	int rc;


	sql = "SELECT * FROM location;";

	/* Execute SQL statement */
	rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	if (rc != SQLITE_OK)
		return (NULL);
	json_str = print_json_locations(rc, stmt, "list of locations");
	sqlite3_finalize(stmt);
	return (json_str);
}

char	*get_location_books(sqlite3 *db, int location_id)
{
	char sql[200];
	sqlite3_stmt *stmt;
	char *json_str;
	int rc;

	sprintf(sql, "SELECT * FROM books WHERE book_id IN (SELECT book_id FROM book_locations WHERE location_id=%d);", location_id);

	/* Execute SQL statement */
	rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	if (rc != SQLITE_OK)
		return (NULL);
	json_str = print_json_locations(rc, stmt, "list books in given location");
	sqlite3_finalize(stmt);
	return (json_str);
}

char	*get_book_locations(sqlite3 *db, int book_id)
{
	char sql[200];
	sqlite3_stmt *stmt;
	char *json_str;
	int rc;

	sprintf(sql, "SELECT * FROM location WHERE location_id IN (SELECT location_id FROM book_locations WHERE book_id=%d);", book_id);

	/* Execute SQL statement */
	rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	if (rc != SQLITE_OK)
		return (NULL);
	json_str = print_json_locations(rc, stmt, "list of locations a given book is in");
	sqlite3_finalize(stmt);
	return (json_str);
}

int	post_book(sqlite3 *db, char *title, char *author)
{
	char sql[200];
	char *zErrMsg = 0;
	int rc;

	sprintf(sql, "INSERT INTO books (title, author) "  \
			"VALUES ('%s', '%s');", title, author);

	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
	if (rc != SQLITE_OK)
		return (0);
	return (1);
}

int	post_location(sqlite3 *db, char *location_name, char *location_address)
{
	char sql[200];
	char *zErrMsg = 0;
	int rc;

	sprintf(sql, "INSERT INTO location (location_name, location_address) "  \
			"VALUES ('%s', '%s');", location_name, location_address);

	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
	if (rc != SQLITE_OK)
		return (0);
	return (1);
}

int	put_book_location(sqlite3 *db, int location_id, int book_id)
{
	char sql[200];
	char *zErrMsg = 0;
	int rc;

	sprintf(sql, "INSERT INTO book_locations (location_id, book_id) "  \
			"VALUES (%d,%d);", location_id, book_id);

	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
	if (rc != SQLITE_OK)
		return (0);
	return (1);
}


