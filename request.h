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
        std::string allDocs(const std::string &database, const std::string &params) const;
        std::string search(const std::string &database, const std::string &query) const;
        std::string queryView(const std::string &database, 
            const std::string &ddoc, 
            const std::string &view, 
            const std::string &params) const;
        bool createDdoc(const std::string &database, const std::string &ddoc, const std::string &docData) const;
        bool insertDocs(const std::string &database, const std::string &data) const;
        bool createDb(const std::string &database) const;
        bool isAvailable() const;
};

#endif // REQUEST_H