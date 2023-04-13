/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bib_api.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvarussa <vvarussa@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/25 15:17:03 by vvarussa          #+#    #+#             */
/*   Updated: 2022/02/27 17:40:05 by vvarussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "api.h"
sqlite3	*g_db;
struct mg_mgr mgr;
struct	mg_connection *g_c;

void	sigint_handler(int sig)
{
	printf("Exiting server\n");
	mg_mgr_free(&mgr);
	sqlite3_close(g_db);
	exit(0);
}

void	log_connection(struct mg_connection *c, struct mg_http_message *hm, t_status status)
{
	char log[10000];
	char buf[100];
	char ti[1000];
	char *cutime;
	FILE *log_file;

	log_file = fopen("server.log", "a");

	time_t t = time(NULL);
	struct tm * p = localtime(&t);
	strftime(ti, 1000, "%d/%b/%Y:%X", p);

	fprintf(log_file, "%s ", mg_ntoa(&c->peer, buf, sizeof(buf)));
	fprintf(log_file, "[%s] ",ti);
	fprintf(log_file, "%.*s ", (int) hm->method.len, hm->method.ptr);
	fprintf(log_file, "%.*s ", (int) hm->proto.len, hm->proto.ptr);
	fprintf(log_file, "%.*s ", (int) hm->uri.len, hm->uri.ptr);
	fprintf(log_file, "%d ", status.path_code);
	fprintf(log_file, "%d ", status.status);
	fprintf(log_file, "%d\n", status.size_sent);

	fclose(log_file);
}

int	check_route(struct mg_http_message *hm, struct mg_str caps[3], char *path, char *method)
{
	return (mg_match(hm->uri, mg_str(path), caps) && mg_match(hm->method, mg_str(method), NULL));
}

static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
	t_status status;
	unsigned int size;

	g_c = c;
	if (ev == MG_EV_HTTP_MSG)
	{
		struct mg_str caps[3];
		struct mg_http_message *hm = (struct mg_http_message *) ev_data;

		if(check_route(hm, caps, "/book", "GET"))
			status = route_get_books(hm);
		else if(check_route(hm, caps, "/book/*", "GET"))
			status = route_get_book_locations(hm, caps);
		else if(check_route(hm, caps, "/location", "GET"))
			status = route_get_locations(hm);
		else if(check_route(hm, caps, "/location/*", "GET"))
			status = route_get_location_books(hm, caps);
		else if(check_route(hm, caps, "/book", "POST"))
			status = route_post_book(hm, caps);
		else if(check_route(hm, caps, "/location", "POST"))
			status = route_post_location(hm, caps);
		else if(check_route(hm, caps, "/location/*/book/*", "PUT"))
			status = route_put_book_location(hm, caps);
		else if(check_route(hm, caps, "/", "GET"))
			status = route_std(hm);
		else
			status = route_std_error(hm);
		log_connection(c, hm, status);
	}
}

char *get_host_ip()
{
	static char hostbuffer[256];
	char *IPbuffer;
	struct hostent *host_entry;
	int hostname;

	hostname = gethostname(hostbuffer, sizeof(hostbuffer));
	host_entry = gethostbyname(hostbuffer);
	IPbuffer = inet_ntoa(*((struct in_addr*)
		host_entry->h_addr_list[0]));
	sprintf(hostbuffer, "%s:8000", IPbuffer);
	return(hostbuffer);
}

int	main(void)
{
	// struct mg_mgr mgr;
	sqlite3	*db;
	int	rc;
	char *zErrMsg = 0;
	char *host_ip;

	signal(SIGINT, &sigint_handler);
	host_ip = get_host_ip();
	rc = sqlite3_open("test.db", &db);
	if(rc)
	{
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return(0);
	}
	else
		fprintf(stderr, "Opened database successfully\n");

	rc = sqlite3_exec(db, tables_info, NULL, 0, &zErrMsg);
	if( rc != SQLITE_OK )
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else
		fprintf(stdout, "Table created successfully\n");

	g_db = db;
	mg_mgr_init(&mgr);
	fprintf(stderr, "host: %s\n", host_ip);
	mg_http_listen(&mgr, host_ip, fn, &mgr);
	for (;;) mg_mgr_poll(&mgr, 10000);
	mg_mgr_free(&mgr);
	sqlite3_close(db);
	exit (0);
}
