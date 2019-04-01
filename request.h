#ifndef REQUEST_H
#define REQUEST_H
#include <string>

class Request 
{
    private:
        std::string host, user, pwd, baseUri, credentials;
        long port;
        long timeout = 60;

    public:
        Request(const std::string &host, 
            const std::string &user, 
            const std::string &pwd, 
            long port, 
            long timeout);
        std::string uuids(long count) const;
        std::string allDbs() const;
        bool isAvailable() const;
};

#endif // REQUEST_H