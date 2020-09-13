/**
 * @file php_couchdb_ext.h
 * @author Lochemem Bruno Michael (lochbm@gmail.com)
 * @brief header file containing essential module bindings and C-file preprocessor inclusions
 * @version 0.1.0
 * 
 * @copyright Copyright (c) 1999-2019 The PHP Group
 * 
 */
#ifndef PHP_COUCHDB_H
#define PHP_COUCHDB_H

#define PHP_COUCHDB_EXT_EXTNAME "couchdb"
#define PHP_COUCHDB_EXT_EXTVER "0.1.0"

#define DB_ACTION_INFO 1
#define DB_ACTION_CHECK 2
#define DB_ACTION_CREATE 3
#define DB_ACTION_DELETE 4

#define DB_QUERY_SEARCH 5
#define DB_QUERY_EXPLAIN 6
#define DB_QUERY_CREATE_INDEX 7

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

extern "C"
{
#include "php.h"
#include "zend_smart_str.h"
#include "zend_exceptions.h"
#include "ext/json/php_json.h"
#include "ext/standard/info.h"
#include "ext/standard/php_http.h"
#include "ext/standard/php_array.h"
}

extern zend_module_entry couchdb_module_entry;
#define phpext_couchdb_ext_ptr &couchdb_module_entry

#endif /* PHP_COUCHDB_H */
