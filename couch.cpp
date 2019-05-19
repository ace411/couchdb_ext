#include "php_couchdb_ext.h"
#include "request.h"
#include "zend_smart_str.h"

zend_object_handlers request_object_handlers;

typedef struct _request_object {
    Request *request;
    zend_object std;
} request_object;

static inline request_object *php_request_obj_from_obj(zend_object *obj)
{
    return (request_object *)((char *)(obj)-XtOffsetOf(request_object, std));
}

#define Z_TSTOBJ_P(zv) php_request_obj_from_obj(Z_OBJ_P((zv)))

zend_class_entry *request_ce;

PHP_METHOD(Request, __construct)
{
    zend_string* host;
    zend_string* user;
    zend_string* pwd;
    long timeout = 60, port = 5984;
    zval *id = getThis();
    request_object *intern;

    ZEND_PARSE_PARAMETERS_START(0, 5)
        Z_PARAM_STR(host)
        Z_PARAM_STR(user)
        Z_PARAM_STR(pwd)
        Z_PARAM_LONG(port)
        Z_PARAM_LONG(timeout)
    ZEND_PARSE_PARAMETERS_END();

    intern = Z_TSTOBJ_P(id);
    if (intern != NULL)
        intern->request = new Request(ZSTR_VAL(host), ZSTR_VAL(user), ZSTR_VAL(pwd), port, timeout);
}

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

PHP_METHOD(Request, isAvailable)
{
    zval *id = getThis();
    request_object *intern;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = Z_TSTOBJ_P(id);
    if (intern != NULL)
        RETURN_BOOL(intern->request->isAvailable());
}

PHP_METHOD(Request, allDbs)
{
    zval *id = getThis();
    request_object *intern;

    ZEND_PARSE_PARAMETERS_NONE(); 

    intern = Z_TSTOBJ_P(id);
    if (intern != NULL) 
    {
        std::string retval = intern->request->allDbs();
        RETURN_STRING(retval.c_str())
    }
}

PHP_METHOD(Request, allDocs)
{
    zend_string *database;
    HashTable *queryParams;
    zend_string *skey;
    zval *val;
    smart_str queryBuild = {0};

    zval *id = getThis();
    request_object *intern;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_STR(database)
        Z_PARAM_ARRAY_HT(queryParams)
    ZEND_PARSE_PARAMETERS_END();

    ZEND_HASH_FOREACH_STR_KEY_VAL(queryParams, skey, val) {
        zval dup;
        ZVAL_COPY_VALUE(&dup, val);
        convert_to_cstring(&dup);
        smart_str_appends(&queryBuild, ZSTR_VAL(skey));
        smart_str_appendc(&queryBuild, '=');
        smart_str_appends(&queryBuild, Z_STRVAL(dup));
        smart_str_appendc(&queryBuild, '&');
        zval_dtor(&dup);
    } ZEND_HASH_FOREACH_END();
    
    smart_str_0(&queryBuild);
    zval_dtor(val);
    zend_string_release(skey);

    intern = Z_TSTOBJ_P(id);
    if (intern != NULL) {
        std::string retval = intern->request->allDocs(ZSTR_VAL(database), ZSTR_VAL(queryBuild.s));
        smart_str_free(&queryBuild);
        efree(queryBuild.s);
        zend_hash_destroy(queryParams);
        FREE_HASHTABLE(queryParams);
        zend_string_release(database);
        RETURN_STRING(retval.c_str());
    }
}

PHP_METHOD(Request, insertDocs)
{
    zend_string *database;
    zval *docData;
    smart_str jsonData = {0};

    zval *id = getThis();
    request_object *intern;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_STR(database)
        Z_PARAM_ARRAY(docData)
    ZEND_PARSE_PARAMETERS_END();

    php_json_encode(&jsonData, docData, 0);
    smart_str_0(&jsonData);

    intern = Z_TSTOBJ_P(id);
    if (intern != NULL)
    {
        bool retval = intern->request->insertDocs(ZSTR_VAL(database), ZSTR_VAL(jsonData.s));
        smart_str_free(&jsonData);
        RETURN_BOOL(retval);
    }
}

PHP_METHOD(Request, search)
{
    zend_string *database;
    zval *query;
    smart_str jsonData = {0};

    zval *id = getThis();
    request_object *intern;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_STR(database)
        Z_PARAM_ARRAY(query)
    ZEND_PARSE_PARAMETERS_END();

    php_json_encode(&jsonData, query, 0);
    smart_str_0(&jsonData);

    intern = Z_TSTOBJ_P(id);
    if (intern != NULL)
    {
        std::string retval = intern->request->search(ZSTR_VAL(database), ZSTR_VAL(jsonData.s));
        smart_str_free(&jsonData);
        RETURN_STRING(retval.c_str());
    }
}

PHP_METHOD(Request, createDdoc)
{
    zend_string *database;
    zend_string *ddoc;
    zval *docData;
    smart_str jsonData = {0};

    zval *id = getThis();
    request_object *intern;

    ZEND_PARSE_PARAMETERS_START(3, 3)
        Z_PARAM_STR(database)
        Z_PARAM_STR(ddoc)
        Z_PARAM_ARRAY(docData)
    ZEND_PARSE_PARAMETERS_END();

    php_json_encode(&jsonData, docData, 0);
    smart_str_0(&jsonData);

    intern = Z_TSTOBJ_P(id);
    if (intern != NULL) 
    {
        bool retval = intern->request->createDdoc(ZSTR_VAL(database), ZSTR_VAL(ddoc), ZSTR_VAL(jsonData.s));
        smart_str_free(&jsonData);
        RETURN_BOOL(retval);
    }
}

PHP_METHOD(Request, queryView)
{
    zval *queryData;
    zend_string *database;
    zend_string *ddoc;
    zend_string *view;
    std::string errResult("");
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

    if (Z_TYPE_P(queryData) != IS_ARRAY || zend_hash_num_elements(HASH_OF(queryData)) == 0)
        RETURN_STRING(errResult.c_str());

    if (php_url_encode_hash_ex(HASH_OF(queryData), &queryStr, NULL, 0, NULL, 0, NULL, 0, (Z_TYPE_P(queryData) == IS_OBJECT ? queryData : NULL), argSep, 1) == FAILURE)
        RETURN_STRING(errResult.c_str());

    smart_str_0(&queryStr);

    intern = Z_TSTOBJ_P(id);
    if (intern != NULL)
    {
        std::string retval = intern->request->queryView(ZSTR_VAL(database), ZSTR_VAL(ddoc), ZSTR_VAL(view), ZSTR_VAL(queryStr.s));
        smart_str_free(&queryStr);
        efree(queryStr.s);
        RETURN_STRING(retval.c_str());
    }
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_constructor, 0, 0, 5)
    ZEND_ARG_INFO(0, host)
    ZEND_ARG_INFO(0, user)
    ZEND_ARG_INFO(0, pwd)
    ZEND_ARG_INFO(0, port)
    ZEND_ARG_INFO(0, timeout)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_uuids, 0, 0, 1)
    ZEND_ARG_INFO(0, idCount)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_alldocs, 0, 0, 2)
    ZEND_ARG_INFO(0, database)
    ZEND_ARG_ARRAY_INFO(0, options, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_insertdocs, 0, 0, 2)
    ZEND_ARG_INFO(0, database)
    ZEND_ARG_ARRAY_INFO(0, data, 0)
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
    PHP_FE_END
};

zend_object *request_object_new(zend_class_entry *ce TSRMLS_DC)
{
    request_object *intern = (request_object*)ecalloc(1,
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
    INIT_CLASS_ENTRY(ce, "CouchDb", request_methods);
    request_ce = zend_register_internal_class(&ce TSRMLS_CC);
    request_ce->create_object = request_object_new;

    memcpy(&request_object_handlers, 
        zend_get_std_object_handlers(), 
        sizeof(request_object_handlers));
    
    request_object_handlers.free_obj = request_object_free;
    request_object_handlers.dtor_obj = request_object_destroy;

    request_object_handlers.offset = XtOffsetOf(request_object, std);

    return SUCCESS;
}

zend_module_entry couchdb_ext_module_entry = {
    STANDARD_MODULE_HEADER,
    PHP_COUCHDB_EXT_EXTNAME,
    NULL,
    PHP_MINIT(request),
    NULL,
    NULL,
    NULL,
    NULL,
    PHP_COUCHDB_EXT_EXTVER,
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_COUCHDB_EXT
extern "C" {
    ZEND_GET_MODULE(couchdb_ext)
}
#endif