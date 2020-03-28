/**
 * @file request.cpp
 * @author Lochemem Bruno Michael (lochbm@gmail.com)
 * @brief 
 * @version 0.1
 */
#include "request.h"
#include "http.cpp"

using namespace std;

Request::Request(const std::string &host,
                 const std::string &user,
                 const std::string &pwd,
                 long port,
                 long timeout) : host(host),
                                 user(user),
                                 pwd(pwd),
                                 port(port),
                                 timeout(timeout)
{
    baseUri = uriGen<std::string, long>(host, user, pwd, port);
    credentials = concat<std::string, StrArgs>(":", {user, pwd});
}

std::string Request::uuids(long count) const
{
    std::string reqUri = concat<std::string, StrArgs>("?", {(baseUri + "/_uuids"),
                                                            ("count=" + std::to_string(count))});

    return getRequest<const std::string, long>(reqUri, credentials, timeout);
}

bool Request::isAvailable() const
{
    std::string reqUri = concat<std::string, StrArgs>("/", {baseUri, "_up"});
    std::string retval = getRequest<const std::string, long>(reqUri, credentials, timeout);

    return checkStrExists<const std::string>("\"ok\"", retval);
}

std::string Request::allDbs() const
{
    auto reqUri = concat<std::string, StrArgs>("/", {baseUri, "_all_dbs"});

    return getRequest<const std::string, long>(reqUri, credentials, timeout);
}

std::string Request::allDocs(const std::string &database, const std::string &params) const
{
    auto reqUri = concat<std::string, StrArgs>("", {(baseUri + "/" + database + "/"),
                                                    concat<std::string, StrArgs>("?", {"_all_docs",
                                                                                       remAmpersand<std::string>(params)})});

    return getRequest<const std::string, long>(reqUri, credentials, timeout);
}

bool Request::insertDocs(const std::string &database, const std::string &data) const
{
    auto reqUri = concat<std::string, StrArgs>("/", {baseUri, database, "_bulk_docs"});
    auto result = postRequest<const std::string, long>(reqUri, credentials, data, timeout);

    return checkStrExists<const std::string>("\"ok\"", result);
}

std::string Request::search(const std::string &database, const std::string &query) const
{
    auto reqUri = concat<std::string, StrArgs>("/", {baseUri, database, "_find"});

    return postRequest<const std::string, long>(reqUri, credentials, query, timeout);
}

bool Request::createDdoc(const std::string &database,
                         const std::string &ddoc,
                         const std::string &docData) const
{
    auto reqUri = concat<std::string, StrArgs>("/", {baseUri, database, "_design", ddoc});
    auto result = putRequest<const std::string, long>(reqUri, credentials, docData, timeout);

    return checkStrExists<const std::string>("\"ok\"", result);
}

std::string Request::queryView(const std::string &database,
                               const std::string &ddoc,
                               const std::string &view,
                               const std::string &params) const
{
    auto reqUri = concat<std::string, StrArgs>("/", {baseUri,
                                                     database,
                                                     "_design",
                                                     ddoc,
                                                     "_view",
                                                     concat<std::string, StrArgs>("?", {view,
                                                                                        remAmpersand<std::string>(params)})});

    return getRequest<const std::string, long>(reqUri, credentials, timeout);
}

bool Request::createDb(const std::string &database) const
{
    auto reqUri = concat<std::string, StrArgs>("/", {baseUri, database});
    auto result = putRequest<const std::string, long>(reqUri, credentials, "", timeout);

    return checkStrExists<const std::string>("\"ok\"", result);
}

bool Request::deleteOpt(const std::string &endpoint) const
{
    auto reqUri = concat<std::string, StrArgs>("/", {baseUri, endpoint});
    auto result = delRequest<const std::string, long>(reqUri, credentials, timeout);

    return checkStrExists<const std::string>("\"ok\"", result);
}

bool Request::updateSingle(const std::string &database,
                           const std::string &idkey,
                           const std::string &revkey,
                           const std::string &data) const
{
    auto reqUri = concat<std::string, StrArgs>("?", {concat<std::string, StrArgs>("/", {baseUri, database, idkey}),
                                                     concat<std::string, StrArgs>("=", {"rev", revkey})});
    auto result = putRequest<const std::string, long>(reqUri, credentials, data, timeout);

    return checkStrExists<const std::string>("\"ok\"", result);
}
