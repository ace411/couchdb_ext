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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_couchdb.h"
#include "src/php/couch.c"
#include "src/couch/couch.c"
#include "couchdb_arginfo.c"

/* {{{ proto CouchDb CouchDb::connect( array config ) */
PHP_METHOD(CouchDb, connect)
{
	couchdb_connect(INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto array CouchDb::transaction( array transactions ) */
PHP_METHOD(CouchDb, transaction)
{
	couchdb_transaction(INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto string|array CouchDb::documents( string database [, array keys ] ) */
PHP_METHOD(CouchDb, documents)
{
	couchdb_all_docs(INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto string|array Couchdb::databases()  */
PHP_METHOD(CouchDb, databases)
{
	couchdb_all_dbs(INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto string|array CouchDb::uuids( int count ) */
PHP_METHOD(CouchDb, uuids)
{
	couchdb_uuids(INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto string|array CouchDb::find( string database [, array query ] )  */
PHP_METHOD(CouchDb, find)
{
	couchdb_find(INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto string|array CouchDb::document( string database [, string documentId ] )  */
PHP_METHOD(CouchDb, document)
{
	couchdb_get_doc(INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto bool CouchDb::createIndex( string database [, array options ] ) */
PHP_METHOD(CouchDb, createIndex)
{
	couchdb_create_index(INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto bool CouchDb::createDatabase( string database )  */
PHP_METHOD(CouchDb, createDatabase)
{
	couchdb_database_op(INTERNAL_FUNCTION_PARAM_PASSTHRU, HTTP_PUT, 0);
}
/* }}} */

/* {{{ proto bool CouchDb::available() */
PHP_METHOD(CouchDb, available)
{
	couchdb_is_available(INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto bool CouchDb::insertDocuments( string database [, array documents ] ) */
PHP_METHOD(CouchDb, insertDocuments)
{
	couchdb_create_docs(INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto bool CouchDb::deleteDocuments( string database [, array documents ] ) */
PHP_METHOD(CouchDb, deleteDocuments)
{
	couchdb_delete_docs(INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto bool CouchDb::updateDocuments( string database [, array documents ] ) */
PHP_METHOD(CouchDb, updateDocuments)
{
	couchdb_update_docs(INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto CouchDb CouchDb::session() */
PHP_METHOD(CouchDb, session)
{
	couchdb_session_init(INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto string|array CouchDb::database( string database ) */
PHP_METHOD(CouchDb, database)
{
	couchdb_database_op(INTERNAL_FUNCTION_PARAM_PASSTHRU, HTTP_GET, 1);
}
/* }}} */

/* {{{ proto bool CouchDb::deleteDatabase( string database ) */
PHP_METHOD(CouchDb, deleteDatabase)
{
	couchdb_database_op(INTERNAL_FUNCTION_PARAM_PASSTHRU, HTTP_DELETE, 0);
}
/* }}} */

/* {{{ proto string|array CouchDb::changes( string database [, array options ] ) */
PHP_METHOD(CouchDb, changes)
{
	couchdb_changes(INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto string|array CouchDb::view( string database [, string designDocument [, string view [, array options ]]] ) */
PHP_METHOD(CouchDb, view)
{
	couchdb_query_view(INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

static const zend_function_entry ext_functions[] = {
		PHP_ME(CouchDb, connect, arginfo_connect, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
				PHP_ME(CouchDb, available, arginfo_null, ZEND_ACC_PUBLIC)
						PHP_ME(CouchDb, databases, arginfo_null, ZEND_ACC_PUBLIC)
								PHP_ME(CouchDb, documents, arginfo_all_docs, ZEND_ACC_PUBLIC)
										PHP_ME(CouchDb, uuids, arginfo_uuids, ZEND_ACC_PUBLIC)
												PHP_ME(CouchDb, find, arginfo_find, ZEND_ACC_PUBLIC)
														PHP_ME(CouchDb, document, arginfo_get_doc, ZEND_ACC_PUBLIC)
																PHP_ME(CouchDb, createIndex, arginfo_create_index, ZEND_ACC_PUBLIC)
																		PHP_ME(CouchDb, createDatabase, arginfo_create_db, ZEND_ACC_PUBLIC)
																				PHP_ME(CouchDb, deleteDatabase, arginfo_create_db, ZEND_ACC_PUBLIC)
																						PHP_ME(CouchDb, database, arginfo_create_db, ZEND_ACC_PUBLIC)
																								PHP_ME(CouchDb, insertDocuments, arginfo_create_docs, ZEND_ACC_PUBLIC)
																										PHP_ME(CouchDb, deleteDocuments, arginfo_create_docs, ZEND_ACC_PUBLIC)
																												PHP_ME(CouchDb, updateDocuments, arginfo_create_docs, ZEND_ACC_PUBLIC)
																														PHP_ME(CouchDb, session, arginfo_null, ZEND_ACC_PUBLIC)
																																PHP_ME(CouchDb, changes, arginfo_create_index, ZEND_ACC_PUBLIC)
																																		PHP_ME(CouchDb, view, arginfo_query_view, ZEND_ACC_PUBLIC)
																																				PHP_ME(CouchDb, transaction, arginfo_transaction, ZEND_ACC_PUBLIC)
																																						PHP_FE_END};

PHP_MINIT_FUNCTION(couchdb)
{
	zend_class_entry couchdb_ce;
	zend_class_entry couchdb_exception_ce;
	INIT_CLASS_ENTRY(couchdb_ce, "CouchDb", ext_functions);
	INIT_CLASS_ENTRY(couchdb_exception_ce, "CouchdbException", NULL);

	couch_ce = zend_register_internal_class(&couchdb_ce);
	couch_ce->create_object = php_couchdb_connection_create_object;

	memcpy(&couchdb_object_handlers, zend_get_std_object_handlers(), sizeof(couchdb_object_handlers));
	couchdb_object_handlers.free_obj = php_couchdb_connection_free_object;
	couchdb_object_handlers.dtor_obj = php_couchdb_connection_destroy_object;
	couchdb_object_handlers.get_properties = php_couchdb_get_properties;

	zend_declare_class_constant_long(couch_ce, "RETURN_ARRAY", sizeof("RETURN_ARRAY") - 1, 1);
	zend_declare_class_constant_long(couch_ce, "RETURN_JSON", sizeof("RETURN_JSON") - 1, 0);

#ifdef HAVE_SPL
	couch_exception_ce = zend_register_internal_class_ex(&couchdb_exception_ce, spl_ce_RuntimeException);
#else
	couch_exception_ce = zend_register_internal_class_ex(&couchdb_exception_ce, zend_exception_get_default());
#endif

	return SUCCESS;
}

/* {{{ PHP_RINIT_FUNCTION */
PHP_RINIT_FUNCTION(couchdb)
{
#if defined(ZTS) && defined(COMPILE_DL_COUCHDB)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION */
PHP_MINFO_FUNCTION(couchdb)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "couchdb support", "enabled");
	php_info_print_table_header(2, "couchdb version", PHP_COUCHDB_VERSION);
	php_info_print_table_header(2, "couchdb async transaction support",
#ifdef ASYNC_H
															"enabled"
#else
															"disabled"
#endif
	);
	php_info_print_table_end();
}
/* }}} */

/* {{{ couchdb_module_entry */
zend_module_entry couchdb_module_entry = {
		STANDARD_MODULE_HEADER,
		"couchdb",					 /* Extension name */
		ext_functions,			 /* zend_function_entry */
		PHP_MINIT(couchdb),	 /* PHP_MINIT - Module initialization */
		NULL,								 /* PHP_MSHUTDOWN - Module shutdown */
		PHP_RINIT(couchdb),	 /* PHP_RINIT - Request initialization */
		NULL,								 /* PHP_RSHUTDOWN - Request shutdown */
		PHP_MINFO(couchdb),	 /* PHP_MINFO - Module info */
		PHP_COUCHDB_VERSION, /* Version */
		STANDARD_MODULE_PROPERTIES};
/* }}} */

#ifdef COMPILE_DL_COUCHDB
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(couchdb)
#endif
