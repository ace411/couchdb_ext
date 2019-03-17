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
        actions.method<&Actions::getDocsByKey>("getDocsByKey", {
            Php::ByVal("database", Php::Type::String, true),
            Php::ByVal("keys", Php::Type::Array, true)
        });
        actions.method<&Actions::getDoc>("getDoc", {
            Php::ByVal("database", Php::Type::String, true),
            Php::ByVal("docId", Php::Type::String, true),
            Php::ByVal("options", Php::Type::Array, true)
        });
        actions.method<&Actions::getDesignDocs>("getDesignDocs", {
            Php::ByVal("database", Php::Type::String, true),
            Php::ByVal("options", Php::Type::Array, true)
        });
        actions.method<&Actions::createDb>("createDb", {
            Php::ByVal("database", Php::Type::String, true)
        });
        actions.method<&Actions::isAvailable>("isAvailable");
        actions.method<&Actions::alldbs>("alldbs");
        
        extension.add(std::move(actions));

        return extension;
    }
}