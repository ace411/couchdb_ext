/**
 * @file couch.cpp
 * @author Lochemem Bruno Michael (lochbm@gmail.com)
 * @brief file containing C++ implementations of PHP userland-destined functions
 * @version 0.1.0
 * 
 * @copyright Copyright (c) 1999-2019 The PHP Group
 * 
 */
#include "php_couchdb.h"
#include "couch.h"

using namespace couch;

#ifdef HAVE_SPL
#include "ext/spl/spl_exceptions.h"
#endif

/* For compatibility with older PHP versions */
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE() \
  ZEND_PARSE_PARAMETERS_START(0, 0)  \
  ZEND_PARSE_PARAMETERS_END()
#endif

zend_object_handlers couchdb_object_handlers;

/* couchdb and couchdb exception objects */
zend_class_entry *couchdb_ce;
zend_class_entry *couchdb_exception_ce;

typedef struct _couchdb_object
{
  CouchDb *couch;
  bool json;
  zend_object std;
} couchdb_object;

/**
 * @brief creates extension-layer usable object from PHP object context
 * 
 * @param obj 
 * @return couchdb_object* 
 */
static inline couchdb_object *php_couchdb_obj_from_obj(zend_object *obj)
{
  return (couchdb_object *)((char *)(obj)-XtOffsetOf(couchdb_object, std));
}

#define Z_COUCHOBJ_P(zv) php_couchdb_obj_from_obj(Z_OBJ_P(zv))

#define COUCHDB_THROW(msg)                                      \
  zend_throw_exception(couchdb_exception_ce, msg, 0 TSRMLS_CC); \
  RETURN_NULL();

// macros to execute CouchDb request in exception-handled context
#define COUCH_ACTION_START() \
  try

#define COUCH_ACTION_END()     \
  catch (std::exception & exp) \
  {                            \
    COUCHDB_THROW(exp.what()); \
  }

/**
 * @brief creates new PHP CouchDb object
 * 
 * @param TSRMLS_DC 
 * @return zend_object* 
 */
zend_object *couchdb_object_new(zend_class_entry *ce TSRMLS_DC)
{
  couchdb_object *intern = (couchdb_object *)ecalloc(1,
                                                     sizeof(couchdb_object) +
                                                         zend_object_properties_size(ce));

  zend_object_std_init(&intern->std, ce TSRMLS_CC);
  object_properties_init(&intern->std, ce);

  intern->std.handlers = &couchdb_object_handlers;

  return &intern->std;
}

static void couchdb_object_destroy(zend_object *object)
{
  couchdb_object *couchdb;
  couchdb = (couchdb_object *)((char *)object - XtOffsetOf(couchdb_object, std));

  zend_objects_destroy_object(object);
}

static void couchdb_object_free(zend_object *object)
{
  couchdb_object *couchdb;
  couchdb = (couchdb_object *)((char *)object - XtOffsetOf(couchdb_object, std));
  delete couchdb->couch;

  zend_object_std_dtor(object);
}

// macro to convert CouchDB-response to PHP hashtable
#define RETURN_ARRAY_FROM_JSON(zv, json) \
  php_json_decode_ex(zv, (char *)json.c_str(), json.size(), PHP_JSON_OBJECT_AS_ARRAY, 512);

// macro to conditionally return CouchDB response as JSON or PHP hashtable
#define COUCHDB_RETURN(res, json) \
  if (json)                       \
  {                               \
    RETURN_STRING(res.c_str());   \
  }                               \
  RETURN_ARRAY_FROM_JSON(return_value, res);

/**
 * @brief PHP CouchDb constructor
 * 
 */
static void constructor(INTERNAL_FUNCTION_PARAMETERS)
{
  zval *options;
  zend_string *host, *user, *pass, *timeout, *port, *http, *json;

  long portv, timeoutv;
  bool httpv, jsonv;

  zval *obj = getThis();
  couchdb_object *intern;

  ZEND_PARSE_PARAMETERS_START(1, 1)
  Z_PARAM_ARRAY(options)
  ZEND_PARSE_PARAMETERS_END();

  host = zend_string_init("host", sizeof("host") - 1, 1);
  user = zend_string_init("user", sizeof("user") - 1, 1);
  pass = zend_string_init("pass", sizeof("pass") - 1, 1);
  port = zend_string_init("port", sizeof("port") - 1, 1);
  http = zend_string_init("http", sizeof("http") - 1, 1);
  json = zend_string_init("json", sizeof("json") - 1, 1);
  timeout = zend_string_init("timeout", sizeof("timeout") - 1, 1);

  if (!zend_hash_exists(HASH_OF(options), host) ||
      !zend_hash_exists(HASH_OF(options), user) ||
      !zend_hash_exists(HASH_OF(options), pass))
  {
    zend_string_release(port);
    zend_string_release(timeout);
    COUCHDB_THROW("'host', 'user', and 'pass' are mandatory config options");
  }

  intern = Z_COUCHOBJ_P(obj);
  if (intern != NULL)
  {
    httpv = !zend_hash_exists(HASH_OF(options), http)
                ? true
                : Z_TYPE_P(zend_hash_find(HASH_OF(options), http)) == IS_TRUE ? true : false;
    jsonv = !zend_hash_exists(HASH_OF(options), json)
                ? true
                : Z_TYPE_P(zend_hash_find(HASH_OF(options), json)) == IS_TRUE ? true : false;
    portv = !zend_hash_exists(HASH_OF(options), port)
                ? long(DEFAULT_PORT)
                : Z_LVAL_P(zend_hash_find(HASH_OF(options), port));
    timeoutv = !zend_hash_exists(HASH_OF(options), timeout)
                   ? long(DEFAULT_TIMEOUT)
                   : Z_LVAL_P(zend_hash_find(HASH_OF(options), timeout));

    intern->json = jsonv;
    intern->couch = new CouchDb(Z_STRVAL_P(zend_hash_find(HASH_OF(options), host)),
                                Z_STRVAL_P(zend_hash_find(HASH_OF(options), user)),
                                Z_STRVAL_P(zend_hash_find(HASH_OF(options), pass)),
                                httpv,
                                portv,
                                timeoutv);
  }
}

/**
 * @brief PHP CouchDb isAvailable method
 * 
 */
static void isAvailable(INTERNAL_FUNCTION_PARAMETERS)
{
  zval *obj = getThis();
  couchdb_object *intern;

  ZEND_PARSE_PARAMETERS_NONE();

  intern = Z_COUCHOBJ_P(obj);
  if (intern != NULL)
  {
    COUCH_ACTION_START()
    {
      RETURN_BOOL(intern->couch->isAvailable());
    }
    COUCH_ACTION_END();
  }
}

/**
 * @brief PHP CouchDb database action-related methods
 * 
 * @param action 
 */
static void dbActions(INTERNAL_FUNCTION_PARAMETERS, long action)
{
  zend_string *database;

  zval *obj = getThis();
  couchdb_object *intern;

  ZEND_PARSE_PARAMETERS_START(1, 1)
  Z_PARAM_STR(database)
  ZEND_PARSE_PARAMETERS_END();

  if (ZSTR_LEN(database) == 0)
  {
    zend_string_release(database);
    COUCHDB_THROW("Argument(s) cannot be empty");
  }

  intern = Z_COUCHOBJ_P(obj);
  if (intern != NULL)
  {
    COUCH_ACTION_START()
    {
      std::string str; // string result
      bool res;        // boolean result

      switch (action)
      {
      case DB_ACTION_INFO:
        str = intern->couch->dbInfo(ZSTR_VAL(database));
        COUCHDB_RETURN(str, intern->json);
        break;

      case DB_ACTION_CREATE:
        res = intern->couch->createDb(ZSTR_VAL(database));
        RETURN_BOOL(res);
        break;

      case DB_ACTION_DELETE:
        res = intern->couch->deleteDb(ZSTR_VAL(database));
        RETURN_BOOL(res);
        break;

      case DB_ACTION_CHECK:
        res = intern->couch->dbExists(ZSTR_VAL(database));
        RETURN_BOOL(res);
        break;
      }
    }
    COUCH_ACTION_END();
  }
}

/**
 * @brief PHP CouchDb uuids method
 * 
 */
static void uuids(INTERNAL_FUNCTION_PARAMETERS)
{
  long count = 1;

  zval *obj = getThis();
  couchdb_object *intern;

  ZEND_PARSE_PARAMETERS_START(0, 1)
  Z_PARAM_LONG(count)
  ZEND_PARSE_PARAMETERS_END();

  intern = Z_COUCHOBJ_P(obj);
  if (intern != NULL)
  {
    COUCH_ACTION_START()
    {
      std::string retval = intern->couch->uuids(count);
      COUCHDB_RETURN(retval, intern->json);
    }
    COUCH_ACTION_END();
  }
}

/**
 * @brief PHP CouchDb allDbs method
 * 
 */
static void allDbs(INTERNAL_FUNCTION_PARAMETERS)
{
  zval *obj = getThis();
  couchdb_object *intern;

  ZEND_PARSE_PARAMETERS_NONE();

  intern = Z_COUCHOBJ_P(obj);
  if (intern != NULL)
  {
    COUCH_ACTION_START()
    {
      std::string retval = intern->couch->allDbs();
      COUCHDB_RETURN(retval, intern->json);
    }
    COUCH_ACTION_END();
  }
}

// macro to URL-encode query parameters in PHP hashtable
#define URL_ENCODE(query, querystr, separator)                              \
  if (php_url_encode_hash_ex(HASH_OF(query),                                \
                             &querystr,                                     \
                             NULL,                                          \
                             0,                                             \
                             NULL,                                          \
                             0,                                             \
                             NULL,                                          \
                             0,                                             \
                             (Z_TYPE_P(query) == IS_OBJECT ? query : NULL), \
                             separator,                                     \
                             1) == FAILURE)                                 \
  {                                                                         \
    COUCHDB_THROW("Cannot URL-encode query");                               \
  }

// macro to convert boolean arguments in query arrays to CouchDB API-apt string forms
#define BOOL_ARG_TO_STRING(htable, val)       \
  ZEND_HASH_FOREACH_VAL(HASH_OF(htable), val) \
  {                                           \
    switch (Z_TYPE_P(val))                    \
    {                                         \
    case IS_TRUE:                             \
      ZVAL_STRING(val, "true");               \
      break;                                  \
    case IS_FALSE:                            \
      ZVAL_STRING(val, "false");              \
      break;                                  \
    }                                         \
  }                                           \
  ZEND_HASH_FOREACH_END();

/**
 * @brief PHP CouchDb getDoc method
 * 
 */
static void getDoc(INTERNAL_FUNCTION_PARAMETERS)
{
  zend_string *database, *docid;

  zval *obj = getThis();
  couchdb_object *intern;

  ZEND_PARSE_PARAMETERS_START(2, 2)
  Z_PARAM_STR(database)
  Z_PARAM_STR(docid)
  ZEND_PARSE_PARAMETERS_END();

  if (ZSTR_LEN(database) == 0 || ZSTR_LEN(docid) == 0)
  {
    zend_string_release(database);
    zend_string_release(docid);
    COUCHDB_THROW("Argument(s) cannot be empty");
  }

  intern = Z_COUCHOBJ_P(obj);
  if (intern != NULL)
  {
    COUCH_ACTION_START()
    {
      std::string retval = intern->couch->getDoc(ZSTR_VAL(database),
                                                 ZSTR_VAL(docid));
      COUCHDB_RETURN(retval, intern->json);
    }
    COUCH_ACTION_END();
  }
}

/**
 * @brief PHP CouchDb allDocs method
 * 
 */
static void allDocs(INTERNAL_FUNCTION_PARAMETERS)
{
  zend_string *database;
  zval *query, *queryv;
  char *separator = NULL;

  smart_str querystr = {0};

  zval *obj = getThis();
  couchdb_object *intern;

  ZEND_PARSE_PARAMETERS_START(2, 2)
  Z_PARAM_STR(database)
  Z_PARAM_ARRAY(query)
  ZEND_PARSE_PARAMETERS_END();

  if (ZSTR_LEN(database) == 0)
  {
    zend_string_release(database);
    COUCHDB_THROW("Argument(s) cannot be empty");
  }

  // convert boolean false and true to their string forms
  BOOL_ARG_TO_STRING(query, queryv);

  // convert array to URL-encoded query string
  URL_ENCODE(query, querystr, separator);
  smart_str_0(&querystr);

  intern = Z_COUCHOBJ_P(obj);
  if (intern != NULL)
  {
    COUCH_ACTION_START()
    {
      std::string retval = intern->couch->allDocs(ZSTR_VAL(database),
                                                  ZSTR_VAL(querystr.s));

      smart_str_free(&querystr);
      efree(querystr.s);

      COUCHDB_RETURN(retval, intern->json);
    }
    COUCH_ACTION_END();
  }

  zend_string_release(database);
}

/**
 * @brief PHP CouchDb insertDocs method
 * 
 */
static void insertDocs(INTERNAL_FUNCTION_PARAMETERS, bool del = false)
{
  zend_string *database, *dockey;
  zval *contents, *doc, docdata, tmpcontents, tmpdata;

  smart_str jsonData = {0};

  zval *obj = getThis();
  couchdb_object *intern;

  ZEND_PARSE_PARAMETERS_START(2, 2)
  Z_PARAM_STR(database)
  Z_PARAM_ARRAY(contents)
  ZEND_PARSE_PARAMETERS_END();

  dockey = zend_string_init("docs", sizeof("docs") - 1, 1);

  array_init(&tmpdata);
  array_init(&tmpcontents);

  if (zend_hash_num_elements(HASH_OF(contents)) == 0 ||
      ZSTR_LEN(database) == 0)
  {
    zend_string_release(database);
    zend_string_release(dockey);
    COUCHDB_THROW("Argument(s) cannot be empty");
  }

  if (!zend_hash_exists(HASH_OF(contents), dockey))
  {
    zend_string_release(database);
    zend_string_release(dockey);
    COUCHDB_THROW("'docs' key is missing");
  }

  ZVAL_COPY(&docdata, zend_hash_find(HASH_OF(contents), dockey));
  if (Z_TYPE(docdata) != IS_ARRAY)
  {
    zend_string_release(database);
    zend_string_release(dockey);
    COUCHDB_THROW("'docs' cannot be non-array");
  }

  ZEND_HASH_FOREACH_VAL(HASH_OF(&docdata), doc)
  {
    if (Z_TYPE_P(doc) != IS_ARRAY)
    {
      zend_string_release(database);
      zend_string_release(dockey);
      COUCHDB_THROW("Non-array value detected");
    }

    if (del)
    {
      zval temp;
      ZVAL_COPY(&temp, doc);

      SEPARATE_ARRAY(&temp);
      convert_to_array(&temp);

      add_assoc_bool(&temp, "_deleted", 1);
      add_next_index_zval(&tmpdata, &temp);
    }
  }
  ZEND_HASH_FOREACH_END();

  if (del)
  {
    add_assoc_zval(&tmpcontents, "docs", &tmpdata);
    php_json_encode(&jsonData, &tmpcontents, 0);

    i_zval_ptr_dtor(&tmpdata);
    i_zval_ptr_dtor(&tmpcontents);
  }

  php_json_encode(&jsonData, contents, 0);
  smart_str_0(&jsonData);

  i_zval_ptr_dtor(&docdata);
  i_zval_ptr_dtor(doc);

  intern = Z_COUCHOBJ_P(obj);
  if (intern != NULL)
  {
    COUCH_ACTION_START()
    {
      bool retval = intern->couch->insertDocs(ZSTR_VAL(database),
                                              ZSTR_VAL(jsonData.s));

      smart_str_free(&jsonData);
      efree(jsonData.s);

      RETURN_BOOL(retval);
    }
    COUCH_ACTION_END();
  }
}

/**
 * @brief PHP CouchDb search method
 * 
 */
static void query(INTERNAL_FUNCTION_PARAMETERS, long type)
{
  zend_string *database;
  zval *query;

  smart_str jsonData = {0};

  zval *obj = getThis();
  couchdb_object *intern;

  ZEND_PARSE_PARAMETERS_START(2, 2)
  Z_PARAM_STR(database)
  Z_PARAM_ARRAY(query)
  ZEND_PARSE_PARAMETERS_END();

  if (zend_hash_num_elements(HASH_OF(query)) == 0 ||
      ZSTR_LEN(database) == 0)
  {
    zend_string_release(database);
    COUCHDB_THROW("Argument(s) cannot be empty");
  }

  php_json_encode(&jsonData, query, 0);
  smart_str_0(&jsonData);

  intern = Z_COUCHOBJ_P(obj);
  if (intern != NULL)
  {
    // free JSON smart string
#define FREE_JSON(json)  \
  smart_str_free(&json); \
  efree(json.s);

    COUCH_ACTION_START()
    {
      std::string retval;
      bool res;

      switch (type)
      {
      case DB_QUERY_SEARCH:
        retval = intern->couch->search(ZSTR_VAL(database),
                                       ZSTR_VAL(jsonData.s));
        FREE_JSON(jsonData);
        COUCHDB_RETURN(retval, intern->json);
        break;

      case DB_QUERY_EXPLAIN:
        retval = intern->couch->search(ZSTR_VAL(database),
                                       ZSTR_VAL(jsonData.s),
                                       true);
        FREE_JSON(jsonData);
        COUCHDB_RETURN(retval, intern->json);
        break;

      case DB_QUERY_CREATE_INDEX:
        res = intern->couch->createIndex(ZSTR_VAL(database),
                                         ZSTR_VAL(jsonData.s));
        FREE_JSON(jsonData);
        RETURN_BOOL(res);
        break;
      }
    }
    COUCH_ACTION_END();
  }
}

/**
 * 
 * @brief PHP CouchDb updateDoc method
 * 
 */
static void updateDoc(INTERNAL_FUNCTION_PARAMETERS)
{
  zend_string *database, *rev, *docid;
  zval *contents;

  smart_str jsonData = {0};

  zval *obj = getThis();
  couchdb_object *intern;

  ZEND_PARSE_PARAMETERS_START(4, 4)
  Z_PARAM_STR(database)
  Z_PARAM_STR(docid)
  Z_PARAM_STR(rev)
  Z_PARAM_ARRAY(contents)
  ZEND_PARSE_PARAMETERS_END();

  if (zend_hash_num_elements(HASH_OF(contents)) == 0 ||
      ZSTR_LEN(database) == 0 ||
      ZSTR_LEN(docid) == 0 ||
      ZSTR_LEN(rev) == 0)
  {
    zend_string_release(database);
    zend_string_release(docid);
    zend_string_release(rev);
    COUCHDB_THROW("Argument(s) cannot be empty");
  }

  php_json_encode(&jsonData, contents, 0);
  smart_str_0(&jsonData);

  intern = Z_COUCHOBJ_P(obj);
  if (intern != NULL)
  {
    COUCH_ACTION_START()
    {
      bool retval = intern->couch->updateDoc(ZSTR_VAL(database),
                                             ZSTR_VAL(docid),
                                             ZSTR_VAL(rev),
                                             ZSTR_VAL(jsonData.s));
      smart_str_free(&jsonData);
      efree(jsonData.s);
      RETURN_BOOL(retval);
    }
    COUCH_ACTION_END();
  }
}

/**
 * @brief PHP CouchDb insertDoc method
 * 
 */
static void insertDoc(INTERNAL_FUNCTION_PARAMETERS)
{
  zend_string *database;
  zval *contents;
  zend_bool batch = 0;

  smart_str jsonData = {0};

  zval *obj = getThis();
  couchdb_object *intern;

  ZEND_PARSE_PARAMETERS_START(2, 3)
  Z_PARAM_STR(database)
  Z_PARAM_ARRAY(contents)
  Z_PARAM_OPTIONAL
  Z_PARAM_BOOL(batch)
  ZEND_PARSE_PARAMETERS_END();

  if (zend_hash_num_elements(HASH_OF(contents)) == 0 ||
      ZSTR_LEN(database) == 0)
  {
    zend_string_release(database);
    COUCHDB_THROW("Argument(s) cannot be empty");
  }

  php_json_encode(&jsonData, contents, 0);
  smart_str_0(&jsonData);

  intern = Z_COUCHOBJ_P(obj);
  if (intern != NULL)
  {
    COUCH_ACTION_START()
    {
      bool retval = intern->couch->insertDoc(ZSTR_VAL(database),
                                             ZSTR_VAL(jsonData.s),
                                             (batch == 0 ? false : true));
      smart_str_free(&jsonData);
      efree(jsonData.s);
      RETURN_BOOL(retval);
    }
    COUCH_ACTION_END();
  }
}

/**
 * @brief PHP CouchDb deleteDoc method
 * 
 */
static void deleteDoc(INTERNAL_FUNCTION_PARAMETERS)
{
  zend_string *database, *docid, *rev;

  zval *obj = getThis();
  couchdb_object *intern;

  ZEND_PARSE_PARAMETERS_START(3, 3)
  Z_PARAM_STR(database)
  Z_PARAM_STR(docid)
  Z_PARAM_STR(rev)
  ZEND_PARSE_PARAMETERS_END();

  if (ZSTR_LEN(database) == 0 || ZSTR_LEN(docid) == 0 || ZSTR_LEN(rev) == 0)
  {
    zend_string_release(database);
    zend_string_release(docid);
    zend_string_release(rev);
    COUCHDB_THROW("Argument(s) cannot be empty");
  }

  intern = Z_COUCHOBJ_P(obj);
  if (intern != NULL)
  {
    COUCH_ACTION_START()
    {
      bool retval = intern->couch->deleteDoc(ZSTR_VAL(database),
                                             ZSTR_VAL(docid),
                                             ZSTR_VAL(rev));
      RETURN_BOOL(retval);
    }
    COUCH_ACTION_END();
  }
}

/**
 * @brief PHP CouchDb createDdoc method
 * 
 */
static void createDdoc(INTERNAL_FUNCTION_PARAMETERS)
{
  zend_string *database, *ddoc;
  zval *contents;

  smart_str jsonData = {0};

  zval *obj = getThis();
  couchdb_object *intern;

  ZEND_PARSE_PARAMETERS_START(3, 3)
  Z_PARAM_STR(database)
  Z_PARAM_STR(ddoc)
  Z_PARAM_ARRAY(contents)
  ZEND_PARSE_PARAMETERS_END();

  if (zend_hash_num_elements(HASH_OF(contents)) == 0 ||
      ZSTR_LEN(database) == 0 ||
      ZSTR_LEN(ddoc) == 0)
  {
    zend_string_release(ddoc);
    zend_string_release(database);
    COUCHDB_THROW("Argument(s) cannot be empty");
  }

  php_json_encode(&jsonData, contents, 0);
  smart_str_0(&jsonData);

  intern = Z_COUCHOBJ_P(obj);
  if (intern != NULL)
  {
    COUCH_ACTION_START()
    {
      bool retval = intern->couch->createDdoc(ZSTR_VAL(database),
                                              ZSTR_VAL(ddoc),
                                              ZSTR_VAL(jsonData.s));
      smart_str_free(&jsonData);
      efree(jsonData.s);
      RETURN_BOOL(retval);
    }
    COUCH_ACTION_END();
  }
}

/**
 * @brief PHP CouchDb queryView method
 * 
 */
static void queryView(INTERNAL_FUNCTION_PARAMETERS)
{
  zend_string *database, *ddoc, *view;
  zval *query, *queryv;
  char *separator = NULL;

  smart_str querystr = {0};

  zval *obj = getThis();
  couchdb_object *intern;

  ZEND_PARSE_PARAMETERS_START(4, 4)
  Z_PARAM_STR(database)
  Z_PARAM_STR(ddoc)
  Z_PARAM_STR(view)
  Z_PARAM_ARRAY(query)
  ZEND_PARSE_PARAMETERS_END();

  if (zend_hash_num_elements(HASH_OF(query)) == 0 ||
      ZSTR_LEN(database) == 0 ||
      ZSTR_LEN(ddoc) == 0 ||
      ZSTR_LEN(view) == 0)
  {
    zend_string_release(database);
    zend_string_release(view);
    zend_string_release(ddoc);
    COUCHDB_THROW("Argument(s) cannot be empty");
  }

  BOOL_ARG_TO_STRING(query, queryv);
  URL_ENCODE(query, querystr, separator);
  smart_str_0(&querystr);

  intern = Z_COUCHOBJ_P(obj);
  if (intern != NULL)
  {
    COUCH_ACTION_START()
    {
      std::string retval = intern->couch->queryView(ZSTR_VAL(database),
                                                    ZSTR_VAL(ddoc),
                                                    ZSTR_VAL(view),
                                                    ZSTR_VAL(querystr.s));
      smart_str_free(&querystr);
      efree(querystr.s);
      COUCHDB_RETURN(retval, intern->json);
    }
    COUCH_ACTION_END();
  }
}

/**
 * @brief PHP CouchDb changes method
 * 
 */
static void changes(INTERNAL_FUNCTION_PARAMETERS)
{
  zend_string *database, *feed;
  zval *options, *opt;
  char *separator = NULL;

  smart_str querystr = {0};

  zval *obj = getThis();
  couchdb_object *intern;

  array_init(options);

  ZEND_PARSE_PARAMETERS_START(1, 2)
  Z_PARAM_STR(database)
  Z_PARAM_OPTIONAL
  Z_PARAM_ARRAY(options)
  ZEND_PARSE_PARAMETERS_END();

  if (ZSTR_LEN(database) == 0)
  {
    zend_string_release(database);
    COUCHDB_THROW("Argument(s) cannot be empty");
  }

  if (zend_hash_num_elements(HASH_OF(options)) == 0 ||
      Z_TYPE_P(options) == IS_NULL)
  {
    add_assoc_string(options, "descending", "false");
    add_assoc_string(options, "conflicts", "false");
  }

  BOOL_ARG_TO_STRING(options, opt);
  URL_ENCODE(options, querystr, separator);
  smart_str_0(&querystr);

  intern = Z_COUCHOBJ_P(obj);
  if (intern != NULL)
  {
    COUCH_ACTION_START()
    {
      std::string retval = intern->couch->changes(ZSTR_VAL(database),
                                                  ZSTR_VAL(querystr.s));
      smart_str_free(&querystr);
      efree(querystr.s);
      COUCHDB_RETURN(retval, intern->json);
    }
    COUCH_ACTION_END();
  }
}

/**
 * @brief PHP CouchDb replicate method
 * 
 */
static void replicate(INTERNAL_FUNCTION_PARAMETERS)
{
  zval *options;
  smart_str jsonData = {0};

  zval *obj = getThis();
  couchdb_object *intern;

  ZEND_PARSE_PARAMETERS_START(1, 1)
  Z_PARAM_ARRAY(options)
  ZEND_PARSE_PARAMETERS_END();

  php_json_encode(&jsonData, options, 0);
  smart_str_0(&jsonData);

  intern = Z_COUCHOBJ_P(obj);
  if (intern != NULL)
  {
    COUCH_ACTION_START()
    {
      std::string retval = intern->couch->replicate(ZSTR_VAL(jsonData.s));
      smart_str_free(&jsonData);
      efree(jsonData.s);
      COUCHDB_RETURN(retval, intern->json);
    }
    COUCH_ACTION_END();
  }
}
