/*
   +----------------------------------------------------------------------+
   | Copyright (c) The PHP Group                                          |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | https://www.php.net/license/3_01.txt                                 |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Author: Lochemem Bruno Michael                                       |
   +----------------------------------------------------------------------+
*/

#ifndef COUCH_H
#define COUCH_H

#include "string.h"
#include "stdarg.h"
#include "curl/curl.h"
#include "stdarg.h"

#define COUCHDB_DEFAULT_PORT 5984
#define COUCHDB_DEFAULT_TIMEOUT 60
#define COUCHDB_DEFAULT_HOST "http://127.0.0.1"

struct couchdb_connection;
struct couchdb_response;
struct couchdb_curl;
struct string_list;

typedef struct couchdb_connection couchdb_connection;
typedef struct couchdb_response couchdb_response;
typedef struct couchdb_curl couchdb_curl;
typedef struct string_list string_list;

/* struct containing CouchDB connection parameters */
struct couchdb_connection
{
  /* CouchDB user information */
  char *user;
  /* CouchDB password information */
  char *pass;
  /* CouchDB host information */
  char *host;
  /* CouchDB auth token for session auth */
  char *token;
  /* CouchDB port information */
  long port;
  /* CouchDB request timeout */
  long timeout;
  /* CouchDB response format (0 for JSON (default) & 1 for array) */
  _Bool format;
};

/* struct containing CouchDB response information */
struct couchdb_response
{
  /* response data (one of either a valid response or error information) */
  union
  {
    char *response;
    char *error;
  } response;
  /* boolean flag to check if the data is either a valid response or undesirable error */
  _Bool valid;
  /* optional HTTP auth token */
  char *token;
};

/* struct containing CURL response data for CouchDB request */
struct couchdb_curl
{
  /* response data */
  char *data;
  /* byte size of response data */
  size_t size;
};

/* struct containing list of strings for arbitrary string manipulations */
struct string_list
{
  /* size of string list */
  size_t count;
  /* list of strings */
  char **list;
};

/* enum containing numeric encodings of HTTP request methods */
enum _http_methods
{
  HTTP_HEAD,
  HTTP_GET,
  HTTP_POST,
  HTTP_PUT,
  HTTP_DELETE,
} http_methods;

/* define CouchDB connection parameters */
couchdb_connection *connection_define(char *user, char *pass, char *token,
                                      char *host, long port, long timeout,
                                      _Bool format);
/* initialize connection with user and password credentials (default) */
couchdb_connection *connection_default(char *user, char *pass, char *host,
                                       long port, long timeout, _Bool format);
/* initialize connection with default connection parameters (ideal for local connections) */
couchdb_connection *connection_local(_Bool format);
/* initialize connection with authorization token */
couchdb_connection *connection_token(char *token, char *host, long port,
                                     long timeout, _Bool format);

/* subsumes response data in couchdb_response artifact */
couchdb_response *response_define(_Bool valid, char *msg, char *token);
/* extract auth token from response artifact */
char *extract_auth_token(char *headers);

/* initialize arbitrary string list */
string_list *string_list_init(void);
/* adds string to string list */
string_list *string_list_add(string_list *l, char *x);
/* add multiple strings, in a single function call, to a string list */
string_list *string_list_add_n(string_list *l, size_t count, ...);
/* concatenate strings in list - effectively flattening it to a single string */
char *string_list_concat(string_list *l);
/* separate string into tokens */
string_list *string_list_split(char *sep, char *text);

/* write contents of CURL response to memory */
size_t curl_write_to_memory(void *contents, size_t size, size_t nmemb, void *userp);
/* asynchronously - via CURL multihandle artifacts - make a request to CouchDB */
couchdb_response *couch_http_request(int method, couchdb_connection *conn, char *params, char *body);

#define STR_INIT(v, s)          \
  v = calloc(1, strlen(s) + 1); \
  strcpy(v, s);

#endif
