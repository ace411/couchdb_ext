/**
 * @file php_couch.cpp
 * @author Lochemem Bruno Michael (lochbm@gmail.com)
 * @brief couchdb_ext module core file
 * @version 0.1.0
 * 
 * @copyright Copyright (c) 1999-2019 The PHP Group
 * 
 */
#include "couch.cpp"

/* {{{ proto CouchDb::__construct( array options ) 
 */
PHP_METHOD(CouchDb, __construct)
{
  constructor(INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto bool CouchDb::isAvailable()
 */
PHP_METHOD(CouchDb, isAvailable)
{
  isAvailable(INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto bool CouchDb::createDb( string database )
 */
PHP_METHOD(CouchDb, createDb)
{
  dbActions(INTERNAL_FUNCTION_PARAM_PASSTHRU, DB_ACTION_CREATE);
}
/* }}} */

/* {{{ proto bool CouchDb::deleteDb( string database )
 */
PHP_METHOD(CouchDb, deleteDb)
{
  dbActions(INTERNAL_FUNCTION_PARAM_PASSTHRU, DB_ACTION_DELETE);
}
/* }}} */

/* {{{ proto bool CouchDb::dbExists( string database )
 */
PHP_METHOD(CouchDb, dbExists)
{
  dbActions(INTERNAL_FUNCTION_PARAM_PASSTHRU, DB_ACTION_CHECK);
}
/* }}} */

/* {{{ proto string|array CouchDb::dbExists( string database )
 */
PHP_METHOD(CouchDb, dbInfo)
{
  dbActions(INTERNAL_FUNCTION_PARAM_PASSTHRU, DB_ACTION_INFO);
}
/* }}} */

/* {{{ proto string CouchDb::uuids( int count )
 */
PHP_METHOD(CouchDb, uuids)
{
  uuids(INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto string|array CouchDb::allDbs()
 */
PHP_METHOD(CouchDb, allDbs)
{
  allDbs(INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto string|array CouchDb::allDocs( string database [, array query ] )
 */
PHP_METHOD(CouchDb, allDocs)
{
  allDocs(INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto bool CouchDb::insertDocs( string database [, array docs ] )
 */
PHP_METHOD(CouchDb, insertDocs)
{
  insertDocs(INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto bool CouchDb::deleteDocs( string database [, array docs ] )
 */
PHP_METHOD(CouchDb, deleteDocs)
{
  insertDocs(INTERNAL_FUNCTION_PARAM_PASSTHRU, true);
}
/* }}} */

/* {{{ proto bool CouchDb::updateDocs( string database [, array docs ] )
 */
PHP_METHOD(CouchDb, updateDocs)
{
  insertDocs(INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto string|array CouchDb::search( string database [, array query ] )
 */
PHP_METHOD(CouchDb, search)
{
  query(INTERNAL_FUNCTION_PARAM_PASSTHRU, DB_QUERY_SEARCH);
}
/* }}} */

/* {{{ proto string|array CouchDb::explain( string database [, array query ] )
 */
PHP_METHOD(CouchDb, explain)
{
  query(INTERNAL_FUNCTION_PARAM_PASSTHRU, DB_QUERY_EXPLAIN);
}
/* }}} */

/* {{{ proto bool CouchDb::createIndex( string database [, array options ] )
 */
PHP_METHOD(CouchDb, createIndex)
{
  query(INTERNAL_FUNCTION_PARAM_PASSTHRU, DB_QUERY_CREATE_INDEX);
}
/* }}} */

/* {{{ proto string|array CouchDb::getDoc( string database [, string docid ] )
 */
PHP_METHOD(CouchDb, getDoc)
{
  getDoc(INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto bool CouchDb::insertDoc( string database [, array contents [, bool batch = false ]] )
 */
PHP_METHOD(CouchDb, insertDoc)
{
  insertDoc(INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto bool CouchDb::updateDoc( string database [, string docId [, string rev [, array contents ]]] )
 */
PHP_METHOD(CouchDb, updateDoc)
{
  updateDoc(INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto bool CouchDb::deleteDoc( string database [, string docId [, string rev ]] )
 */
PHP_METHOD(CouchDb, deleteDoc)
{
  deleteDoc(INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto bool CouchDb::createDdoc( string database [, string ddoc [, array contents ]] )
 */
PHP_METHOD(CouchDb, createDdoc)
{
  createDdoc(INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto string|array CouchDb::queryView( string database [, string ddoc [, string view [, array query ]]] )
 */
PHP_METHOD(CouchDb, queryView)
{
  queryView(INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto string|array CouchDb::replicate( array options )
 */
PHP_METHOD(CouchDb, replicate)
{
  replicate(INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto string|array CouchDb::changes( string database [, array options ] )
 */
PHP_METHOD(CouchDb, changes)
{
  changes(INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

ZEND_BEGIN_ARG_INFO_EX(arginfo_construct, 0, 0, 1)
ZEND_ARG_ARRAY_INFO(0, options, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_dbaction, 0, 0, 1)
ZEND_ARG_INFO(0, database)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_count, 0, 0, 1)
ZEND_ARG_INFO(0, count)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_alldocs, 0, 0, 2)
ZEND_ARG_INFO(0, database)
ZEND_ARG_ARRAY_INFO(0, query, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_insertdocs, 0, 0, 2)
ZEND_ARG_INFO(0, database)
ZEND_ARG_ARRAY_INFO(0, docs, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_updateDoc, 0, 0, 4)
ZEND_ARG_INFO(0, database)
ZEND_ARG_INFO(0, docId)
ZEND_ARG_INFO(0, rev)
ZEND_ARG_ARRAY_INFO(0, contents, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_insertDoc, 0, 0, 3)
ZEND_ARG_INFO(0, database)
ZEND_ARG_ARRAY_INFO(0, contents, 0)
ZEND_ARG_INFO(0, batch)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_deleteDoc, 0, 0, 3)
ZEND_ARG_INFO(0, database)
ZEND_ARG_INFO(0, docId)
ZEND_ARG_INFO(0, rev)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_createddoc, 0, 0, 3)
ZEND_ARG_INFO(0, database)
ZEND_ARG_INFO(0, ddoc)
ZEND_ARG_ARRAY_INFO(0, contents, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_getdoc, 0, 0, 2)
ZEND_ARG_INFO(0, database)
ZEND_ARG_INFO(0, docId)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_queryview, 0, 0, 4)
ZEND_ARG_INFO(0, database)
ZEND_ARG_INFO(0, ddoc)
ZEND_ARG_INFO(0, view)
ZEND_ARG_ARRAY_INFO(0, query, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_changes, 0, 0, 2)
ZEND_ARG_INFO(0, database)
ZEND_ARG_ARRAY_INFO(0, options, 0)
ZEND_END_ARG_INFO();

static const zend_function_entry couchdb_methods[] = {
    PHP_ME(CouchDb, __construct, arginfo_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
        PHP_ME(CouchDb, isAvailable, NULL, ZEND_ACC_PUBLIC)
            PHP_ME(CouchDb, createDb, arginfo_dbaction, ZEND_ACC_PUBLIC)
                PHP_ME(CouchDb, deleteDb, arginfo_dbaction, ZEND_ACC_PUBLIC)
                    PHP_ME(CouchDb, dbExists, arginfo_dbaction, ZEND_ACC_PUBLIC)
                        PHP_ME(CouchDb, dbInfo, arginfo_dbaction, ZEND_ACC_PUBLIC)
                            PHP_ME(CouchDb, uuids, arginfo_count, ZEND_ACC_PUBLIC)
                                PHP_ME(CouchDb, allDbs, NULL, ZEND_ACC_PUBLIC)
                                    PHP_ME(CouchDb, allDocs, arginfo_alldocs, ZEND_ACC_PUBLIC)
                                        PHP_ME(CouchDb, insertDocs, arginfo_insertdocs, ZEND_ACC_PUBLIC)
                                            PHP_ME(CouchDb, updateDocs, arginfo_insertdocs, ZEND_ACC_PUBLIC)
                                                PHP_ME(CouchDb, deleteDocs, arginfo_insertdocs, ZEND_ACC_PUBLIC)
                                                    PHP_ME(CouchDb, search, arginfo_alldocs, ZEND_ACC_PUBLIC)
                                                        PHP_ME(CouchDb, explain, arginfo_alldocs, ZEND_ACC_PUBLIC)
                                                            PHP_ME(CouchDb, createIndex, arginfo_changes, ZEND_ACC_PUBLIC)
                                                                PHP_ME(CouchDb, getDoc, arginfo_getdoc, ZEND_ACC_PUBLIC)
                                                                    PHP_ME(CouchDb, updateDoc, arginfo_updateDoc, ZEND_ACC_PUBLIC)
                                                                        PHP_ME(CouchDb, insertDoc, arginfo_insertDoc, ZEND_ACC_PUBLIC)
                                                                            PHP_ME(CouchDb, deleteDoc, arginfo_deleteDoc, ZEND_ACC_PUBLIC)
                                                                                PHP_ME(CouchDb, createDdoc, arginfo_createddoc, ZEND_ACC_PUBLIC)
                                                                                    PHP_ME(CouchDb, queryView, arginfo_queryview, ZEND_ACC_PUBLIC)
                                                                                        PHP_ME(CouchDb, changes, arginfo_changes, ZEND_ACC_PUBLIC)
                                                                                            PHP_ME(CouchDb, replicate, arginfo_construct, ZEND_ACC_PUBLIC)
                                                                                                PHP_FE_END};

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(couchdb)
{
  php_info_print_table_start();
  php_info_print_table_header(2, "couchdb support", "enabled");
  php_info_print_table_header(2, "couchdb version", PHP_COUCHDB_EXT_EXTVER);
  php_info_print_table_header(2, "couchdb author", "Lochemem Bruno Michael <lochbm@gmail.com>");
  php_info_print_table_end();
}
/* }}} */

/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(couchdb)
{
#if defined(ZTS) && defined(COMPILE_DL_COUCHDB)
  ZEND_TSRMLS_CACHE_UPDATE();
#endif

  return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(couchdb)
{
  zend_class_entry ce, exception_ce;
  INIT_CLASS_ENTRY(ce, "CouchDb", couchdb_methods);
  INIT_CLASS_ENTRY(exception_ce, "CouchDbException", NULL);

  couchdb_ce = zend_register_internal_class(&ce TSRMLS_CC);
  couchdb_ce->create_object = couchdb_object_new;

  memcpy(&couchdb_object_handlers,
         zend_get_std_object_handlers(),
         sizeof(couchdb_object_handlers));

  couchdb_object_handlers.free_obj = couchdb_object_free;
  couchdb_object_handlers.dtor_obj = couchdb_object_destroy;

  couchdb_object_handlers.offset = XtOffsetOf(couchdb_object, std);

#ifdef HAVE_SPL
  couchdb_exception_ce = zend_register_internal_class_ex(
      &exception_ce, spl_ce_RuntimeException);
#else
  couchdb_exception_ce = zend_register_internal_class_ex(
      &exception_ce, zend_exception_get_default(TSRMLS_C));
#endif

  return SUCCESS;
}
/* }}} */

zend_module_entry couchdb_module_entry = {
    STANDARD_MODULE_HEADER,
    PHP_COUCHDB_EXT_EXTNAME,
    NULL,
    PHP_MINIT(couchdb),            // MINIT
    NULL,                          // MSHUTDOWN
    PHP_RINIT(couchdb),            // RINIT
    NULL,                          // RSHUTDOWN
    PHP_MINFO(couchdb),            // MINFO
#if ZEND_MODULE_API_NO >= 20170718 // check if PHP version is greater than 7.2
    PHP_COUCHDB_EXT_EXTVER,
#endif
    STANDARD_MODULE_PROPERTIES};

#ifdef COMPILE_DL_COUCHDB
extern "C"
{
  ZEND_GET_MODULE(couchdb)
}
#endif
