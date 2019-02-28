/* couchdb_ext extension for PHP */

#ifndef PHP_COUCHDB_EXT_H
# define PHP_COUCHDB_EXT_H

extern zend_module_entry couchdb_ext_module_entry;
# define phpext_couchdb_ext_ptr &couchdb_ext_module_entry

# define PHP_COUCHDB_EXT_VERSION "0.1.0"

# if defined(ZTS) && defined(COMPILE_DL_COUCHDB_EXT)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

#endif	/* PHP_COUCHDB_EXT_H */
