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
#include "couch.h"

couchdb_connection *connection_define(char *user, char *pass, char *token, char *host, long port, long timeout, _Bool format)
{
  couchdb_connection *conn = malloc(sizeof(couchdb_connection));
  conn->user = user;
  conn->pass = pass;
  conn->host = host;
  conn->port = port;
  conn->timeout = timeout;
  conn->token = token;
  conn->format = format;

  return conn;
}
couchdb_connection *connection_default(char *user, char *pass, char *host, long port, long timeout, _Bool format)
{
  return connection_define(user, pass, "", host, port, timeout, format);
}
couchdb_connection *connection_local(_Bool format)
{
  return connection_define("", "", "", COUCHDB_DEFAULT_HOST, COUCHDB_DEFAULT_PORT, COUCHDB_DEFAULT_TIMEOUT, format);
}
couchdb_connection *connection_token(char *token, char *host, long port, long timeout, _Bool format)
{
  return connection_define("", "", token, host, port, timeout, format);
}

couchdb_response *response_define(_Bool valid, char *msg, char *token)
{
  couchdb_response *r = malloc(sizeof(couchdb_response));

  if (valid)
  {
    STR_INIT(r->response.response, msg);
  }
  else
  {
    STR_INIT(r->response.error, msg);
  }
  r->valid = valid;

  if (token != NULL)
  {
    STR_INIT(r->token, token);
  }

  return r;
}

string_list *string_list_init(void)
{
  string_list *sl = malloc(sizeof(string_list));
  sl->count = 0;
  sl->list = NULL;

  return sl;
}
string_list *string_list_add(string_list *sl, char *v)
{
  sl->count++;
  sl->list = realloc(sl->list, sizeof(char *) * sl->count);
  // sl->list[sl->count - 1] = v;
  STR_INIT(sl->list[sl->count - 1], v);

  return sl;
}
string_list *string_list_add_n(string_list *sl, size_t count, ...)
{
  size_t idx = 0;
  va_list frags;

  va_start(frags, count);

  while (idx < count)
  {
    string_list_add(sl, va_arg(frags, char *));
    idx++;
  }

  va_end(frags);

  return sl;
}
char *string_list_concat(string_list *sl)
{
  char *res = calloc(1, 1);
  size_t len = 0;

  for (size_t idx = 0; idx < sl->count; idx++)
  {
    len += strlen(sl->list[idx]);
    res = realloc(res, len + 1);
    strcat(res, sl->list[idx]);
  }

  return res;
}
string_list *string_list_split(char *sep, char *text)
{
  char *data, *entry;

  STR_INIT(data, text);

  string_list *params = string_list_init();
  char *split = strtok(data, sep);

  while (split != NULL)
  {
    entry = calloc(1, strlen(split) + 1);
    strcpy(entry, split);

    string_list_add(params, split);

    split = strtok(NULL, sep);
  }

  free(data);

  return params;
}
char *extract_auth_token(char *headers)
{
  string_list *header_data = string_list_split("\n", headers);
  char *token;

  for (size_t i = 0; i < header_data->count; i++)
  {
    char *header = header_data->list[i];

    if (strstr(header, "Set-Cookie") || strstr(header, "set-cookie"))
    {
      string_list *entries = string_list_split("; ", header);
      STR_INIT(token, entries->list[1]);
    }
  }

  if (token == NULL || header_data->count == 0)
  {
    return NULL;
  }

  return token;
}

size_t curl_write_to_memory(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t real_size = size * nmemb;
  couchdb_curl *rsp = (couchdb_curl *)userp;

  char *ptr = realloc(rsp->data, rsp->size + real_size + 1);

  if (!ptr)
  {
    return 0;
  }

  rsp->data = ptr;
  memcpy(&(rsp->data[rsp->size]), contents, real_size);
  rsp->size += real_size;
  rsp->data[rsp->size] = 0;

  return real_size;
}
couchdb_response *couch_http_request(int method, couchdb_connection *conn, char *params, char *body)
{
  CURL *curl;
  CURLM *mcurl;
  CURLMcode status;
  int still_running = 0;
  _Bool auth_check = 0; // flag to check if session auth has been requested

  struct curl_slist *headers = NULL;

  couchdb_curl response, response_headers;
  couchdb_response *couch_response;
  response.data = calloc(1, 1);
  response.size = 0;
  response_headers.data = calloc(1, 1);
  response_headers.size = 0;

  curl_global_init(CURL_GLOBAL_ALL);

  curl = curl_easy_init();

  // condition the use of session auth if the endpoint dictates it
  if (strstr(params, "_session"))
  {
    auth_check = 1;
  }

  // write headers to memory to extract auth token
  if (auth_check)
  {
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, curl_write_to_memory);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, (void *)&response_headers);
  }

  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write_to_memory);

  // convert port to string
  char *port = calloc(1, sizeof(long *));
  sprintf(port, "%ld", conn->port);

  string_list *url = string_list_init();

  // handle situations where a forward slash is appended to host
  size_t host_len = strlen(conn->host);
  if (conn->host[host_len - 1] == '/')
  {
    char *couch_host = calloc(1, host_len);
    for (size_t i = 0; i < (host_len - 1); i++)
    {
      couch_host[i] = conn->host[i];
    }

    string_list_add(url, couch_host);
  }
  else
  {
    string_list_add(url, conn->host);
  }

  // append port to URI
  string_list_add_n(url, 2, ":", port);

  if (params != NULL)
  {
    string_list_add(url, params);
  }

  curl_easy_setopt(curl, CURLOPT_URL, string_list_concat(url));

  // condition the app to use basic auth if auth token is absent
  if (strlen(conn->user) > 0 || strlen(conn->pass) > 0 || strlen(conn->token) == 0)
  {
    string_list *credentials = string_list_init();
    string_list_add_n(credentials, 2, conn->user, conn->pass);

    curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
    curl_easy_setopt(curl, CURLOPT_USERPWD, string_list_concat(credentials));

    free(credentials);
  }

  // configure the app's HTTP method
  if (method == HTTP_POST || method == HTTP_PUT)
  {
    if (method == HTTP_POST)
    {
      curl_easy_setopt(curl, CURLOPT_POST, 1);
    }

    if (method == HTTP_PUT)
    {
      curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
    }

    if (body != NULL)
    {
      curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);
    }
  }

  if (method == HTTP_DELETE)
  {
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
  }

  // mandatory CouchDB headers
  headers = curl_slist_append(headers, "content-type: application/json");
  headers = curl_slist_append(headers, "accept: application/json");
  headers = curl_slist_append(headers, "user-agent: php-couchdb");

  // append token if it exists
  if (strlen(conn->token) > 0)
  {
    string_list *cookie = string_list_init();
    string_list_add_n(cookie, 2, "cookie: ", conn->token);

    headers = curl_slist_append(headers, string_list_concat(cookie));
    free(cookie);
  }

  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

  mcurl = curl_multi_init();
  curl_multi_add_handle(mcurl, curl);

  do
  {
    status = curl_multi_perform(mcurl, &still_running);

    if (!mcurl)
    {
      status = curl_multi_poll(mcurl, NULL, 0, conn->timeout * 1000, NULL);
    }

    if (status != CURLE_OK)
    {
      return response_define(0, (char *)curl_multi_strerror(status), NULL);
    }
  } while (still_running);

  if (auth_check)
  {
    // extract authorization token from headers
    couch_response = response_define(1, response.data, extract_auth_token(response_headers.data));
  }
  else
  {
    couch_response = response_define(1, response.data, NULL);
  }

  curl_multi_remove_handle(mcurl, curl);
  curl_easy_cleanup(curl);

  free(port);
  free(url);
  free(response.data);
  free(response_headers.data);

  curl_multi_cleanup(mcurl);
  curl_global_cleanup();
  curl_slist_free_all(headers);

  return couch_response;
}
