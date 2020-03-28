/**
 * @file request.h
 * @author Lochemem Bruno Michael (lochbm@gmail.com)
 * @brief 
 * @version 0.1
 */
#ifndef REQUEST_H
#define REQUEST_H
#include <string>

class Request
{
private:
    std::string host, user, pwd, baseUri, credentials;
    long port, timeout = 60;

public:
    /**
     * @brief Construct a new Request object
     * 
     * @param host 
     * @param user 
     * @param pwd 
     * @param port 
     * @param timeout 
     */
    Request(const std::string &host,
            const std::string &user,
            const std::string &pwd,
            long port,
            long timeout);
    /**
     * @brief 
     * 
     * @param count 
     * @return std::string 
     */
    std::string uuids(long count) const;
    /**
     * @brief Show all databases
     * 
     * @return std::string 
     */
    std::string allDbs() const;
    /**
     * @brief Show all documents in a database
     * 
     * @param database 
     * @param params 
     * @return std::string 
     */
    std::string allDocs(const std::string &database, const std::string &params) const;
    /**
     * @brief Search database
     * 
     * @param database 
     * @param query 
     * @return std::string 
     */
    std::string search(const std::string &database, const std::string &query) const;
    /**
     * @brief queries a CouchDB view
     * 
     * @param database 
     * @param ddoc 
     * @param view 
     * @param params 
     * @return std::string 
     */
    std::string queryView(const std::string &database,
                          const std::string &ddoc,
                          const std::string &view,
                          const std::string &params) const;
    /**
     * @brief Create a Ddoc object
     * 
     * @param database 
     * @param ddoc 
     * @param docData 
     * @return true 
     * @return false 
     */
    bool createDdoc(const std::string &database, const std::string &ddoc, const std::string &docData) const;
    /**
     * @brief 
     * 
     * @param database 
     * @param data 
     * @return true 
     * @return false 
     */
    bool insertDocs(const std::string &database, const std::string &data) const;
    /**
     * @brief Create a Db object
     * 
     * @param database 
     * @return true 
     * @return false 
     */
    bool createDb(const std::string &database) const;
    /**
     * @brief
     * 
     * @param endpoint 
     * @return true 
     * @return false 
     */
    bool deleteOpt(const std::string &endpoint) const;
    /**
     * @brief Updates a single CouchDB document
     * 
     * @param database 
     * @param idkey 
     * @param revkey 
     * @param data 
     * @return true 
     * @return false 
     */
    bool updateSingle(const std::string &database,
                      const std::string &idkey,
                      const std::string &revkey,
                      const std::string &data) const;
    /**
     * @brief Checks if an instance of CouchDB is running
     * 
     * @return true 
     * @return false 
     */
    bool isAvailable() const;
};

#endif // REQUEST_H
