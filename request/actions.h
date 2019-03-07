#ifndef __ACTIONS_H__
#define __ACTIONS_H__

#include <string>
#include "request.cpp"

class Actions: public Php::Base {
    private:
        std::string baseUri;
        std::string host;
        std::string user;
        std::string pwd;
        long port = 5984;
        long timeout = 60;
    public:
        Actions();
        Actions(
            const std::string &host,
            const std::string &user,
            const std::string &pwd,
            long port,
            long timeout);
        virtual ~Actions() {};
        std::string request(int opt, const std::string &path, const std::string &data) const;
        Php::Value uuids(Php::Parameters &params) const;
        Php::Value alldbs() const;
        Php::Value allDocs(Php::Parameters &params) const;
        Php::Value getDocsByKey(Php::Parameters &params) const;
        //Php::Value getDoc(Php::Parameters &params) const;
};

#endif //end __ACTIONS_H__