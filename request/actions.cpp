#include "actions.h"
#include <tuple>

const static std::string _uuids = "_uuids?count=";
const static std::string _alldbs = "_all_dbs";
const static std::string _alldocs = "_all_docs";
const static std::tuple<std::string, std::string> scheme("http://", "https://"); 

std::string baseUriGen(
    const std::string &host,
    const std::string &user,
    const std::string &pwd,
    long port = 5984)
{
    std::string baseUri;
    baseUri += (host == "localhost") ? std::get<0>(scheme) + host + ":" + std::to_string(port) : 
        std::get<1>(scheme) + user + ":" + pwd + "@" + host;
    baseUri += "/";
    return baseUri;
}

Actions::Actions() : 
host("localhost"),
user(""),
pwd(""),
port(5984),
timeout(60)
{
    baseUri = baseUriGen(host, user, pwd, port);
}

Actions::Actions(
    const std::string &host,
    const std::string &user,
    const std::string &pwd,
    long port, 
    long timeout) : 
host(host),
user(user),
pwd(pwd),
port(port),
timeout(timeout) 
{
    baseUri = baseUriGen(host, user, pwd, port);
}

std::string Actions::request(int opt, const std::string &path, const std::string &data = "") const
{
    HttpRequest request(baseUri, user, pwd, timeout);
    std::string result = "";
    switch (opt) {
        case GET_REQUEST:
            result += request.getResult(path);
            break;
        case PUT_REQUEST:
            result += request.putResult(path, data);
            break;
        case POST_REQUEST:
            result += request.postResult(path, data);
            break;
    }
    return result;
} 

Php::Value Actions::uuids(Php::Parameters &params) const
{
    int idCount = params[0];
    Php::Value _result = request(GET_REQUEST, (_uuids + std::to_string(idCount)));

    return _result;
}

Php::Value Actions::alldbs() const
{
    Php::Value _result = request(GET_REQUEST, _alldbs);

    return _result;
}

Php::Value Actions::allDocs(Php::Parameters &params) const
{
    std::string database = params[0];
    Php::Value result = request(GET_REQUEST, (database + "/" + _alldocs));

    return result;    
}

Php::Value Actions::getDocsByKey(Php::Parameters &params) const
{
    std::string database = params[0];
    std::string postData = Php::call("json_encode", params[1]);
    Php::Value result = request(POST_REQUEST, (database + "/" + _alldocs), postData);

    return result;
}
