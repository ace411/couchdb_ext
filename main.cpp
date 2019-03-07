#include <iostream>
#include <string>
#include "request/actions.cpp"

using namespace std;

extern "C" {
    PHPCPP_EXPORT void *get_module()
    {
        static Php::Extension extension("couchdb_ext", "1.0");

        Php::Class<Actions> actions("CouchDb");

        actions.method<&Actions::uuids>("uuids", {
            Php::ByVal("count", Php::Type::Numeric, true)
        });
        actions.method<&Actions::allDocs>("alldocs", {
            Php::ByVal("database", Php::Type::String, true)
        });
        actions.method<&Actions::alldbs>("alldbs");
        
        extension.add(std::move(actions));

        return extension;
    }
}