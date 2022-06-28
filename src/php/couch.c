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

zend_object *php_couchdb_connection_create_object_ex(couchdb_connection *c)
{
  php_couchdb_connection *obj = zend_object_alloc(sizeof(php_couchdb_connection), couch_ce);
  zend_object_std_init(&obj->std, couch_ce);
  obj->std.handlers = &couchdb_object_handlers;
  obj->conn = c;

  return &obj->std;
}
zend_object *php_couchdb_connection_create_object(zend_class_entry *ce)
{
  return php_couchdb_connection_create_object_ex(connection_local(0));
}

void php_couchdb_connection_free_object(zend_object *object)
{
  php_couchdb_connection *intern = php_couchdb_obj_from_obj(object);
  free(intern->conn);

  zend_object_std_dtor(&intern->std);
}
void php_couchdb_connection_destroy_object(zend_object *object)
{
  php_couchdb_connection *intern = (php_couchdb_connection *)((char *)object -
                                                              XtOffsetOf(php_couchdb_connection, std));
  zend_object_std_dtor(object);
}
#if ZEND_MODULE_API_NO < ZEND_API_PHP80
HashTable *php_couchdb_get_properties(zval *object)
#else
HashTable *php_couchdb_get_properties(zend_object *object)
#endif
{
  HashTable *props;

  zval *key;
  zval *value;

  zval config;
  array_init(&config);

#if ZEND_MODULE_API_NO < ZEND_API_PHP80
  php_couchdb_connection *couch = COUCHDB_OBJ(object);
#else
  php_couchdb_connection *couch = php_couchdb_obj_from_obj(object);
#endif

  add_assoc_string(&config, "user", couch->conn->user);
  add_assoc_string(&config, "password", couch->conn->pass);
  add_assoc_string(&config, "token", couch->conn->token);
  add_assoc_string(&config, "host", couch->conn->host);
  add_assoc_long(&config, "timeout", couch->conn->timeout);
  add_assoc_long(&config, "port", couch->conn->port);
  add_assoc_string(&config, "type", couch->conn->format ? "array" : "JSON");

  ALLOC_HASHTABLE(props);
  zend_hash_init(props, 7, NULL, ZVAL_PTR_DTOR, 0);

  zend_hash_next_index_insert(props, &config);

  return props;
}

zval *convert_couchdb_response_to_array(char *response, zval *output)
{
  char *data;
  STR_INIT(data, response);

  if (php_json_decode_ex(output, data, strlen(data), PHP_JSON_OBJECT_AS_ARRAY, 512) == FAILURE)
  {
    COUCHDB_THROW("Could not decode JSON");
  }

  free(data);

  return output;
}
zend_string *php_build_query(zval *list)
{
  smart_str query = {0};
  zend_string *ret;

  php_url_encode_hash_ex(Z_ARRVAL_P(list),
                         &query, NULL, 0, NULL, 0, NULL, 0,
                         (Z_TYPE_P(list) == IS_OBJECT ? list : NULL),
                         NULL, 1);

  smart_str_0(&query);

  ret = zend_string_init(ZSTR_VAL(query.s), ZSTR_LEN(query.s), 1);

  smart_str_free(&query);
  efree(query.s);

  return ret;
}
char *php_json_stringify(zval *list)
{
  smart_str data = {0};
  php_json_encode(&data, list, 0);

  smart_str_0(&data);
  char *json = ZSTR_VAL(data.s);

  smart_str_free(&data);
  return json;
}
void php_ht_convert_bool_to_string(zval *list, zval *output)
{
  zend_string *key;
  zval *val;

  ZEND_HASH_FOREACH_STR_KEY_VAL(Z_ARRVAL_P(list), key, val)
  {
    if (Z_TYPE_P(val) == IS_FALSE)
    {
      add_assoc_string(output, ZSTR_VAL(key), "false");
    }
    else if (Z_TYPE_P(val) == IS_TRUE)
    {
      add_assoc_string(output, ZSTR_VAL(key), "true");
    }
    else
    {
      add_assoc_zval(output, ZSTR_VAL(key), val);
    }
  }
  ZEND_HASH_FOREACH_END();
}
char *extract_couchdb_response_error(char *response)
{
  zval output;
  char *msg;
  char *error;
  char *reason;
  size_t msg_len = 0;
  zend_string *db_error;
  zend_string *db_reason;

  Z_STR_DEF(db_error, "error");
  Z_STR_DEF(db_reason, "reason");

  convert_couchdb_response_to_array(response, &output);
  STR_INIT(error, HT_GET_STRVAL(&output, db_error));
  STR_INIT(reason, HT_GET_STRVAL(&output, db_reason));

  msg_len += strlen(error) + strlen(reason) + 3;
  msg = calloc(1, msg_len);
  sprintf(msg, "(%s) %s", error, reason);

  zend_string_release(db_error);
  zend_string_release(db_reason);

  free(error);
  free(reason);

  return msg;
}

void couchdb_connect(INTERNAL_FUNCTION_PARAMETERS)
{
  zval *db_config;
  zend_string *db_user, *db_pass, *db_host, *db_port, *db_timeout, *db_return_type;

  php_couchdb_connection *couch;
  couchdb_connection *tmp;

  ZEND_PARSE_PARAMETERS_START(0, 1)
  Z_PARAM_OPTIONAL
  Z_PARAM_ARRAY(db_config)
  ZEND_PARSE_PARAMETERS_END();

  Z_STR_DEF(db_user, "user");
  Z_STR_DEF(db_pass, "pass");
  Z_STR_DEF(db_host, "host");
  Z_STR_DEF(db_port, "port");
  Z_STR_DEF(db_return_type, "type");
  Z_STR_DEF(db_timeout, "timeout");

  if (Z_TYPE_P(db_config) != IS_ARRAY)
  {
    tmp = connection_local(0);
  }
  else
  {
    tmp = connection_default(HT_KEY_EXISTS(db_config, db_user)
                                 ? HT_GET_STRVAL(db_config, db_user)
                                 : "",
                             HT_KEY_EXISTS(db_config, db_pass)
                                 ? HT_GET_STRVAL(db_config, db_pass)
                                 : "",
                             HT_KEY_EXISTS(db_config, db_host)
                                 ? HT_GET_STRVAL(db_config, db_host)
                                 : COUCHDB_DEFAULT_HOST,
                             HT_KEY_EXISTS(db_config, db_port)
                                 ? HT_GET_LVAL(db_config, db_port)
                                 : COUCHDB_DEFAULT_PORT,
                             HT_KEY_EXISTS(db_config, db_timeout)
                                 ? HT_GET_LVAL(db_config, db_timeout)
                                 : COUCHDB_DEFAULT_TIMEOUT,
                             HT_KEY_EXISTS(db_config, db_return_type)
                                 ? HT_GET_LVAL(db_config, db_return_type)
                                 : 0);
  }

  object_init_ex(return_value, couch_ce);
  couch = COUCHDB_OBJ(return_value);

  couch->conn = tmp;

  zend_string_release(db_user);
  zend_string_release(db_pass);
  zend_string_release(db_host);
  zend_string_release(db_port);
  zend_string_release(db_return_type);
  zend_string_release(db_timeout);
}
void couchdb_session_init(INTERNAL_FUNCTION_PARAMETERS)
{
  zval *obj = getThis();
  zval output;
  php_couchdb_connection *couch;
  couchdb_connection *tmp;

  ZEND_PARSE_PARAMETERS_NONE();

  couch = COUCHDB_OBJ(obj);

  if (couch != NULL)
  {
    zval body;
    array_init(&body);
    add_assoc_string(&body, "name", couch->conn->user);
    add_assoc_string(&body, "password", couch->conn->pass);

    couchdb_response *resp = couch_http_request(HTTP_POST, couch->conn, "/_session",
                                                php_json_stringify(&body));

    if (resp->valid)
    {
      if (resp->token != NULL)
      {
        tmp = connection_token(resp->token, couch->conn->host, couch->conn->port,
                               couch->conn->timeout, couch->conn->format);
      }
      else
      {
        tmp = connection_define(couch->conn->user, couch->conn->pass, couch->conn->token,
                                couch->conn->host, couch->conn->port, couch->conn->timeout,
                                couch->conn->format);
      }
    }
    else
    {
      tmp = connection_define(couch->conn->user, couch->conn->pass, couch->conn->token,
                              couch->conn->host, couch->conn->port, couch->conn->timeout,
                              couch->conn->format);
    }

    ZVAL_OBJ(return_value, php_couchdb_connection_create_object_ex(tmp));
  }
}
void couchdb_is_available(INTERNAL_FUNCTION_PARAMETERS)
{
  zval *obj = getThis();
  php_couchdb_connection *couch;

  ZEND_PARSE_PARAMETERS_NONE();

  couch = COUCHDB_OBJ(obj);

  if (couch != NULL)
  {
    couchdb_response *resp = couch_http_request(HTTP_GET, couch->conn, "/_up", NULL);

    RETURN_COUCHDB_BOOL(resp, "available");

    free(resp);
  }
}
void couchdb_all_dbs(INTERNAL_FUNCTION_PARAMETERS)
{
  zval output;
  zval *obj = getThis();
  php_couchdb_connection *couch;

  ZEND_PARSE_PARAMETERS_NONE();

  couch = COUCHDB_OBJ(obj);

  if (couch != NULL)
  {
    couchdb_response *resp = couch_http_request(HTTP_GET, couch->conn, "/_all_dbs", NULL);

    RETURN_COUCHDB(couch->conn->format, resp, output);

    free(resp);
  }
}
void couchdb_all_docs(INTERNAL_FUNCTION_PARAMETERS)
{
  zend_string *db_database;
  zval *db_keys;
  zval output;

  zval *obj = getThis();
  php_couchdb_connection *couch;

  ZEND_PARSE_PARAMETERS_START(1, 2)
  Z_PARAM_STR(db_database)
  Z_PARAM_OPTIONAL
  Z_PARAM_ARRAY(db_keys)
  ZEND_PARSE_PARAMETERS_END();

  couch = COUCHDB_OBJ(obj);

  if (couch != NULL)
  {
    couchdb_response *resp;

    string_list *params = string_list_init();
    string_list_add_n(params, 3, "/", ZSTR_VAL(db_database), "/_all_docs");

    if (ZEND_NUM_ARGS() == 1)
    {
      resp = couch_http_request(HTTP_GET, couch->conn, string_list_concat(params), NULL);
    }
    else
    {
      zval keys;
      array_init(&keys);

      add_assoc_zval(&keys, "keys", db_keys);

      resp = couch_http_request(HTTP_POST, couch->conn, string_list_concat(params),
                                php_json_stringify(&keys));
    }

    RETURN_COUCHDB(couch->conn->format, resp, output);

    free(resp);
    free(params);
  }
}
void couchdb_get_doc(INTERNAL_FUNCTION_PARAMETERS)
{
  zend_string *db_database, *db_doc_id;
  zval output;
  zval *obj = getThis();

  php_couchdb_connection *couch;

  ZEND_PARSE_PARAMETERS_START(2, 2)
  Z_PARAM_STR(db_database)
  Z_PARAM_STR(db_doc_id)
  ZEND_PARSE_PARAMETERS_END();

  couch = COUCHDB_OBJ(obj);

  if (couch != NULL)
  {
    string_list *params = string_list_init();
    string_list_add_n(params, 4, "/", ZSTR_VAL(db_database), "/", ZSTR_VAL(db_doc_id));

    couchdb_response *resp = couch_http_request(HTTP_GET, couch->conn, string_list_concat(params), NULL);

    RETURN_COUCHDB(couch->conn->format, resp, output);

    free(params);
    free(resp);
  }
}
void couchdb_find(INTERNAL_FUNCTION_PARAMETERS)
{
  zend_string *db_database;
  zval *db_query;
  zval output;
  zval *obj = getThis();

  php_couchdb_connection *couch;

  ZEND_PARSE_PARAMETERS_START(2, 2)
  Z_PARAM_STR(db_database)
  Z_PARAM_ARRAY(db_query)
  ZEND_PARSE_PARAMETERS_END();

  couch = COUCHDB_OBJ(obj);

  if (couch != NULL)
  {
    string_list *params = string_list_init();
    string_list_add_n(params, 3, "/", ZSTR_VAL(db_database), "/_find");

    couchdb_response *resp = couch_http_request(HTTP_POST, couch->conn, string_list_concat(params),
                                                php_json_stringify(db_query));

    RETURN_COUCHDB(couch->conn->format, resp, output);

    free(params);
    free(resp);
  }
}
void couchdb_uuids(INTERNAL_FUNCTION_PARAMETERS)
{
  long db_uuid_count = 1;
  zval output;
  zval *obj = getThis();

  php_couchdb_connection *couch;

  ZEND_PARSE_PARAMETERS_START(0, 1)
  Z_PARAM_OPTIONAL
  Z_PARAM_LONG(db_uuid_count)
  ZEND_PARSE_PARAMETERS_END();

  couch = COUCHDB_OBJ(obj);

  if (couch != NULL)
  {
    zval uuids;
    array_init(&uuids);

    add_assoc_long(&uuids, "count", db_uuid_count);
    zend_string *query = php_build_query(&uuids);

    char *uuid_count = calloc(1, sizeof(long *));
    sprintf(uuid_count, "%ld", db_uuid_count);

    string_list *params = string_list_init();
    string_list_add_n(params, 2, "/_uuids?", ZSTR_VAL(query));

    couchdb_response *resp = couch_http_request(HTTP_GET, couch->conn, string_list_concat(params), NULL);

    RETURN_COUCHDB(couch->conn->format, resp, output);

    free(params);
    free(resp);
  }
}
void couchdb_create_index(INTERNAL_FUNCTION_PARAMETERS)
{
  zend_string *db_database;
  zval *db_index_options;
  zval *obj = getThis();

  php_couchdb_connection *couch;

  ZEND_PARSE_PARAMETERS_START(2, 2)
  Z_PARAM_STR(db_database)
  Z_PARAM_ARRAY(db_index_options)
  ZEND_PARSE_PARAMETERS_END();

  couch = COUCHDB_OBJ(obj);

  if (couch != NULL)
  {
    string_list *params = string_list_init();
    string_list_add_n(params, 3, "/", ZSTR_VAL(db_database), "/_index");

    couchdb_response *resp = couch_http_request(HTTP_POST, couch->conn, string_list_concat(params),
                                                php_json_stringify(db_index_options));

    RETURN_COUCHDB_BOOL(resp, "\"created\"");

    free(resp);
    free(params);
  }
}
void couchdb_create_ddoc(INTERNAL_FUNCTION_PARAMETERS)
{
  zend_string *db_database, *db_design;
  zval *db_options;
  zval *obj = getThis();

  php_couchdb_connection *couch;

  ZEND_PARSE_PARAMETERS_START(3, 3)
  Z_PARAM_STR(db_database)
  Z_PARAM_STR(db_design)
  Z_PARAM_ARRAY(db_options)
  ZEND_PARSE_PARAMETERS_END();

  couch = COUCHDB_OBJ(obj);

  if (couch != NULL)
  {
    string_list *params = string_list_init();
    string_list_add_n(params, 4, "/", ZSTR_VAL(db_database), "/_design/", ZSTR_VAL(db_design));

    couchdb_response *resp = couch_http_request(HTTP_POST, couch->conn, string_list_concat(params),
                                                php_json_stringify(db_options));

    RETURN_COUCHDB_BOOL(resp, NULL);

    free(resp);
    free(params);
  }
}
void couchdb_create_docs(INTERNAL_FUNCTION_PARAMETERS)
{
  zend_string *db_database;
  zval *db_documents;
  zval *obj = getThis();

  php_couchdb_connection *couch;

  ZEND_PARSE_PARAMETERS_START(2, 2)
  Z_PARAM_STR(db_database)
  Z_PARAM_ARRAY(db_documents)
  ZEND_PARSE_PARAMETERS_END();

  couch = COUCHDB_OBJ(obj);

  if (couch != NULL)
  {
    zval docs;
    array_init(&docs);

    add_assoc_zval(&docs, "docs", db_documents);
    string_list *params = string_list_init();
    string_list_add_n(params, 3, "/", ZSTR_VAL(db_database), "/_bulk_docs");

    couchdb_response *resp = couch_http_request(HTTP_POST, couch->conn, string_list_concat(params),
                                                php_json_stringify(&docs));

    RETURN_COUCHDB_BOOL(resp, NULL);

    free(resp);
    free(params);
  }
}
void couchdb_delete_docs(INTERNAL_FUNCTION_PARAMETERS)
{
  zend_string *db_database;
  zval *db_documents, *db_document;
  zval db_doc_data, db_doc_data_final;
  zval *obj = getThis();

  php_couchdb_connection *couch;

  ZEND_PARSE_PARAMETERS_START(2, 2)
  Z_PARAM_STR(db_database)
  Z_PARAM_ARRAY(db_documents)
  ZEND_PARSE_PARAMETERS_END();

  array_init(&db_doc_data);
  array_init(&db_doc_data_final);
  couch = COUCHDB_OBJ(obj);

  if (couch != NULL)
  {
    ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(db_documents), db_document)
    {
      if (Z_TYPE_P(db_document) != IS_ARRAY)
      {
        COUCHDB_THROW("Non-array value encountered");
      }

      zval doc;
      ZVAL_COPY(&doc, db_document);

      // handle copy-on-write for shared array
      SEPARATE_ARRAY(&doc);
      convert_to_array(&doc);

      add_assoc_bool(&doc, "_deleted", 1);
      add_next_index_zval(&db_doc_data, &doc);
    }
    ZEND_HASH_FOREACH_END();

    add_assoc_zval(&db_doc_data_final, "docs", &db_doc_data);
    string_list *params = string_list_init();
    string_list_add_n(params, 3, "/", ZSTR_VAL(db_database), "/_bulk_docs");

    couchdb_response *resp = couch_http_request(HTTP_POST, couch->conn, string_list_concat(params),
                                                php_json_stringify(&db_doc_data_final));

    RETURN_COUCHDB_BOOL(resp, NULL);

    free(resp);
    free(params);
  }
}
void couchdb_update_docs(INTERNAL_FUNCTION_PARAMETERS)
{
  zend_string *db_database, *db_doc_id, *db_doc_rev;
  zval *db_documents, *db_document;
  zval *obj = getThis();

  php_couchdb_connection *couch;

  ZEND_PARSE_PARAMETERS_START(2, 2)
  Z_PARAM_STR(db_database)
  Z_PARAM_ARRAY(db_documents)
  ZEND_PARSE_PARAMETERS_END();

  couch = COUCHDB_OBJ(obj);

  Z_STR_DEF(db_doc_id, "_id");
  Z_STR_DEF(db_doc_rev, "_rev");

  if (couch != NULL)
  {
    zval docs;
    array_init(&docs);

    ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(db_documents), db_document)
    {
      if (Z_TYPE_P(db_document) != IS_ARRAY)
      {
        COUCHDB_THROW("Non-array value encountered");
      }

      zval doc;
      ZVAL_COPY(&doc, db_document);

      SEPARATE_ARRAY(&doc);
      convert_to_array(&doc);

      if (!HT_KEY_EXISTS(&doc, db_doc_id) || !HT_KEY_EXISTS(&doc, db_doc_rev))
      {
        COUCHDB_THROW("Missing one of either _rev or _id");
      }
    }
    ZEND_HASH_FOREACH_END();

    add_assoc_zval(&docs, "docs", db_documents);
    string_list *params = string_list_init();
    string_list_add_n(params, 3, "/", ZSTR_VAL(db_database), "/_bulk_docs");

    couchdb_response *resp = couch_http_request(HTTP_POST, couch->conn, string_list_concat(params),
                                                php_json_stringify(&docs));

    RETURN_COUCHDB_BOOL(resp, NULL);

    free(resp);
    free(params);
  }

  zend_string_release(db_doc_id);
  zend_string_release(db_doc_rev);
}
void couchdb_changes(INTERNAL_FUNCTION_PARAMETERS)
{
  zend_string *db_database;
  zval *db_options, *db_option;
  zval output;
  zval *obj = getThis();

  php_couchdb_connection *couch;

  ZEND_PARSE_PARAMETERS_START(1, 2)
  Z_PARAM_STR(db_database)
  Z_PARAM_OPTIONAL
  Z_PARAM_ARRAY(db_options)
  ZEND_PARSE_PARAMETERS_END();

  couch = COUCHDB_OBJ(obj);

  if (couch != NULL)
  {
    zend_string *query;
    if (ZEND_NUM_ARGS() == 1)
    {
      zval opts;
      array_init(&opts);

      // exclude conflicts
      add_assoc_string(&opts, "conflicts", "false");
      // print changes in reverse chronological order
      add_assoc_string(&opts, "descending", "true");

      query = php_build_query(&opts);
    }
    else
    {
      zval opts;
      array_init(&opts);

      php_ht_convert_bool_to_string(db_options, &opts);

      query = php_build_query(&opts);
    }

    string_list *params = string_list_init();
    string_list_add_n(params, 4, "/", ZSTR_VAL(db_database), "/_changes?", ZSTR_VAL(query));

    couchdb_response *resp = couch_http_request(HTTP_GET, couch->conn, string_list_concat(params), NULL);

    RETURN_COUCHDB(couch->conn->format, resp, output);

    free(resp);
    free(params);
  }
}
void couchdb_database_op(INTERNAL_FUNCTION_PARAMETERS, int method, _Bool return_json)
{
  zend_string *db_database;
  zval *obj = getThis();
  php_couchdb_connection *couch;

  ZEND_PARSE_PARAMETERS_START(1, 1)
  Z_PARAM_STR(db_database)
  ZEND_PARSE_PARAMETERS_END();

  couch = COUCHDB_OBJ(obj);

  if (couch != NULL)
  {
    string_list *params = string_list_init();
    string_list_add_n(params, 2, "/", ZSTR_VAL(db_database));

    couchdb_response *resp = couch_http_request(method, couch->conn, string_list_concat(params), NULL);

    if (!return_json)
    {
      RETURN_COUCHDB_BOOL(resp, NULL);
    }
    else
    {
      zval output;
      RETURN_COUCHDB(couch->conn->format, resp, output);
    }

    free(params);
    free(resp);
  }
}
void couchdb_query_view(INTERNAL_FUNCTION_PARAMETERS)
{
  zend_string *db_database, *db_design, *db_view;
  zval *db_options;
  zval output;

  zval *obj = getThis();
  php_couchdb_connection *couch;

  ZEND_PARSE_PARAMETERS_START(3, 4)
  Z_PARAM_STR(db_database)
  Z_PARAM_STR(db_design)
  Z_PARAM_STR(db_view)
  Z_PARAM_OPTIONAL
  Z_PARAM_ARRAY(db_options)
  ZEND_PARSE_PARAMETERS_END();

  couch = COUCHDB_OBJ(obj);

  if (couch != NULL)
  {
    couchdb_response *resp;
    string_list *params = string_list_init();
    string_list_add_n(params, 6, "/", ZSTR_VAL(db_database), "/_design/", ZSTR_VAL(db_design),
                      "/_view/", ZSTR_VAL(db_view));

    if (ZEND_NUM_ARGS() == 3)
    {
      resp = couch_http_request(HTTP_GET, couch->conn, string_list_concat(params), NULL);
    }
    else
    {
      // build query otherwise
      zval opts;
      array_init(&opts);

      php_ht_convert_bool_to_string(db_options, &opts);

      zend_string *query = php_build_query(&opts);
      string_list_add_n(params, 2, "?", ZSTR_VAL(query));

      resp = couch_http_request(HTTP_GET, couch->conn, string_list_concat(params), NULL);
    }

    RETURN_COUCHDB(couch->conn->format, resp, output);

    free(resp);
    free(params);
  }
}

#ifdef ASYNC_H
async couchdb_async_func_call(struct async ptr, int *status, zval *fn, zval *args,
                              zval *retval, zval *output)
{
  async_begin(&ptr);

  if (!zend_is_callable(fn, 0, NULL))
  {
    COUCHDB_THROW("Cannot parse non-callable");
  }

  *status = call_user_function(NULL, NULL, fn, retval, 1, args);

  if (*status == SUCCESS || !Z_ISUNDEF_P(retval))
  {
    add_next_index_zval(output, retval);
  }

  async_end;
}
async couchdb_execute_async_transactions(struct async ptr, zval *transactions,
                                         zval *action, zval *output)
{
  async_begin(&ptr);

  ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(transactions), action)
  {
    int *status = malloc(sizeof(int));

    zval args, retval;
    struct async ptr;

    async_init(&ptr);
    await(couchdb_async_func_call(ptr, status, action, &args, &retval, output));
  }
  ZEND_HASH_FOREACH_END();

  async_end;
}
#endif

void couchdb_transaction(INTERNAL_FUNCTION_PARAMETERS)
{
  zval *actions, *action;
  zval output;
  array_init(&output);

  zval *obj = getThis();
  php_couchdb_connection *couch;

  ZEND_PARSE_PARAMETERS_START(1, 1)
  Z_PARAM_ARRAY(actions)
  ZEND_PARSE_PARAMETERS_END();

  couch = COUCHDB_OBJ(obj);

  if (couch != NULL)
  {
#ifndef ASYNC_H
    COUCHDB_THROW("Please install async.h to use this feature");
#else
    struct async ptr;
    couchdb_execute_async_transactions(ptr, actions, action, &output);

    RETURN_ZVAL(&output, 1, 0);
#endif
  }
}
