PHP_ARG_ENABLE(couchdb_ext, whether to enable couchdb_ext support,
[  --enable-couchdb_ext          Enable couchdb_ext support], no)

PHP_ARG_WITH(pthread, add pthread library path,
[  --with-pthread=[=DIR]        Ignore pthread path check])

PHP_ARG_WITH([curl], [add CURL support], 
[AS_HELP_STRING([--with-curl], [Ignore CURL path check])])

if test "$PHP_COUCHDB_EXT" != "no"; then
    HEADER_INSTALL_LIBS="/usr/local /usr /usr/local/opt"
    PTHREAD_INCLUDE_DIR="include"
    PTHREAD_CPP_HEADER="pthread.h"

    dnl check if pthread exists
    AC_MSG_CHECKING([for pthread])
    for iter in $HEADER_INSTALL_LIBS; do
        if test -s $iter/$PTHREAD_INCLUDE_DIR/$PTHREAD_CPP_HEADER; then
            AC_MSG_RESULT(found $PTHREAD_CPP_HEADER)
            PTHREAD_CPP_LIB=$iter/$PTHREAD_INCLUDE_DIR/$PTHREAD_CPP_HEADER
            PTHREAD_CPP_DIR=$iter
        fi
    done

    if test -z "$PTHREAD_CPP_LIB"; then
        AC_MSG_RESULT(pthread is not installed)
        AC_MSG_ERROR(check your pthread installation)
    fi

    dnl check if curl is installed
    AC_MSG_CHECKING([for libcurl])
    PKG_CHECK_MODULES([CURL], [libcurl >= 7.15.5])
    PKG_CHECK_VAR([CURL_FEATURES], [libcurl], [supported_features])

    PHP_EVAL_INCLINE($CURL_CFLAGS)
    PHP_EVAL_LIBLINE($CURL_LIBS, COUCHDB_EXT_SHARED_LIBADD)

    PHP_CHECK_LIBRARY(curl, curl_easy_perform,
    [
        AC_DEFINE(HAVE_CURL, 1, [ ])
    ], [
        AC_MSG_ERROR(Please check your curl installation)
    ], [
        $CURL_LIBS
    ])

    PHP_ADD_INCLUDE($PTHREAD_CPP_DIR/include)
    AC_CHECK_HEADERS([pthread.h])

    PHP_CHECK_LIBRARY(pthread, pthread_create,
    [
        PHP_ADD_INCLUDE($PTHREAD_CPP_DIR/include)
        PHP_ADD_LIBRARY_WITH_PATH(pthread, $PTHREAD_CPP_DIR/lib, COUCHDB_EXT_SHARED_LIBADD)
    ], [
        AC_MSG_ERROR(Please check your pthread installation)
    ], [
        -L$PTHREAD_CPP_DIR -lpthread
    ])
    
    CXXFLAGS="-std=c++11 -lpthread -lcurl"
    PHP_REQUIRE_CXX()
    PHP_SUBST(COUCHDB_EXT_SHARED_LIBADD)
    PHP_ADD_LIBRARY(stdc++, 1, COUCHDB_EXT_SHARED_LIBADD)
    PHP_ADD_EXTENSION_DEP(couchdb_ext, json)
    PHP_NEW_EXTENSION(couchdb_ext, couch.cpp request.cpp, $ext_shared)
fi
