#include "request.h"

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

std::string HttpRequest::credentials() const
{
    auto credentials = [](const std::string &user, const std::string &pwd) -> std::string {
        std::string cred = "";
        if (user.empty() || pwd.empty())
        {
            return "";
        }
        cred += user + ":" + pwd;
        return cred;
    };
    return credentials(user, pwd);
}

std::string HttpRequest::getResult(const std::string &path) const
{ 
    return curlRequest<std::string, int, long>(uri + path, GET_REQUEST, "", credentials(), timeout);
}

std::string HttpRequest::postResult(const std::string &path, const std::string &postData) const
{
    return curlRequest<std::string, int, long>(uri + path, POST_REQUEST, postData, credentials(), timeout);
}

std::string HttpRequest::putResult(const std::string &path, const std::string &postData) const
{
    return curlRequest<std::string, int, long>(uri + path, PUT_REQUEST, postData, credentials(), timeout);
}
