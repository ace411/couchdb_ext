/* couchdb_ext extension for PHP */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "couch.c"
#include "php_couchdb_ext.h"
#define BASE_COUCH_URI "http://localhost:5984"
#define UUIDS "/uuids?count="
#define ALLDBS "/_all_dbs"

/* For compatibility with older PHP versions */
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE() \
	ZEND_PARSE_PARAMETERS_START(0, 0) \
	ZEND_PARSE_PARAMETERS_END()
#endif

PHP_FUNCTION(couchdb_connect)
{
	char *baseUri = BASE_COUCH_URI;
	size_t uriLen = sizeof(baseUri) - 1;
	zend_string *ret;
	struct RetString result;

	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_STRING(baseUri, uriLen)
	ZEND_PARSE_PARAMETERS_END();

	result = couchRequest(baseUri, COUCH_GETOPT, "");
	ret = zend_string_init(result.ptr, result.len, 1);
	freeResult(result.ptr);
	zend_string_release(ret);

	RETURN_STR(ret);
}

PHP_FUNCTION(couchdb_uuids)
{
	char *count = "1";
	size_t countLen = sizeof(count) - 1;
	zend_string *ret;
	struct RetString result;

	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		ZEND_PARAM_STRING(count, countLen)
	ZEND_PARSE_PARAMETERS_END();

	for (int i = 0; i < countLen; i++) {
		if (!isdigit(count[i]))
			php_error_docref(NULL, E_WARNING, "Non-numeric value encountered");
	}

	result = couchRequest(strConcat(3, BASE_COUCH_URI, UUIDS, count), COUCH_GETOPT, "");
	ret = zend_string_init(result.ptr, result.len, 1);
	freeResult(result.ptr);
	zend_string_release(ret);

	RETURN_STR(ret);
}

PHP_FUNCTION(couchdb_alldbs)
{
	zend_string *ret;
	struct RetString result;

	ZEND_PARSE_PARAMETERS_NONE();

	result = couchRequest(strConcat(2, BASE_COUCH_URI, "/_all_dbs"), COUCH_GETOPT, "");
	ret = zend_string_init(result.ptr, result.len, 1);
	freeResult(result);
	zend_string_release(ret);

	RETURN_STR(ret);
}

/* {{{ void couchdb_ext_test1()
 */
PHP_FUNCTION(couchdb_ext_test1)
{
	ZEND_PARSE_PARAMETERS_NONE();

	php_printf("The extension %s is loaded and working!\r\n", "couchdb_ext");
}
/* }}} */

/* {{{ string couchdb_ext_test2( [ string $var ] )
 */
PHP_FUNCTION(couchdb_ext_test2)
{
	char *var = "World";
	size_t var_len = sizeof("World") - 1;
	zend_string *retval;

	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_STRING(var, var_len)
	ZEND_PARSE_PARAMETERS_END();

	retval = strpprintf(0, "Hello %s", var);

	RETURN_STR(retval);
}
/* }}}*/

/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(couchdb_ext)
{
#if defined(ZTS) && defined(COMPILE_DL_COUCHDB_EXT)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(couchdb_ext)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "couchdb_ext support", "enabled");
	php_info_print_table_end();
}
/* }}} */

/* {{{ arginfo
 */
ZEND_BEGIN_ARG_INFO(arginfo_couchdb_ext_test1, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_couchdb_ext_test2, 0)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_couchdb_connect, 0)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_couchdb_uuids, 0)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_couchdb_alldbs, 0)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ couchdb_ext_functions[]
 */
static const zend_function_entry couchdb_ext_functions[] = {
	PHP_FE(couchdb_ext_test1,		arginfo_couchdb_ext_test1)
	PHP_FE(couchdb_ext_test2,		arginfo_couchdb_ext_test2)
	PHP_FE(couchdb_connect,			arginfo_couchdb_connect)
	PHP_FE(couchdb_uuids,			arginfo_couchdb_uuids)
	PHP_FE(couchdb_alldbs,			arginfo_couchdb_alldbs)
	PHP_FE_END
};
/* }}} */

/* {{{ couchdb_ext_module_entry
 */
zend_module_entry couchdb_ext_module_entry = {
	STANDARD_MODULE_HEADER,
	"couchdb_ext",					/* Extension name */
	couchdb_ext_functions,			/* zend_function_entry */
	NULL,							/* PHP_MINIT - Module initialization */
	NULL,							/* PHP_MSHUTDOWN - Module shutdown */
	PHP_RINIT(couchdb_ext),			/* PHP_RINIT - Request initialization */
	NULL,							/* PHP_RSHUTDOWN - Request shutdown */
	PHP_MINFO(couchdb_ext),			/* PHP_MINFO - Module info */
	PHP_COUCHDB_EXT_VERSION,		/* Version */
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_COUCHDB_EXT
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(couchdb_ext)
#endif
