dnl config.m4 for couchdb

PHP_ARG_ENABLE(couchdb, whether to enable couchdb support,
[  --enable-couchdb          Enable couchdb support], no)

PHP_ARG_WITH(curl, add curl library path,
[  --with-curl=[=DIR]        Ignore CURL path check])

if test "$PHP_COUCHDB" != "no"; then
  ASYNCH_LOCATIONS="/usr/local/include /usr/include /usr/local/opt/include"
  ASYNCH_HEADER="async.h"
  ASYNCH_SEM_HEADER="async-sem.h"
  HAVE_ASYNCH=0
  
  dnl check if libcurl is installed
  PKG_CHECK_MODULES([CURL], [libcurl >= 7.29.0])
  PKG_CHECK_VAR([CURL_FEATURES], [libcurl], [supported_features])

  PHP_EVAL_LIBLINE($CURL_LIBS, CURL_SHARED_LIBADD)
  PHP_EVAL_INCLINE($CURL_CFLAGS)
  CURL_SSL=0

  AC_MSG_CHECKING([for SSL support in libcurl])
  case "$CURL_FEATURES" in
    *SSL*)
      CURL_SSL=1
      AC_MSG_RESULT([yes])
      ;;
    *)
      AC_MSG_RESULT([no])
      ;;
  esac

  if test $CURL_SSL == "1"; then
    sava_LDFLAGS="$LDFLAGS"
    LDFLAGS="$LDFLAGS $CURL_LIBS"
  else
    AC_MSG_ERROR([Please install libcurl with SSL support])
  fi

  PHP_CHECK_LIBRARY(curl, curl_easy_perform,
  [
    AC_DEFINE(HAVE_CURL, 1, [ ])
  ], [
    AC_MSG_ERROR([Please check your libcurl installation])
  ], [
    $CURL_LIBS
  ])

  dnl check if async.h is installed
  AC_MSG_CHECKING([for async.h])
  for iter in $ASYNCH_LOCATIONS; do
    if test -s $iter/$ASYNCH_HEADER; then
      if test -s $iter/$ASYNCH_SEM_HEADER; then
        AC_MSG_RESULT([yes])
        HAVE_ASYNCH=1
      else
        AC_MSG_RESULT([no])
      fi
    fi
  done

  if test $HAVE_ASYNCH == "0"; then
    AC_MSG_NOTICE([Consider installing async.h for a better experience])
  fi

  PHP_ADD_EXTENSION_DEP(couchdb, json)

  AC_DEFINE(HAVE_COUCHDB, 1, [ Have couchdb support ])

  PHP_NEW_EXTENSION(couchdb, couchdb.c, $ext_shared)
fi
