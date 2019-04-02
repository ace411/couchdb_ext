#ifndef PHP_COUCHDB_EXT_H
#define PHP_COUCHDB_EXT_H

#define PHP_COUCHDB_EXT_EXTNAME "couchdb_ext"
#define PHP_COUCHDB_EXT_EXTVER  "1.0"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

extern "C"
{
    #include "php.h"
    #include <curl/curl.h>
}

extern zend_module_entry couchdb_ext_module_entry;
#define phpext_couchdb_ext_ptr &couchdb_ext_module_entry

#endif /* PHP_COUCHDB_EXT_H */