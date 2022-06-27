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

ZEND_BEGIN_ARG_INFO_EX(arginfo_connect, 0, 0, 1)
ZEND_ARG_ARRAY_INFO(0, config, 0)
ZEND_END_ARG_INFO();

/* functions with no arguments but with a definite return type */
ZEND_BEGIN_ARG_INFO_EX(arginfo_null, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_all_docs, 0, 0, 2)
ZEND_ARG_TYPE_INFO(0, database, IS_STRING, 0)
ZEND_ARG_ARRAY_INFO(0, keys, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_get_doc, 0, 0, 2)
ZEND_ARG_TYPE_INFO(0, database, IS_STRING, 0)
ZEND_ARG_TYPE_INFO(0, documentId, IS_STRING, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_create_db, 0, 0, 1)
ZEND_ARG_TYPE_INFO(0, database, IS_STRING, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_find, 0, 0, 2)
ZEND_ARG_TYPE_INFO(0, database, IS_STRING, 0)
ZEND_ARG_TYPE_INFO(0, query, IS_ARRAY, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_uuids, 0, 0, 1)
ZEND_ARG_TYPE_INFO(0, count, IS_LONG, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_create_index, 0, 0, 2)
ZEND_ARG_TYPE_INFO(0, database, IS_STRING, 0)
ZEND_ARG_TYPE_INFO(0, options, IS_ARRAY, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_transaction, 0, 0, 1)
ZEND_ARG_TYPE_INFO(0, actions, IS_ARRAY, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_create_docs, 0, 0, 2)
ZEND_ARG_TYPE_INFO(0, database, IS_STRING, 0)
ZEND_ARG_TYPE_INFO(0, documents, IS_ARRAY, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_query_view, 0, 0, 4)
ZEND_ARG_TYPE_INFO(0, database, IS_STRING, 0)
ZEND_ARG_TYPE_INFO(0, designDocument, IS_STRING, 0)
ZEND_ARG_TYPE_INFO(0, view, IS_STRING, 0)
ZEND_ARG_ARRAY_INFO(0, options, 0)
ZEND_END_ARG_INFO();
