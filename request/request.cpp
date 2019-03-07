#include "request.h"

std::string credGen(const std::string &user, const std::string &pwd)
{
    std::string cred = "";
    if (user.empty() || pwd.empty()) {
        return "";
    }
    cred += user + ":" + pwd;
    return cred;
}

HttpRequest::HttpRequest() : 
uri("http://localhost:5984"),
user(""),
pwd(""),
timeout(60)
{
    curl_global_init(CURL_GLOBAL_ALL);
}

HttpRequest::HttpRequest(
    const std::string &uri, 
    const std::string &user,
    const std::string &pwd,
    long timeout) :
uri(uri),
user(user),
pwd(pwd),
timeout(timeout)
{
    curl_global_init(CURL_GLOBAL_ALL);
}

HttpRequest::~HttpRequest()
{
    curl_global_cleanup();
}

std::string HttpRequest::getResult(const std::string &path)
{
    return curlRequest(uri + path, GET_REQUEST, "", credGen(user, pwd), timeout);
}

std::string HttpRequest::postResult(const std::string &path, const std::string &postData)
{
    return curlRequest(uri + path, POST_REQUEST, postData, credGen(user, pwd), timeout);
}

std::string HttpRequest::putResult(const std::string &path, const std::string &postData)
{
    return curlRequest(uri + path, PUT_REQUEST, postData, credGen(user, pwd), timeout);
}
