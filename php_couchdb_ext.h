/**
 * @file php_couchdb_ext.h
 * @author Lochemem Bruno Michael (lochbm@gmail.com)
 * @brief 
 * @version 0.1.0
 */
#ifndef PHP_COUCHDB_EXT_H
#define PHP_COUCHDB_EXT_H

#define PHP_COUCHDB_EXT_EXTNAME "couchdb_ext"
#define PHP_COUCHDB_EXT_EXTVER "0.1.0"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

extern "C"
{
#include <php.h>
#include <curl/curl.h>
#include <zend_smart_str.h>
#include <ext/json/php_json.h>
#include <zend_exceptions.h>
#include <ext/standard/php_http.h>
#include <ext/standard/php_array.h>
#include <ext/standard/info.h>
}

extern zend_module_entry couchdb_ext_module_entry;
#define phpext_couchdb_ext_ptr &couchdb_ext_module_entry

#endif /* PHP_COUCHDB_EXT_H */
