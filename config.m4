dnl config.m4 for extension couchdb

PHP_ARG_ENABLE(couchdb_ext, whether to enable couchdb_ext support,
[  --enable-couchdb             Enable couchdb support], no)

PHP_ARG_WITH(curl, add curl library path,
[  --with-curl=[=DIR]           Ignore CURL path check])

if test "$PHP_COUCHDB" != "no"; then
  HEADER_INSTALL_DIRS="/usr/local /usr /usr/local/opt /usr/bin"
  PTHREAD_INCLUDE_DIR="include"
  PTHREAD_CPP_HEADER="pthread.h"
  
  dnl check if pthread is installed
  PHP_CHECK_LIBRARY(pthread, pthread_create,
  [
    AC_DEFINE(HAVE_PTHREAD, 1, [ ])
  ], [
    AC_MSG_ERROR(Something is wrong. Please check your pthread installation)
  ], [-lpthread])

  dnl check if CURL is installed
  PHP_CHECK_LIBRARY(curl, curl_easy_perform,
  [
    AC_DEFINE(HAVE_CURL, 1, [ ])
  ], [
    AC_MSG_ERROR(Something is wrong. Please check your curl installation)
  ], [-lcurl])

  dnl add C/C++ flags
  CFLAGS="-lpthread -lcurl"
  CXXFLAGS="-std=c++11 -lpthread -lcurl"
  PHP_REQUIRE_CXX()
  PHP_SUBST(COUCHDB_SHARED_LIBADD)
  
  dnl add C++11 standard library
  PHP_ADD_LIBRARY(stdc++, 1, COUCHDB_SHARED_LIBADD)
  
  dnl specify extension dependencies
  PHP_ADD_EXTENSION_DEP(couchdb, json)
  AC_DEFINE(HAVE_COUCHDB, 1, [ Have couchdb support ])
  
  PHP_NEW_EXTENSION(couchdb, php_couch.cpp, $ext_shared)
fi
