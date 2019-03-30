#include "php_couchdb_ext.h"
#include "request.h"

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
        intern->request = new Request((const char *)host, (const char *)user, (const char *)pwd, port, timeout);
}

PHP_METHOD(Request, uuids)
{
    long idCount;
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

static const zend_function_entry request_methods[] = {
    PHP_ME(Request, __construct, arginfo_constructor, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(Request, uuids, arginfo_uuids, ZEND_ACC_PUBLIC)
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