#ifndef __REQUEST_H__
#define __REQUEST_H__

#include "curl.cpp"

class HttpRequest
{
    private:
        std::string uri, user, pwd;
        long timeout;

    public:
        HttpRequest(
            const std::string &uri,
            const std::string &user,
            const std::string &pwd,
            long timeout);
        HttpRequest();
        ~HttpRequest();
        std::string credentials() const;
        std::string getResult(const std::string &path) const;
        std::string postResult(const std::string &path, const std::string &postData) const;
        std::string putResult(const std::string &path, const std::string &postData) const;
};

#endif //end __REQUEST_H__