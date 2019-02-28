dnl config.m4 for extension couchdb_ext

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(couchdb_ext, for couchdb_ext support,
dnl Make sure that the comment is aligned:
dnl [  --with-couchdb_ext             Include couchdb_ext support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(couchdb_ext, whether to enable couchdb_ext support,
dnl Make sure that the comment is aligned:
[  --enable-couchdb_ext          Enable couchdb_ext support], no)

if test "$PHP_COUCHDB_EXT" != "no"; then
  dnl Write more examples of tests here...

  dnl # get library FOO build options from pkg-config output
  dnl AC_PATH_PROG(PKG_CONFIG, pkg-config, no)
  dnl AC_MSG_CHECKING(for libfoo)
  dnl if test -x "$PKG_CONFIG" && $PKG_CONFIG --exists foo; then
  dnl   if $PKG_CONFIG foo --atleast-version 1.2.3; then
  dnl     LIBFOO_CFLAGS=\`$PKG_CONFIG foo --cflags\`
  dnl     LIBFOO_LIBDIR=\`$PKG_CONFIG foo --libs\`
  dnl     LIBFOO_VERSON=\`$PKG_CONFIG foo --modversion\`
  dnl     AC_MSG_RESULT(from pkgconfig: version $LIBFOO_VERSON)
  dnl   else
  dnl     AC_MSG_ERROR(system libfoo is too old: version 1.2.3 required)
  dnl   fi
  dnl else
  dnl   AC_MSG_ERROR(pkg-config not found)
  dnl fi
  dnl PHP_EVAL_LIBLINE($LIBFOO_LIBDIR, COUCHDB_EXT_SHARED_LIBADD)
  dnl PHP_EVAL_INCLINE($LIBFOO_CFLAGS)

  dnl # --with-couchdb_ext -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/couchdb_ext.h"  # you most likely want to change this
  dnl if test -r $PHP_COUCHDB_EXT/$SEARCH_FOR; then # path given as parameter
  dnl   COUCHDB_EXT_DIR=$PHP_COUCHDB_EXT
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for couchdb_ext files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       COUCHDB_EXT_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$COUCHDB_EXT_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the couchdb_ext distribution])
  dnl fi

  dnl # --with-couchdb_ext -> add include path
  dnl PHP_ADD_INCLUDE($COUCHDB_EXT_DIR/include)

  dnl # --with-couchdb_ext -> check for lib and symbol presence
  dnl LIBNAME=COUCHDB_EXT # you may want to change this
  dnl LIBSYMBOL=COUCHDB_EXT # you most likely want to change this

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $COUCHDB_EXT_DIR/$PHP_LIBDIR, COUCHDB_EXT_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_COUCHDB_EXTLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong couchdb_ext lib version or lib not found])
  dnl ],[
  dnl   -L$COUCHDB_EXT_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(COUCHDB_EXT_SHARED_LIBADD)

  dnl # In case of no dependencies
  AC_DEFINE(HAVE_COUCHDB_EXT, 1, [ Have couchdb_ext support ])

  PHP_NEW_EXTENSION(couchdb_ext, couchdb_ext.c, $ext_shared)
fi
