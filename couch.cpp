#include "php_couchdb_ext.h"
#include "request.h"
#include "zend_smart_str.h"
#include <tuple>

#define COUCH_DEL_DB 1
#define COUCH_DEL_DOC 2
#define COUCH_DEL_DDOC 3

#define COUCH_UPDATE_SINGLE 1
#define COUCH_UPDATE_MULTIPLE 2

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

    ZEND_PARSE_PARAMETERS_START(3, 5)
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
    std::string errStr("");
    zval *val;
    char *argSep = NULL;
    zval *queryParams;

    smart_str queryBuild = {0};

    zval *id = getThis();
    request_object *intern;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_STR(database)
        Z_PARAM_ARRAY(queryParams)
    ZEND_PARSE_PARAMETERS_END();

    if (zend_hash_num_elements(HASH_OF(queryParams)) == 0)
        RETURN_STRING(errStr.c_str());

    if (php_url_encode_hash_ex(HASH_OF(queryParams), &queryBuild, NULL, 0, NULL, 0, NULL, 0, (Z_TYPE_P(queryParams) == IS_OBJECT ? queryParams : NULL), argSep, 1) == FAILURE)
        RETURN_STRING(errStr.c_str());

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

PHP_METHOD(Request, insertDocs)
{
    zend_string *database;
    zval *docData;
    bool errRetval(false);
    smart_str jsonData = {0};

    zval *id = getThis();
    request_object *intern;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_STR(database)
        Z_PARAM_ARRAY(docData)
    ZEND_PARSE_PARAMETERS_END();

    if (zend_hash_num_elements(HASH_OF(docData)) == 0)
        RETURN_BOOL(errRetval);

    php_json_encode(&jsonData, docData, 0);
    smart_str_0(&jsonData);

    intern = Z_TSTOBJ_P(id);
    if (intern != NULL)
    {
        bool retval = intern->request->insertDocs(ZSTR_VAL(database), ZSTR_VAL(jsonData.s));
        smart_str_free(&jsonData);
        efree(jsonData.s);
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

PHP_METHOD(Request, createDb)
{
    zend_string *database;
    zval *id = getThis();
    request_object *intern;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_STR(database)
    ZEND_PARSE_PARAMETERS_END();

    intern = Z_TSTOBJ_P(id);
    if (intern != NULL)
    {
        RETURN_BOOL(intern->request->createDb(ZSTR_VAL(database)));
        zend_string_release(database);
    }
}

PHP_METHOD(Request, _delete)
{
    long opt;
    HashTable *opts;
    zval *retOpt;
    zval *_retOpt;
    zval *__retOpt;
    zend_string *dbkey;
    zend_string *idkey;
    zend_string *revkey;
    std::string strOpt("");

    zval *id = getThis();
    request_object *intern;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_LONG(opt)
        Z_PARAM_ARRAY_HT(opts)
    ZEND_PARSE_PARAMETERS_END();

    if (zend_hash_num_elements(opts) == 0)
        RETURN_BOOL(false);

    dbkey = zend_string_init("database", (sizeof("database") - 1), 1);
    revkey = zend_string_init("_rev", (sizeof("_rev") - 1), 1);
    idkey = zend_string_init("_id", (sizeof("_id") - 1), 1);

    if (!zend_hash_exists(opts, dbkey))
        RETURN_BOOL(false);

    retOpt = zend_hash_find(opts, dbkey);
    _retOpt = zend_hash_find(opts, revkey); 
    __retOpt = zend_hash_find(opts, idkey);

    strOpt += Z_STRVAL_P(retOpt);
    zend_string_release(dbkey);

    intern = Z_TSTOBJ_P(id);
    if (intern != NULL)
    {
        switch (opt)
        {
            case COUCH_DEL_DB:
                RETURN_BOOL(intern->request->deleteOpt(strOpt));
                break;

            case COUCH_DEL_DOC:
                if (!zend_hash_exists(opts, idkey) || !zend_hash_exists(opts, revkey))
                    RETURN_BOOL(false);

                zend_string_release(idkey);
                zend_string_release(revkey);

                strOpt += std::string("/") + Z_STRVAL_P(__retOpt) + "?rev=" + Z_STRVAL_P(_retOpt);
                RETURN_BOOL(intern->request->deleteOpt(strOpt));
                break;

            default:
                RETURN_BOOL(false);
                break;
        }

        zend_hash_destroy(opts);
        FREE_HASHTABLE(opts);
    }
}

PHP_METHOD(Request, update)
{
    zend_string *database;
    long opt;
    zval *update;

    zend_string *idkey;
    zend_string *revkey;

    smart_str docData = {0};

    zval *id = getThis();
    request_object *intern;

    ZEND_PARSE_PARAMETERS_START(3, 3)
        Z_PARAM_STR(database)
        Z_PARAM_LONG(opt)
        Z_PARAM_ARRAY(update)
    ZEND_PARSE_PARAMETERS_END();

    if (zend_hash_num_elements(HASH_OF(update)) == 0)
        RETURN_BOOL(false);

    idkey = zend_string_init("_id", sizeof("_id") - 1, 1);
    revkey = zend_string_init("_rev", sizeof("_rev") - 1, 1);

    intern = Z_TSTOBJ_P(id);

    if (intern != NULL)
    {
        switch (opt)
        {
            case COUCH_UPDATE_SINGLE:
                zend_string *dockey;

                zval *_opt;
                zval *__opt;
                zval *___opt;

                dockey = zend_string_init("doc", sizeof("doc") - 1, 1);

                if (!zend_hash_exists(HASH_OF(update), idkey) || !zend_hash_exists(HASH_OF(update), revkey) || !zend_hash_exists(HASH_OF(update), dockey))
                    RETURN_BOOL(false);

                _opt = zend_hash_find(HASH_OF(update), idkey);
                __opt = zend_hash_find(HASH_OF(update), revkey);
                ___opt = zend_hash_find(HASH_OF(update), dockey);
                
                if (Z_TYPE_P(___opt) != IS_ARRAY)
                    RETURN_BOOL(false);

                php_json_encode(&docData, ___opt, 0);
                smart_str_0(&docData);

                RETURN_BOOL(intern->request->updateSingle(ZSTR_VAL(database), 
                    Z_STRVAL_P(_opt), 
                    Z_STRVAL_P(__opt),
                    ZSTR_VAL(docData.s)));                    
                break;

            case COUCH_UPDATE_MULTIPLE:                
                zval *retval;
                zval *_docs;

                ZEND_HASH_FOREACH_VAL(HASH_OF(update), retval) {
                    if (Z_TYPE_P(retval) != IS_ARRAY)
                        RETURN_BOOL(false);

                    if (!zend_hash_exists(Z_ARRVAL_P(retval), idkey) || !zend_hash_exists(Z_ARRVAL_P(retval), revkey))
                        RETURN_BOOL(false);
                } ZEND_HASH_FOREACH_END();

                array_init(_docs);
                add_assoc_zval(_docs, "docs", update);

                php_json_encode(&docData, _docs, 0);
                smart_str_0(&docData);

                RETURN_BOOL(intern->request->insertDocs(ZSTR_VAL(database), ZSTR_VAL(docData.s)));
                break;


            default:
                RETURN_BOOL(false);
                break;
        }

        smart_str_free(&docData);
        efree(docData.s);
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

ZEND_BEGIN_ARG_INFO_EX(arginfo_createdb, 0, 0, 1)
    ZEND_ARG_INFO(0, database)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_delete, 0, 0, 2)
    ZEND_ARG_INFO(0, option)
    ZEND_ARG_ARRAY_INFO(0, params, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_update, 0, 0, 3)
    ZEND_ARG_INFO(0, database)
    ZEND_ARG_INFO(0, option)
    ZEND_ARG_ARRAY_INFO(0, update, 0)
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
    PHP_ME(Request, _delete, arginfo_delete, ZEND_ACC_PUBLIC)
    PHP_ME(Request, update, arginfo_update, ZEND_ACC_PUBLIC)
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

    REGISTER_LONG_CONSTANT("COUCH_DEL_DB", COUCH_DEL_DB, CONST_CS|CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("COUCH_DEL_DOC", COUCH_DEL_DOC, CONST_CS|CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("COUCH_UPDATE_SINGLE", COUCH_UPDATE_SINGLE, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("COUCH_UPDATE_MULTIPLE", COUCH_UPDATE_MULTIPLE, CONST_CS | CONST_PERSISTENT);

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
