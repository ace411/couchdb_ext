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

#ifndef PHP_COUCH_H
#define PHP_COUCH_H

#include "php.h"
#include "ext/json/php_json.h"
#include "ext/standard/php_array.h"
#include "ext/standard/php_http.h"
#include "ext/standard/php_var.h"
#include "zend_exceptions.h"
#include "zend_smart_str.h"
#include "ext/spl/spl_exceptions.h"

#ifndef ASYNC_H
#include "async.h"
#endif

#include "../couch/couch.h"

/* For compatibility with older PHP versions */
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE() \
  ZEND_PARSE_PARAMETERS_START(0, 0)  \
  ZEND_PARSE_PARAMETERS_END()
#endif

/* PHP version module numbers */
#define ZEND_API_PHP80 20200930

/* PHP CouchDB object handlers */
zend_object_handlers couchdb_object_handlers;

struct php_couchdb_connection;
struct php_couchdb_transactions;
typedef struct php_couchdb_connection php_couchdb_connection;

/* struct containing PHP userland-destined CouchDB connection parameters */
struct php_couchdb_connection
{
  /* pointer to the couchdb_connection artifact/object */
  couchdb_connection *conn;
  /* PHP object */
  zend_object std;
};

/* instantiate PHP userland-destined CouchDb object */
static inline php_couchdb_connection *php_couchdb_obj_from_obj(zend_object *obj)
{
  return (php_couchdb_connection *)((char *)obj - XtOffsetOf(php_couchdb_connection, std));
}

#define COUCHDB_OBJ(zv) php_couchdb_obj_from_obj(Z_OBJ_P(zv))

/* create PHP CouchDb object from couchdb_connection artifact */
zend_object *php_couchdb_connection_create_object_ex(couchdb_connection *c);
/* create PHP CouchDb object from couchdb_connection with default parameters */
zend_object *php_couchdb_connection_create_object(zend_class_entry *ce);

/* free memory allocated to CouchDb PHP object */
void php_couchdb_connection_free_object(zend_object *object);
/* nuke PHP object from memory */
void php_couchdb_connection_destroy_object(zend_object *object);
/* convey object configuration */
#if ZEND_MODULE_API_NO < ZEND_API_PHP80
HashTable *php_couchdb_get_properties(zval *object);
#else
HashTable *php_couchdb_get_properties(zend_object *object);
#endif

/* create a PHP array from a JSON string subsumed in couchdb_response artifact */
zval *convert_couchdb_response_to_array(char *response, zval *output);
/* create a PHP-parsable version of a URL query string from a PHP array */
zend_string *php_build_query(zval *list);
/* convert a PHP array to a JSON string */
char *php_json_stringify(zval *list);
/* convert boolean data in arrays to apt string representations (ideal for query parameters) */
void php_ht_convert_bool_to_string(zval *list, zval *output);
/* extract CouchDB error response */
char *extract_couchdb_response_error(char *response);

#ifdef ASYNC_H
/* asynchronously execute functions subsumed in a PHP variable - in a protothread */
async couchdb_async_func_call(struct async ptr, int *status, zval *fn, zval *args, zval *retval, zval *output);
/* asynchronously proxy asynchronous transactions defined in a PHP array to the couchdb_async_func_call function */
async couchdb_execute_async_transactions(struct async ptr, zval *transactions, zval *action, zval *output);
#endif

/* establishes CouchDB configuration */
void couchdb_connect(INTERNAL_FUNCTION_PARAMETERS);
/* retrieve CouchDB auth token */
void couchdb_session_init(INTERNAL_FUNCTION_PARAMETERS);
/* check if CouchDB is available at a specified address */
void couchdb_is_available(INTERNAL_FUNCTION_PARAMETERS);
/* return list of all databases */
void couchdb_all_dbs(INTERNAL_FUNCTION_PARAMETERS);
/* return list of all documents present in a specified database */
void couchdb_all_docs(INTERNAL_FUNCTION_PARAMETERS);
/* return a document in a specified database */
void couchdb_get_doc(INTERNAL_FUNCTION_PARAMETERS);
/* execute Mango-powered database search */
void couchdb_find(INTERNAL_FUNCTION_PARAMETERS);
/* generate a specified number of unique identifiers */
void couchdb_uuids(INTERNAL_FUNCTION_PARAMETERS);
/* define index on specified database */
void couchdb_create_index(INTERNAL_FUNCTION_PARAMETERS);
/* asynchronously execute a series of library-powered CouchDB actions */
void couchdb_transaction(INTERNAL_FUNCTION_PARAMETERS);
/* create new design document */
void couchdb_create_ddoc(INTERNAL_FUNCTION_PARAMETERS);
/* create new documents */
void couchdb_create_docs(INTERNAL_FUNCTION_PARAMETERS);
/* delete documents */
void couchdb_delete_docs(INTERNAL_FUNCTION_PARAMETERS);
/* update multiple documents */
void couchdb_update_docs(INTERNAL_FUNCTION_PARAMETERS);
/* track changes made to database */
void couchdb_changes(INTERNAL_FUNCTION_PARAMETERS);
/* perform one of either database creation, artifact information retrieval, or deletion */
void couchdb_database_op(INTERNAL_FUNCTION_PARAMETERS, int method, _Bool return_json);
/* retrieve data via specified view document */
void couchdb_query_view(INTERNAL_FUNCTION_PARAMETERS);

zend_class_entry *couch_ce;
zend_class_entry *couch_exception_ce;

#define COUCHDB_THROW(msg) zend_throw_exception(couch_exception_ce, msg, 0);

#define RETURN_COUCHDB(return_array, resp, output)                            \
  if (resp->valid)                                                            \
  {                                                                           \
    if (strstr(resp->response.response, "error"))                             \
    {                                                                         \
      COUCHDB_THROW(extract_couchdb_response_error(resp->response.response)); \
    }                                                                         \
    if (return_array)                                                         \
    {                                                                         \
      convert_couchdb_response_to_array(resp->response.response, &output);    \
      RETURN_ZVAL(&output, 1, 0);                                             \
    }                                                                         \
    else                                                                      \
    {                                                                         \
      RETURN_STRING(resp->response.response);                                 \
    }                                                                         \
  }                                                                           \
  else                                                                        \
  {                                                                           \
    COUCHDB_THROW(resp->response.error);                                      \
  }

#define RETURN_COUCHDB_BOOL(resp, needle)                                     \
  if (resp->valid)                                                            \
  {                                                                           \
    if (strstr(resp->response.response, "error"))                             \
    {                                                                         \
      COUCHDB_THROW(extract_couchdb_response_error(resp->response.response)); \
    }                                                                         \
    if (strstr(resp->response.response, needle == NULL ? "\"ok\"" : needle))  \
    {                                                                         \
      RETURN_BOOL((zend_bool)1);                                              \
    }                                                                         \
  }                                                                           \
  else                                                                        \
  {                                                                           \
    COUCHDB_THROW(resp->response.error);                                      \
  }

#define Z_STR_DEF(var, key) var = zend_string_init(key, sizeof(key) - 1, 1);

#define HT_KEY_EXISTS(ht, key) (zend_hash_exists(Z_ARRVAL_P(ht), key))

#define HT_GET_VAL(ht, key) zend_hash_find(Z_ARRVAL_P(ht), key)

#define HT_GET_STRVAL(ht, key) Z_STRVAL_P(HT_GET_VAL(ht, key))

#define HT_GET_LVAL(ht, key) Z_LVAL_P(HT_GET_VAL(ht, key))

#endif
