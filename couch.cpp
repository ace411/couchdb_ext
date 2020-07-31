/**
 * @file couch.cpp
 * @author Lochemem Bruno Michael (lochbm@gmail.com)
 * @brief 
 * @version 0.1.0
 */
#include "php_couchdb_ext.h"
#include "request.h"
#include "ext/standard/php_var.h"

#ifdef HAVE_SPL
#include "ext/spl/spl_exceptions.h"
#endif

zend_object_handlers request_object_handlers;

typedef struct _request_object
{
    Request *request;
    zend_object std;
} request_object;

static inline request_object *php_request_obj_from_obj(zend_object *obj)
{
    return (request_object *)((char *)(obj)-XtOffsetOf(request_object, std));
}

#define Z_TSTOBJ_P(zv) php_request_obj_from_obj(Z_OBJ_P((zv)))

zend_class_entry *request_ce;
zend_class_entry *request_exception_ce;

/* {{{ proto CouchDb::__construct(array options)
*/
PHP_METHOD(Request, __construct)
{
    // database options array
    zval *options;
    // database option keys
    zend_string *host, *user, *pass, *timeout, *port;

    long portVal, timeoutVal;

    zval *id = getThis();
    request_object *intern;

    ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_ARRAY(options)
    ZEND_PARSE_PARAMETERS_END();

    host = zend_string_init("host", sizeof("host") - 1, 1);
    user = zend_string_init("user", sizeof("user") - 1, 1);
    pass = zend_string_init("pass", sizeof("pass") - 1, 1);
    port = zend_string_init("port", sizeof("port") - 1, 1);
    timeout = zend_string_init("timeout", sizeof("timeout") - 1, 1);

    // check if host, user, and pass keys exist
    if (!zend_hash_exists(HASH_OF(options), host) ||
        !zend_hash_exists(HASH_OF(options), user) ||
        !zend_hash_exists(HASH_OF(options), pass))
    {
        zend_string_release(port);
        zend_string_release(timeout);
        zend_throw_exception(request_exception_ce,
                             "'host', 'user', and 'pass' are mandatory config options",
                             0 TSRMLS_CC);
        RETURN_NULL();
    }

    intern = Z_TSTOBJ_P(id);
    if (intern != NULL)
    {
        // set default port to 5984 if port key is absent
        portVal = !zend_hash_exists(HASH_OF(options), port)
                      ? long(5984)
                      : Z_LVAL_P(zend_hash_find(HASH_OF(options), port));

        // set default timeout to 60s if timeout key is absent
        timeoutVal = !zend_hash_exists(HASH_OF(options), timeout)
                         ? long(60)
                         : Z_LVAL_P(zend_hash_find(HASH_OF(options), timeout));

        intern->request = new Request(Z_STRVAL_P(zend_hash_find(HASH_OF(options), host)),
                                      Z_STRVAL_P(zend_hash_find(HASH_OF(options), user)),
                                      Z_STRVAL_P(zend_hash_find(HASH_OF(options), pass)),
                                      portVal,
                                      timeoutVal);
    }
}
/* }}} */

/* {{{ proto string CouchDb::uuids(int count)
*/
PHP_METHOD(Request, uuids)
{
    long idCount = 1;
    zval *id = getThis();
    request_object *intern;

    ZEND_PARSE_PARAMETERS_START(0, 1)
    Z_PARAM_LONG(idCount)
    ZEND_PARSE_PARAMETERS_END();

    intern = Z_TSTOBJ_P(id);
    if (intern != NULL)
    {
        std::string retval = intern->request->uuids(idCount);
        RETURN_STRING(retval.c_str());
    }
}
/* }}} */

/* {{{ proto bool CouchDb::isAvailable(void)
*/
PHP_METHOD(Request, isAvailable)
{
    zval *id = getThis();
    request_object *intern;

#ifdef ZEND_PARSE_PARAMETERS_NONE
    ZEND_PARSE_PARAMETERS_NONE();
#endif
    if (zend_parse_parameters_none() == FAILURE)
    {
        RETURN_NULL();
    }

    intern = Z_TSTOBJ_P(id);
    if (intern != NULL)
        RETURN_BOOL(intern->request->isAvailable());
}
/* }}} */

/* {{{ proto string CouchDb::allDbs(void)
*/
PHP_METHOD(Request, allDbs)
{
    zval *id = getThis();
    request_object *intern;
#ifdef ZEND_PARSE_PARAMETERS_NONE
    ZEND_PARSE_PARAMETERS_NONE();
#endif
    if (zend_parse_parameters_none() == FAILURE)
    {
        RETURN_NULL();
    }

    intern = Z_TSTOBJ_P(id);
    if (intern != NULL)
    {
        std::string retval = intern->request->allDbs();
        RETURN_STRING(retval.c_str())
    }
}
/* }}} */

/* {{{ proto string CouchDb::allDocs(string database)
*/
PHP_METHOD(Request, allDocs)
{
    zend_string *database; // *attrKey;
    std::string errStr("");
    zval *val;
    char *argSep = NULL;
    zval *queryParams; // *queryAttr, *finalQuery;

    smart_str queryBuild = {0};

    zval *id = getThis();
    request_object *intern;

    ZEND_PARSE_PARAMETERS_START(2, 2)
    Z_PARAM_STR(database)
    Z_PARAM_ARRAY(queryParams)
    ZEND_PARSE_PARAMETERS_END();

    // throw exception if the query parameter count is zero or database name is empty
    if (zend_hash_num_elements(HASH_OF(queryParams)) == 0 ||
        ZSTR_LEN(database) == 0)
    {
        zend_string_release(database);
        zend_throw_exception(request_exception_ce,
                             "Argument(s) cannot be empty",
                             0 TSRMLS_CC);
        RETURN_NULL();
    }

    // url-encode query parameters; throw exception if parameters cannot be encoded
    if (php_url_encode_hash_ex(HASH_OF(queryParams),
                               &queryBuild,
                               NULL,
                               0,
                               NULL,
                               0,
                               NULL,
                               0,
                               (Z_TYPE_P(queryParams) == IS_OBJECT ? queryParams : NULL),
                               argSep,
                               1) == FAILURE)
    {
        zend_string_release(database);
        zend_throw_exception(request_exception_ce, "Cannot encode query", 0 TSRMLS_CC);
        RETURN_NULL();
    }

    smart_str_0(&queryBuild);

    intern = Z_TSTOBJ_P(id);
    if (intern != NULL)
    {
        std::string retval = intern->request->allDocs(ZSTR_VAL(database), ZSTR_VAL(queryBuild.s));
        smart_str_free(&queryBuild);
        efree(queryBuild.s);
        RETURN_STRING(retval.c_str());
    }
}
/* }}} */

/* {{{ proto string CouchDb::insertDocs(string database, array docs)
*/
PHP_METHOD(Request, insertDocs)
{
    zend_string *database, *docKey;
    zval *docData;
    smart_str jsonData = {0};

    zval *id = getThis();
    request_object *intern;

    ZEND_PARSE_PARAMETERS_START(2, 2)
    Z_PARAM_STR(database)
    Z_PARAM_ARRAY(docData)
    ZEND_PARSE_PARAMETERS_END();

    docKey = zend_string_init("docs", sizeof("docs") - 1, 1);

    // throw exception if parameters are empty
    if (zend_hash_num_elements(HASH_OF(docData)) == 0 || ZSTR_LEN(database) == 0)
    {
        zend_string_release(docKey);
        zend_string_release(database);
        zend_throw_exception(request_exception_ce,
                             "Argument(s) cannot be empty",
                             0 TSRMLS_CC);
        RETURN_NULL();
    }

    // throw exception if docs key is missing
    if (!zend_hash_exists(Z_ARRVAL_P(docData), docKey))
    {
        zend_string_release(docKey);
        zend_string_release(database);
        zend_throw_exception(request_exception_ce,
                             "'docs' key is missing",
                             0 TSRMLS_CC);
        RETURN_NULL();
    }

    php_json_encode(&jsonData, docData, 0);
    smart_str_0(&jsonData);

    intern = Z_TSTOBJ_P(id);
    if (intern != NULL)
    {
        bool retval = intern->request->insertDocs(ZSTR_VAL(database),
                                                  ZSTR_VAL(jsonData.s));
        smart_str_free(&jsonData);
        efree(jsonData.s);
        RETURN_BOOL(retval);
    }
}
/* }}} */

/* {{{ proto string CouchDb::search(string database, array query)
*/
PHP_METHOD(Request, search)
{
    zend_string *database;
    zval *query;
    smart_str jsonData = {0};
    zend_string *selectorKey;
    zval *id = getThis();
    request_object *intern;

    ZEND_PARSE_PARAMETERS_START(2, 2)
    Z_PARAM_STR(database)
    Z_PARAM_ARRAY(query)
    ZEND_PARSE_PARAMETERS_END();

    selectorKey = zend_string_init("selector", (sizeof("selector") - 1), 1);

    if (zend_hash_num_elements(HASH_OF(query)) == 0 || ZSTR_LEN(database) == 0)
    {
        zend_string_release(database);
        zend_string_release(selectorKey);
        zend_throw_exception(request_exception_ce,
                             "Argument(s) cannot be empty",
                             0 TSRMLS_CC);
        RETURN_NULL();
    }

    // check if selector key exists
    if (!zend_hash_exists(HASH_OF(query), selectorKey))
    {
        zend_string_release(database);
        zend_string_release(selectorKey);
        zend_throw_exception(request_exception_ce,
                             "'selector' key is missing",
                             0 TSRMLS_CC);
        RETURN_NULL();
    }

    php_json_encode(&jsonData, query, 0);
    smart_str_0(&jsonData);

    intern = Z_TSTOBJ_P(id);
    if (intern != NULL)
    {
        std::string retval = intern->request->search(ZSTR_VAL(database),
                                                     ZSTR_VAL(jsonData.s));
        smart_str_free(&jsonData);
        RETURN_STRING(retval.c_str());
        zend_string_release(selectorKey);
    }
}
/* }}} */

/* {{{ proto string CouchDb::createDdoc(string database, string ddoc, array options)
*/
PHP_METHOD(Request, createDdoc)
{
    zend_string *database, *ddoc;
    zval *docData;
    smart_str jsonData = {0};

    zval *id = getThis();
    request_object *intern;

    ZEND_PARSE_PARAMETERS_START(3, 3)
    Z_PARAM_STR(database)
    Z_PARAM_STR(ddoc)
    Z_PARAM_ARRAY(docData)
    ZEND_PARSE_PARAMETERS_END();

    if (zend_hash_num_elements(HASH_OF(docData)) == 0 ||
        ZSTR_LEN(database) == 0 ||
        ZSTR_LEN(ddoc) == 0)
    {
        zend_string_release(ddoc);
        zend_string_release(database);
        zend_throw_exception(request_exception_ce,
                             "Argument(s) cannot be empty",
                             0 TSRMLS_CC);
        RETURN_NULL();
    }

    php_json_encode(&jsonData, docData, 0);
    smart_str_0(&jsonData);

    intern = Z_TSTOBJ_P(id);
    if (intern != NULL)
    {
        bool retval = intern->request->createDdoc(ZSTR_VAL(database),
                                                  ZSTR_VAL(ddoc),
                                                  ZSTR_VAL(jsonData.s));
        smart_str_free(&jsonData);
        RETURN_BOOL(retval);
    }
}
/* }}} */

/* {{{ proto string CouchDb::queryView(string database, string ddoc, string view, array options)
*/
PHP_METHOD(Request, queryView)
{
    zval *queryData;
    zend_string *database;
    zend_string *ddoc;
    zend_string *view;
    char *argSep = NULL;
    smart_str queryStr = {0};

    zval *id = getThis();
    request_object *intern;

    ZEND_PARSE_PARAMETERS_START(4, 4)
    Z_PARAM_STR(database)
    Z_PARAM_STR(ddoc)
    Z_PARAM_STR(view)
    Z_PARAM_ARRAY(queryData)
    ZEND_PARSE_PARAMETERS_END();

    if (zend_hash_num_elements(HASH_OF(queryData)) == 0 ||
        ZSTR_LEN(database) == 0 ||
        ZSTR_LEN(ddoc) == 0 ||
        ZSTR_LEN(view) == 0)
    {
        zend_string_release(database);
        zend_string_release(ddoc);
        zend_string_release(view);
        zend_throw_exception(request_exception_ce,
                             "Argument(s) cannot be empty",
                             0 TSRMLS_CC);
        RETURN_NULL();
    }

    if (php_url_encode_hash_ex(HASH_OF(queryData),
                               &queryStr,
                               NULL,
                               0,
                               NULL,
                               0,
                               NULL,
                               0,
                               (Z_TYPE_P(queryData) == IS_OBJECT ? queryData : NULL),
                               argSep,
                               1) == FAILURE)
    {
        zend_string_release(database);
        zend_string_release(ddoc);
        zend_string_release(view);
        zend_throw_exception(request_exception_ce,
                             "Cannot encode query parameters",
                             0 TSRMLS_CC);
        RETURN_NULL();
    }

    smart_str_0(&queryStr);

    intern = Z_TSTOBJ_P(id);
    if (intern != NULL)
    {
        std::string retval = intern->request->queryView(ZSTR_VAL(database),
                                                        ZSTR_VAL(ddoc),
                                                        ZSTR_VAL(view),
                                                        ZSTR_VAL(queryStr.s));
        smart_str_free(&queryStr);
        efree(queryStr.s);
        RETURN_STRING(retval.c_str());
    }
}
/* }}} */

/* {{{ proto string CouchDb::createDb(string database)
*/
PHP_METHOD(Request, createDb)
{
    zend_string *database;
    zval *id = getThis();
    request_object *intern;

    ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_STR(database)
    ZEND_PARSE_PARAMETERS_END();

    if (ZSTR_LEN(database) == 0)
    {
        zend_string_release(database);
        zend_throw_exception(request_exception_ce,
                             "Database name cannot be empty",
                             0 TSRMLS_CC);
        RETURN_NULL();
    }

    intern = Z_TSTOBJ_P(id);
    if (intern != NULL)
    {
        RETURN_BOOL(intern->request->createDb(ZSTR_VAL(database)));
        zend_string_release(database);
    }
}
/* }}} */

/* {{{ proto bool CouchDb::deleteDb(string database)
*/
PHP_METHOD(Request, deleteDb)
{
    zend_string *database;
    zval *id = getThis();
    request_object *intern;

    ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_STR(database)
    ZEND_PARSE_PARAMETERS_END();

    if (ZSTR_LEN(database) == 0)
    {
        zend_string_release(database);
        zend_throw_exception(request_exception_ce,
                             "Database name cannot be empty",
                             0 TSRMLS_CC);
        RETURN_NULL();
    }

    intern = Z_TSTOBJ_P(id);
    if (intern != NULL)
    {
        RETURN_BOOL(intern->request->deleteOpt(ZSTR_VAL(database)));
        zend_string_release(database);
    }
}
/* }}} */

/* {{{ proto bool CouchDb::deleteDoc(string database, string docId, string docRev)
*/
PHP_METHOD(Request, deleteDoc)
{
    zend_string *database, *docId, *docRev;
    zval *params;

    zval *id = getThis();
    request_object *intern;

    std::string deleteParam("");

    ZEND_PARSE_PARAMETERS_START(3, 3)
    Z_PARAM_STR(database)
    Z_PARAM_STR(docId)
    Z_PARAM_STR(docRev)
    ZEND_PARSE_PARAMETERS_END();

    // throw an exception if any of the arguments is empty
    if (ZSTR_LEN(database) == 0 ||
        ZSTR_LEN(docRev) == 0 ||
        ZSTR_LEN(docId) == 0)
    {
        zend_string_release(database);
        zend_string_release(docRev);
        zend_string_release(docId);
        zend_throw_exception(request_exception_ce,
                             "Argument(s) cannot be empty",
                             0 TSRMLS_CC);
        RETURN_NULL();
    }

    intern = Z_TSTOBJ_P(id);
    if (intern != NULL)
    {
        // {host}/{database}/{docId}?rev={docRev}
        deleteParam += ZSTR_VAL(database) +
                       std::string("/") +
                       ZSTR_VAL(docId) +
                       std::string("?rev=") +
                       ZSTR_VAL(docRev);

        RETURN_BOOL(intern->request->deleteOpt(deleteParam));

        zend_string_release(docId);
        zend_string_release(docRev);
        zend_string_release(database);
    }
}
/* }}} */

/* {{{ proto bool CouchDb::deleteDocs(string database, array docs)
*/
PHP_METHOD(Request, deleteDocs)
{
    zend_string *database;
    zval *docs, *jsonFinal, *jsonDocs, *docVal;

    smart_str jsonData = {0};

    zval *id = getThis();
    request_object *intern;

    ZEND_PARSE_PARAMETERS_START(2, 2)
    Z_PARAM_STR(database)
    Z_PARAM_ARRAY(docs)
    ZEND_PARSE_PARAMETERS_END();

    if (zend_hash_num_elements(HASH_OF(docs)) == 0)
    {
        zend_string_release(database);
        zend_throw_exception(request_exception_ce,
                             "docs array cannot be empty",
                             0 TSRMLS_CC);
        RETURN_NULL();
    }

    array_init(jsonDocs);
    array_init(jsonFinal);

    intern = Z_TSTOBJ_P(id);
    if (intern != NULL)
    {
        ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(docs), docVal)
        {
            zval dup;
            ZVAL_COPY(&dup, docVal);

            SEPARATE_ARRAY(&dup); // handle copy-on-write for shared array
            convert_to_array(&dup);

            add_assoc_bool(&dup, "_deleted", 1);
            add_next_index_zval(jsonDocs, &dup);
        }
        ZEND_HASH_FOREACH_END();

        add_assoc_zval(jsonFinal, "docs", jsonDocs);
        php_json_encode(&jsonData, jsonFinal, 0);
        smart_str_0(&jsonData);

        RETURN_BOOL(intern->request->insertDocs(ZSTR_VAL(database),
                                                ZSTR_VAL(jsonData.s)));
        smart_str_free(&jsonData);
        zend_string_release(database);
    }
}
/* }}} */

/* {{{ proto bool CouchDb::updateDoc(string database, string docId, string docRev, array doc)
*/
PHP_METHOD(Request, updateDoc)
{
    zend_string *database, *docId, *docRev;
    zval *doc;

    smart_str jsonData = {0};

    zval *id = getThis();
    request_object *intern;

    ZEND_PARSE_PARAMETERS_START(4, 4)
    Z_PARAM_STR(database)
    Z_PARAM_STR(docId)
    Z_PARAM_STR(docRev)
    Z_PARAM_ARRAY(doc)
    ZEND_PARSE_PARAMETERS_END();

    // throw exception if any of the arguments are empty
    if (zend_hash_num_elements(HASH_OF(doc)) == 0 ||
        ZSTR_LEN(database) == 0 ||
        ZSTR_LEN(docRev) == 0 ||
        ZSTR_LEN(docId) == 0)
    {
        zend_string_release(docId);
        zend_string_release(docRev);
        zend_string_release(database);
        zend_throw_exception(request_exception_ce,
                             "Argument(s) cannot be empty",
                             0 TSRMLS_CC);
        RETURN_NULL();
    }

    intern = Z_TSTOBJ_P(id);
    if (intern != NULL)
    {
        php_json_encode(&jsonData, doc, 0);
        smart_str_0(&jsonData);

        RETURN_BOOL(intern->request->updateSingle(ZSTR_VAL(database),
                                                  ZSTR_VAL(docId),
                                                  ZSTR_VAL(docRev),
                                                  ZSTR_VAL(jsonData.s)));
        smart_str_free(&jsonData);
        zend_string_release(docId);
        zend_string_release(docRev);
        zend_string_release(database);
    }
}
/* }}} */

/* {{{ proto bool CouchDb::updateDocs(string database, array docs)
*/
PHP_METHOD(Request, updateDocs)
{
    zend_string *database, *docKey;
    zval *doc, *docVal;

    smart_str jsonData = {0};

    zval *id = getThis();
    request_object *intern;

    ZEND_PARSE_PARAMETERS_START(2, 2)
    Z_PARAM_STR(database)
    Z_PARAM_ARRAY(doc)
    ZEND_PARSE_PARAMETERS_END();

    docKey = zend_string_init("docs", sizeof("docs") - 1, 1);

    if (zend_hash_num_elements(HASH_OF(doc)) == 0 ||
        ZSTR_LEN(database) == 0)
    {
        zend_string_release(docKey);
        zend_string_release(database);
        zend_throw_exception(request_exception_ce,
                             "Argument(s) cannot be empty",
                             0 TSRMLS_CC);
        RETURN_NULL();
    }

    // throw exception if docs key is missing
    if (!zend_hash_exists(Z_ARRVAL_P(doc), docKey))
    {
        zend_string_release(docKey);
        zend_string_release(database);
        zend_throw_exception(request_exception_ce,
                             "'docs' key is missing",
                             0 TSRMLS_CC);
        RETURN_NULL();
    }

    intern = Z_TSTOBJ_P(id);
    if (intern != NULL)
    {
        ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(doc), docVal)
        {
            if (Z_TYPE_P(docVal) != IS_ARRAY)
            {
                zend_throw_exception(request_exception_ce,
                                     "Only array values are accepted",
                                     0 TSRMLS_CC);
                RETURN_NULL();
            }
        }
        ZEND_HASH_FOREACH_END();

        php_json_encode(&jsonData, doc, 0);
        smart_str_0(&jsonData);

        RETURN_BOOL(intern->request->insertDocs(ZSTR_VAL(database),
                                                ZSTR_VAL(jsonData.s)));
        smart_str_free(&jsonData);
        zend_string_release(database);
        zend_string_release(docKey);
    }
}
/* }}} */

/* {{{ proto string CouchDb::getDoc(string database, string docId)
*/
PHP_METHOD(Request, getDoc)
{
    zend_string *database, *docId;

    zval *id = getThis();
    request_object *intern;

    ZEND_PARSE_PARAMETERS_START(2, 2)
    Z_PARAM_STR(database)
    Z_PARAM_STR(docId)
    ZEND_PARSE_PARAMETERS_END();

    if (ZSTR_LEN(database) == 0 || ZSTR_LEN(docId) == 0)
    {
        zend_throw_exception(request_exception_ce,
                             "Argument(s) cannot be empty",
                             0 TSRMLS_CC);
        RETURN_NULL();
    }

    intern = Z_TSTOBJ_P(id);
    if (intern != NULL)
    {
        std::string result = intern->request->getDoc(ZSTR_VAL(database),
                                                     ZSTR_VAL(docId));
        RETURN_STRING(result.c_str());
    }

    zend_string_release(database);
    zend_string_release(docId);
}
/* }}} */

/* {{{ proto bool createIndex(string database, array index, bool partial = false) 
*/
PHP_METHOD(Request, createIndex)
{
    zend_string *database, *idxKey, *partialKey;
    zval *index, idxVal;
    // initialize partial index flag
    zend_bool partial = 0;

    smart_str jsonData = {0};

    zval *id = getThis();
    request_object *intern;

    ZEND_PARSE_PARAMETERS_START(2, 3)
    Z_PARAM_STR(database)
    Z_PARAM_ARRAY(index)
    Z_PARAM_OPTIONAL
    Z_PARAM_BOOL(partial)
    ZEND_PARSE_PARAMETERS_END();

    // initialize the index key
    idxKey = zend_string_init("index", sizeof("index") - 1, 1);

    // throw an exception if any of the args are empty
    if (!zend_hash_num_elements(Z_ARRVAL_P(index)) ||
        ZSTR_LEN(database) == 0)
    {
        zend_string_release(database);
        zend_string_release(idxKey);
        zend_throw_exception(request_exception_ce,
                             "Argument(s) cannot be empty",
                             0 TSRMLS_CC);
        RETURN_NULL();
    }

    // throw exception if index does not exist
    if (!zend_hash_exists(Z_ARRVAL_P(index), idxKey))
    {
        zend_string_release(database);
        zend_string_release(idxKey);
        zend_throw_exception(request_exception_ce,
                             "'index' key is missing",
                             0 TSRMLS_CC);
        RETURN_NULL();
    }

    ZVAL_COPY(&idxVal, zend_hash_find(Z_ARRVAL_P(index), idxKey));

    // throw exception if the index is not an array
    if (Z_TYPE_P(&idxVal) != IS_ARRAY)
    {
        zend_string_release(database);
        zend_string_release(idxKey);
        zend_throw_exception(request_exception_ce,
                             "'index' must be an array",
                             0 TSRMLS_CC);
        RETURN_NULL();
    }

    intern = Z_TSTOBJ_P(id);
    if (intern != NULL)
    {
        if (partial == 1)
        {
            partialKey = zend_string_init("partial_filter_selector",
                                          sizeof("partial_filter_selector") - 1,
                                          1);
            if (!zend_hash_exists(Z_ARRVAL_P(&idxVal), partialKey))
            {
                zend_string_release(database);
                zend_string_release(idxKey);
                zend_string_release(partialKey);
                zend_throw_exception(request_exception_ce,
                                     "'partial_filter_selector' key is missing",
                                     0 TSRMLS_CC);
                RETURN_NULL();
            }
        }

        php_json_encode(&jsonData, index, 0);
        smart_str_0(&jsonData);

        std::string result = intern->request->createIndex(ZSTR_VAL(database),
                                                          ZSTR_VAL(jsonData.s));
        RETURN_STRING(result.c_str());
        smart_str_free(&jsonData);
    }

    zend_string_release(idxKey);
    zend_string_release(database);
}
/* }}} */

/* {{{ proto string changes(string database, [array options = []])
*/
PHP_METHOD(Request, changes)
{
    zend_string *database;
    zval *options;

    smart_str queryStr = {0};
    char *argSep = NULL;

    array_init(options);

    zval *id = getThis();
    request_object *intern;

    ZEND_PARSE_PARAMETERS_START(1, 2)
    Z_PARAM_STR(database)
    Z_PARAM_OPTIONAL
    Z_PARAM_ARRAY(options)
    ZEND_PARSE_PARAMETERS_END();

    if (ZSTR_LEN(database) == 0)
    {
        zend_string_release(database);
        zend_throw_exception(request_exception_ce,
                             "Argument(s) cannot be empty",
                             0 TSRMLS_CC);
        RETURN_NULL();
    }

    if (zend_hash_num_elements(Z_ARRVAL_P(options)) == 0)
    {
        add_assoc_string(options, "conflicts", "false");
        add_assoc_string(options, "descending", "false");
    }

    intern = Z_TSTOBJ_P(id);
    if (intern != NULL)
    {
        if (php_url_encode_hash_ex(Z_ARRVAL_P(options),
                                   &queryStr,
                                   NULL,
                                   0,
                                   NULL,
                                   0,
                                   NULL,
                                   0,
                                   (Z_TYPE_P(options) == IS_OBJECT ? options : NULL),
                                   argSep,
                                   1) == FAILURE)
        {
            zend_throw_exception(request_exception_ce,
                                 "Cannot encode parameters",
                                 0 TSRMLS_CC);
            RETURN_NULL();
        }
        smart_str_0(&queryStr);

        std::string result = intern->request->changes(ZSTR_VAL(database),
                                                      ZSTR_VAL(queryStr.s));
        RETURN_STRING(result.c_str());
        smart_str_free(&queryStr);

        zend_string_release(database);
    }
}
/* }}} */

ZEND_BEGIN_ARG_INFO_EX(arginfo_constructor, 0, 0, 1)
ZEND_ARG_ARRAY_INFO(0, options, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_uuids, 0, 0, 1)
ZEND_ARG_INFO(0, idCount)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_alldocs, 0, 0, 2)
ZEND_ARG_INFO(0, database)
ZEND_ARG_ARRAY_INFO(0, options, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_getdoc, 0, 0, 2)
ZEND_ARG_INFO(0, database)
ZEND_ARG_INFO(0, docId)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_insertdocs, 0, 0, 2)
ZEND_ARG_INFO(0, database)
ZEND_ARG_ARRAY_INFO(0, documents, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_search, 0, 0, 2)
ZEND_ARG_INFO(0, database)
ZEND_ARG_ARRAY_INFO(0, query, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_createddoc, 0, 0, 3)
ZEND_ARG_INFO(0, database)
ZEND_ARG_INFO(0, ddoc)
ZEND_ARG_ARRAY_INFO(0, docdata, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_queryview, 0, 0, 4)
ZEND_ARG_INFO(0, database)
ZEND_ARG_INFO(0, ddoc)
ZEND_ARG_INFO(0, view)
ZEND_ARG_ARRAY_INFO(0, params, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_createdb, 0, 0, 1)
ZEND_ARG_INFO(0, database)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_deletedoc, 0, 0, 3)
ZEND_ARG_INFO(0, database)
ZEND_ARG_INFO(0, docId)
ZEND_ARG_INFO(0, docRev)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_deletedocs, 0, 0, 2)
ZEND_ARG_INFO(0, database)
ZEND_ARG_ARRAY_INFO(0, documents, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_updatesingle, 0, 0, 4)
ZEND_ARG_INFO(0, database)
ZEND_ARG_INFO(0, docId)
ZEND_ARG_INFO(0, docRev)
ZEND_ARG_ARRAY_INFO(0, document, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_createindex, 0, 0, 3)
ZEND_ARG_INFO(0, database)
ZEND_ARG_ARRAY_INFO(0, index, 0)
ZEND_ARG_INFO(0, isPartial)
ZEND_END_ARG_INFO();

static const zend_function_entry request_methods[] = {
    PHP_ME(Request, __construct, arginfo_constructor, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
        PHP_ME(Request, uuids, arginfo_uuids, ZEND_ACC_PUBLIC)
            PHP_ME(Request, isAvailable, NULL, ZEND_ACC_PUBLIC)
                PHP_ME(Request, allDbs, NULL, ZEND_ACC_PUBLIC)
                    PHP_ME(Request, allDocs, arginfo_alldocs, ZEND_ACC_PUBLIC)
                        PHP_ME(Request, insertDocs, arginfo_insertdocs, ZEND_ACC_PUBLIC)
                            PHP_ME(Request, search, arginfo_search, ZEND_ACC_PUBLIC)
                                PHP_ME(Request, createDdoc, arginfo_createddoc, ZEND_ACC_PUBLIC)
                                    PHP_ME(Request, queryView, arginfo_queryview, ZEND_ACC_PUBLIC)
                                        PHP_ME(Request, createDb, arginfo_createdb, ZEND_ACC_PUBLIC)
                                            PHP_ME(Request, deleteDb, arginfo_createdb, ZEND_ACC_PUBLIC)
                                                PHP_ME(Request, deleteDoc, arginfo_deletedoc, ZEND_ACC_PUBLIC)
                                                    PHP_ME(Request, deleteDocs, arginfo_deletedocs, ZEND_ACC_PUBLIC)
                                                        PHP_ME(Request, updateDoc, arginfo_updatesingle, ZEND_ACC_PUBLIC)
                                                            PHP_ME(Request, updateDocs, arginfo_deletedocs, ZEND_ACC_PUBLIC)
                                                                PHP_ME(Request, getDoc, arginfo_getdoc, ZEND_ACC_PUBLIC)
                                                                    PHP_ME(Request, createIndex, arginfo_createindex, ZEND_ACC_PUBLIC)
                                                                        PHP_ME(Request, changes, arginfo_alldocs, ZEND_ACC_PUBLIC)
                                                                            PHP_FE_END};

zend_object *request_object_new(zend_class_entry *ce TSRMLS_DC)
{
    request_object *intern = (request_object *)ecalloc(1,
                                                       sizeof(request_object) +
                                                           zend_object_properties_size(ce));

    zend_object_std_init(&intern->std, ce TSRMLS_CC);
    object_properties_init(&intern->std, ce);

    intern->std.handlers = &request_object_handlers;

    return &intern->std;
}

static void request_object_destroy(zend_object *object)
{
    request_object *my_obj;
    my_obj = (request_object *)((char *)object - XtOffsetOf(request_object, std));

    zend_objects_destroy_object(object);
}

static void request_object_free(zend_object *object)
{
    request_object *my_obj;
    my_obj = (request_object *)((char *)object - XtOffsetOf(request_object, std));
    delete my_obj->request;

    zend_object_std_dtor(object);
}

PHP_MINIT_FUNCTION(request)
{
    zend_class_entry ce;
    zend_class_entry exception_ce;
    INIT_CLASS_ENTRY(ce, "CouchDb", request_methods);
    INIT_CLASS_ENTRY(exception_ce, "CouchDbException", NULL);
    request_ce = zend_register_internal_class(&ce TSRMLS_CC);
    request_ce->create_object = request_object_new;

    memcpy(&request_object_handlers,
           zend_get_std_object_handlers(),
           sizeof(request_object_handlers));

    request_object_handlers.free_obj = request_object_free;
    request_object_handlers.dtor_obj = request_object_destroy;

    request_object_handlers.offset = XtOffsetOf(request_object, std);

#ifdef HAVE_SPL
    request_exception_ce = zend_register_internal_class_ex(
        &exception_ce, spl_ce_RuntimeException);
#else
    request_exception_ce = zend_register_internal_class_ex(
        &exception_ce, zend_exception_get_default(TSRMLS_C));
#endif

    return SUCCESS;
}

PHP_MINFO_FUNCTION(request)
{
    // print extension information
    php_info_print_table_start();
    php_info_print_table_header(2, "couchdb_ext support", "enabled");
    php_info_print_table_header(2, "couchdb_ext version", PHP_COUCHDB_EXT_EXTVER);
    php_info_print_table_header(2, "couchdb_ext author", "Lochemem Bruno Michael <lochbm@gmail.com>");
    php_info_print_table_end();
}

PHP_RINIT_FUNCTION(request)
{
#if defined(ZTS) && defined(COMPILE_DL_COUCHDB_EXT)
    ZEND_TSRMLS_CACHE_UPDATE();
#endif

    return SUCCESS;
}

/*static const zend_module_dep request_deps[] = {
    ZEND_MOD_REQUIRED("json")
        ZEND_MOD_END};*/

zend_module_entry couchdb_ext_module_entry = {
    STANDARD_MODULE_HEADER,
    PHP_COUCHDB_EXT_EXTNAME,
    NULL,
    PHP_MINIT(request),            // MINIT
    NULL,                          // MSHUTDOWN
    NULL,                          // RINIT
    NULL,                          // RSHUTDOWN
    PHP_MINFO(request),            // MINFO
#if ZEND_MODULE_API_NO >= 20170718 // check if PHP version is greater than 7.2
    PHP_COUCHDB_EXT_EXTVER,
#endif
    STANDARD_MODULE_PROPERTIES};

#ifdef COMPILE_DL_COUCHDB_EXT
extern "C"
{
    ZEND_GET_MODULE(couchdb_ext)
}
#endif
