PHP_ARG_ENABLE(couchdb_ext, whether to enable couchdb_ext support,
dnl Make sure that the comment is aligned:
[  --enable-couchdb_ext          Enable couchdb_ext support], no)

if test "$PHP_COUCHDB_EXT" != "no"; then
    CXXFLAGS="-std=c++11 -lpthread -lcurl"
    PHP_REQUIRE_CXX()
    PHP_SUBST(COUCHDB_EXT_SHARED_LIBADD)
    PHP_ADD_LIBRARY(stdc++, 1, COUCHDB_EXT_SHARED_LIBADD)
    PHP_ADD_EXTENSION_DEP(couchdb_ext, json)
    PHP_NEW_EXTENSION(couchdb_ext, couch.cpp request.cpp, $ext_shared)
fi
