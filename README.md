# LABS CHALLENGE 2022

This is the code for a (very) simple “library system” API built according to the REST architecture. It uses SQLite for its database system and the [mongoose](https://mongoose.ws/) library for the http server. There is also a CLI for the interactive reading and interpretation of the log file produced by the application.

for more information on the development process look at the [walkthough.md](./walkthrough.md) file

both programs can be compiled with the given makefiles, the only requirements are the SQLite libraries for C, which can be installed with:

> apt-get install libsqlite3-dev

#### DOCKER
there is a dockerfile available for a containered version of the app. If you have docker installed, you can make the image and run the container.

## API
### usage

All the GET methods return a json data in the body of the http repply

the API accepts the following endpoints:

#### GET /

shows a summary of the API endpoints

#### GET /book

list all the books in the database as an array of “books” objects in the following json format:

``{"id": 1,"title": "vidas secas"}``

#### GET /location

list all the locations in the database as an array of “locations” objects in the following json format:

``{
	"id": 1,
     "location name": "loja principal"
}``

#### GET /book/:id

list all the locations a given book is in

#### GET /location/:id

list all the books in a given location

#### POST /book

Accepts the following json message:

``{“title”:”book title”,”author”:”book author”}``

and adds a book to the database

#### POST /location

Accepts the following json message:

``{“name”:”location name”,”address”:”location address”}``

and adds a location to the database

#### PUT /location/:id/book/:id

Add a relation between a book and a location

## CLI
### usage

	./log_reader + log file + optional_flags + optional filters

#### filters:
	--get
	--post
	--put
	--(status code number) (eg: --400, --201 …)

#### flags:
	-i    shows the IP
	-d    shows the DATE
	-m    shows the METHOD
	-p    shows the PATH
	-s    shows the STATUS code
	-r    shows the size of the reply in bytes

when run without any flags the cli show an overview of the amount of given request to each one of the paths:

```
┌────────────────┬────────┬────────────────────────┐
│ TOTAL REQUESTS │ METHOD │ PATH                   │
├────────────────┼────────┼────────────────────────┤
│ 1              │ GET    │ /                      │
│ 6              │ GET    │ /books                 │
│ 4              │ GET    │ /location              │
│ 4              │ GET    │ /book/:id              │
│ 6              │ GET    │ /location/:id          │
│ 2              │ POST   │ /book                  │
│ 11             │ POST   │ /location              │
│ 4              │ PUT    │ /location/:id/book/:id │
└────────────────┴────────┴────────────────────────┘
```

but when one or more flags are applied the cli shows the selected fields of the filtered log:

```
$ ./log_reader server.log -mps --get --400
METHOD URI                  STATUS
GET    /56                  400
GET    /books               400
GET    /book/0              400
GET    /location/po         400
```
