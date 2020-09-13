/**
 * @file couch.h
 * @author Lochemem Bruno Michael (lochbm@gmail.com)
 * @brief header file containing extension's core CouchDb bindings
 * @version 0.1.0
 * 
 * @copyright Copyright (c) 1999-2019 The PHP Group
 * 
 */
#ifndef COUCH_H
#define COUCH_H

#include <iostream>
#include <string>
#include <thread>
#include <future>
#include <stdexcept>
#include <initializer_list>
#include <curl/curl.h>

#define HTTP_PUT 1
#define HTTP_GET 2
#define HTTP_POST 3
#define HTTP_DELETE 4

#define DEFAULT_PORT 5984
#define DEFAULT_TIMEOUT 60

namespace couch
{
  typedef std::initializer_list<std::string> strargs;

  /**
   * @brief joins string list elements with a string
   * 
   * @param glue 
   * @param args 
   * @return std::string 
   */
  auto concat(std::string glue, strargs args) -> std::string
  {
    std::string retval("");
    for (auto &idx : args)
      retval += idx + glue;

    if (!glue.empty())
      retval.pop_back();

    return retval;
  }

  /**
   * @brief replaces all occurrences of one string with another
   * 
   * @param search 
   * @param replace 
   * @param subject 
   * @return std::string 
   */
  auto replaceAll(std::string search,
                  std::string replace,
                  std::string subject) -> std::string
  {
    auto pos = subject.find(search);

    while (pos != std::string::npos)
    {
      subject.replace(pos, search.size(), replace);
      pos = subject.find(search, pos + replace.size());
    }

    return subject;
  }

  /**
   * @brief checks if a string exists inside another
   * 
   * @param needle 
   * @param haystack 
   * @return true 
   * @return false 
   */
  auto strCheck(std::string needle, std::string haystack) -> bool
  {
    std::string::size_type size = haystack.find(needle);

    return size == std::string::npos ? false : true;
  }

  /**
   * @brief performs CURL request
   * 
   * @param uri 
   * @param method 
   * @param data 
   * @param timeout 
   * @return std::string 
   */
  auto curlRequest(std::string uri,
                   long method,
                   std::string data,
                   long timeout) -> std::string
  {
    CURLcode code(CURLE_FAILED_INIT);
    CURL *curl = curl_easy_init();
    std::string result;
    curl_global_init(CURL_GLOBAL_ALL);

    if (curl)
    {
      typedef size_t (*CURL_WRITEFUNCTION_PTR)(char *, size_t, size_t, std::string *);
      curl_easy_setopt(curl,
                       CURLOPT_WRITEFUNCTION,
                       static_cast<CURL_WRITEFUNCTION_PTR>([](char *contents,
                                                              size_t size,
                                                              size_t nmemb,
                                                              std::string *data) -> size_t {
                         size_t newSize = size * nmemb;
                         if (data == NULL)
                           return 0;
                         data->append(contents, newSize);
                         return newSize;
                       }));
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
      curl_easy_setopt(curl, CURLOPT_URL, uri.c_str());
      curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
      curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);

      // set the certificate
      curl_easy_setopt(curl, CURLOPT_CAINFO, "cacert.pem");

      // set the Content-Type and Accept headers to application/json
      struct curl_slist *chunk = NULL;

      chunk = curl_slist_append(chunk, "Content-Type: application/json");
      chunk = curl_slist_append(chunk, "Accept: application/json");
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);

      // set the HTTP method
      switch (method)
      {
      case HTTP_PUT:
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
        break;

      case HTTP_POST:
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
        break;

      case HTTP_DELETE:
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
        break;

      default:
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
        break;
      }

      code = curl_easy_perform(curl);
      if (code != CURLE_OK)
        throw std::runtime_error(curl_easy_strerror(code));

      curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    result.shrink_to_fit();

    return result;
  }

  /**
   * @brief executes CURL request asynchronously - using C++'s future API
   * 
   * @param uri 
   * @param method 
   * @param data 
   * @param timeout 
   * @return std::string 
   */
  auto futureCurl(std::string uri,
                  long method,
                  std::string data,
                  long timeout) -> std::string
  {
    std::future<std::string> future = std::async(std::launch::async, [=]() {
      return curlRequest(uri, method, data, timeout);
    });

    future.wait();
    return future.get();
  }

  /**
   * @brief executes POST request
   * 
   * @param uri 
   * @param data 
   * @param timeout 
   * @return std::string 
   */
  auto post(std::string uri, std::string data, long timeout) -> std::string
  {
    return futureCurl(uri, HTTP_POST, data, timeout);
  }

  /**
   * @brief executes GET request
   * 
   * @param uri 
   * @param timeout 
   * @return std::string 
   */
  auto get(std::string uri, long timeout) -> std::string
  {
    return futureCurl(uri, HTTP_GET, "", timeout);
  }

  /**
   * @brief executes PUT request
   * 
   * @param uri 
   * @param data 
   * @param timeout 
   * @return std::string 
   */
  auto put(std::string uri, std::string data, long timeout) -> std::string
  {
    return futureCurl(uri, HTTP_PUT, data, timeout);
  }

  /**
   * @brief executes DELETE request
   * 
   * @param uri 
   * @param timeout 
   * @return std::string 
   */
  auto fdelete(std::string uri, long timeout) -> std::string
  {
    return futureCurl(uri, HTTP_DELETE, "", timeout);
  }

  class CouchDb
  {
  public:
    CouchDb(std::string host,
            std::string user,
            std::string pass,
            bool http = true,
            long port = DEFAULT_PORT,
            long timeout = DEFAULT_TIMEOUT) : host(host),
                                              user(user),
                                              pass(pass),
                                              port(port),
                                              http(http),
                                              timeout(timeout)
    {
      bool isLocal = host == "localhost" || host == "127.0.0.1";

      credentials = user + ":" + pass;
      baseUri = concat("", {(http || isLocal ? "http" : "https"),
                            "://",
                            credentials,
                            "@",
                            host,
                            (isLocal ? ":" + std::to_string(port) : "")});
    }

    /**
     * @brief checks if CouchDB service is available
     * 
     * @return true 
     * @return false 
     */
    bool isAvailable()
    {
      auto uri = concat("/", {baseUri, "_up"});
      auto result = get(uri, timeout);

      return strCheck("\"ok\"", result);
    }

    /**
     * @brief creates CouchDB database
     * 
     * @param database 
     * @return true 
     * @return false 
     */
    bool createDb(std::string database)
    {
      auto uri = concat("/", {baseUri, database});
      auto result = put(uri, "", timeout);

      return strCheck("\"ok\"", result);
    }

    /**
     * @brief deletes CouchDB database
     * 
     * @param database 
     * @return true 
     * @return false 
     */
    bool deleteDb(std::string database)
    {
      auto uri = concat("/", {baseUri, database});
      auto result = fdelete(uri, timeout);

      return strCheck("\"ok\"", result);
    }

    /**
     * @brief gets CouchDB database metadata
     * 
     * @param database 
     * @return std::string 
     */
    std::string dbInfo(std::string database)
    {
      auto uri = concat("/", {baseUri, database});
      auto result = get(uri, timeout);

      if (strCheck("\"error\"", result))
        throw std::runtime_error(errStr(result).c_str());

      return result;
    }

    /**
     * @brief checks if a database exists
     * 
     * @param database 
     * @return true 
     * @return false 
     */
    bool dbExists(std::string database)
    {
      auto uri = concat("/", {baseUri, database});
      auto result = get(uri, timeout);

      return strCheck("\"" + database + "\"", result);
    }

    /**
     * @brief generates an arbitrary number of UUIDs
     * 
     * @param count 
     * @return std::string 
     */
    std::string uuids(int count = 1)
    {
      auto uri = concat("?", {(baseUri + "/_uuids"),
                              ("count=" + std::to_string(count))});
      return get(uri, timeout);
    }

    /**
     * @brief returns all databases available
     * 
     * @return std::string 
     */
    std::string allDbs()
    {
      auto uri = concat("/", {baseUri, "_all_dbs"});

      return get(uri, timeout);
    }

    /**
     * @brief returns a specified document in a specified database
     * 
     * @param database 
     * @param docid 
     * @return std::string 
     */
    std::string getDoc(std::string database, std::string docid)
    {
      auto uri = concat("/", {baseUri, database, docid});

      return get(uri, timeout);
    }

    /**
     * @brief returns all documents in a database
     * 
     * @param database 
     * @param query 
     * @return std::string 
     */
    std::string allDocs(std::string database, std::string query)
    {
      auto uri = concat("", {concat("/", {baseUri, database, "_all_docs"}),
                             "?",
                             query});
      return get(uri, timeout);
    }

    /**
     * @brief inserts documents into database
     * 
     * @param database 
     * @param docs 
     * @return true 
     * @return false 
     */
    bool insertDocs(std::string database, std::string docs)
    {
      auto uri = concat("/", {baseUri, database, "_bulk_docs"});
      auto result = post(uri, docs, timeout);

      return strCheck("\"ok\"", result);
    }

    /**
     * @brief performs a Mango-query powered search
     * 
     * @param database 
     * @param query 
     * @return std::string 
     */
    std::string search(std::string database,
                       std::string query,
                       bool explain = false)
    {
      auto uri = concat("/", {baseUri, database, explain ? "_explain" : "_find"});
      auto result = post(uri, query, timeout);

      if (strCheck("\"error\"", result))
        throw std::runtime_error(errStr(result).c_str());

      return result;
    }

    /**
     * @brief inserts document into database
     * 
     * @param database 
     * @param contents 
     * @param batch 
     * @return true 
     * @return false 
     */
    bool insertDoc(std::string database,
                   std::string contents,
                   bool batch = false)
    {
      auto uri = concat("/", {baseUri, database});

      if (batch)
        uri.append("?batch=ok");

      auto result = post(uri, contents, timeout);

      return strCheck("\"ok\"", result);
    }

    /**
     * @brief updates a single database document
     * 
     * @param database 
     * @param docid 
     * @param rev 
     * @param contents 
     * @return true 
     * @return false 
     */
    bool updateDoc(std::string database,
                   std::string docid,
                   std::string rev,
                   std::string contents)
    {
      auto uri = concat("?", {concat("/", {baseUri, database, docid}),
                              concat("=", {"rev", rev})});
      auto result = put(uri, contents, timeout);

      return strCheck("\"ok\"", result);
    }

    /**
     * @brief deletes a single document
     * 
     * @param database 
     * @param docid 
     * @param rev 
     * @return true 
     * @return false 
     */
    bool deleteDoc(std::string database,
                   std::string docid,
                   std::string rev)
    {
      auto uri = concat("?", {concat("/", {baseUri, database, docid}),
                              concat("=", {"rev", rev})});
      auto result = fdelete(uri, timeout);

      return strCheck("\"ok\"", result);
    }

    /**
     * @brief creates a CouchDb design document
     * 
     * @param database 
     * @param ddoc 
     * @param contents 
     * @return true 
     * @return false 
     */
    bool createDdoc(std::string database,
                    std::string ddoc,
                    std::string contents)
    {
      auto uri = concat("/", {baseUri, database, "_design", ddoc});
      auto result = put(uri, contents, timeout);

      return strCheck("\"ok\"", result);
    }

    /**
     * @brief queries a CouchDb view
     * 
     * @param database 
     * @param ddoc 
     * @param view 
     * @param query 
     * @return std::string 
     */
    std::string queryView(std::string database,
                          std::string ddoc,
                          std::string view,
                          std::string query)
    {
      auto uri = concat("?", {concat("/", {baseUri,
                                           database,
                                           "_design",
                                           ddoc,
                                           "_view"}),
                              concat("?", {view, query})});
      auto result = get(uri, timeout);
      auto missing = strCheck("\"missing\"", result);

      if (strCheck("\"error\"", result) && !missing)
        throw std::runtime_error(errStr(result).c_str());

      return missing ? "{}" : result;
    }

    /**
     * @brief returns a list of changes made to documents in a database
     * 
     * @param database 
     * @param options 
     * @return std::string 
     */
    std::string changes(std::string database, std::string options)
    {
      auto uri = concat("?", {concat("/", {baseUri, database, "_changes"}),
                              options});
      auto result = get(uri, timeout);

      if (strCheck("\"error\"", result))
        throw std::runtime_error(errStr(result).c_str());

      return result;
    }

    /**
     * @brief configures a replication operation
     * 
     * @param params 
     * @return std::string 
     */
    std::string replicate(std::string params)
    {
      auto result = post(concat("/", {baseUri, "_replicate"}), params, timeout);

      if (strCheck("\"error\"", result))
        throw std::runtime_error(errStr(result).c_str());

      return result;
    }

    /**
     * @brief creates a CouchDb index
     * 
     * @param database 
     * @param options 
     * @return true 
     * @return false 
     */
    bool createIndex(std::string database, std::string options)
    {
      auto result = post(concat("/", {baseUri, database, "_index"}),
                         options,
                         timeout);

      return strCheck("\"created\"", result);
    }

  private:
    std::string host, user, pass, baseUri, credentials;
    long timeout, port;
    bool http;

    /**
     * @brief extracts descriptive CouchDB response error message 
     * 
     * @param result 
     * @return std::string 
     */
    std::string errStr(std::string result)
    {
      auto base = result.substr(result.find("\"reason\""), result.size() - 1);
      base = replaceAll("\"", "", base);
      base = replaceAll("}", "", base);
      base = replaceAll("]", "", base);

      auto err = base.substr(7, base.size() - 1);

      return err;
    }
  };
}; // namespace couch

#endif // COUCH_H
