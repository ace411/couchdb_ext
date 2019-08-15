#include "request.h"
#include <ext/spl/spl_exceptions.h>
#include <curl/curl.h>
#include <initializer_list>
#include <future>
#include <thread>
#include <tuple>
#define GET_OPT 1
#define PUT_OPT 2
#define POST_OPT 3
#define DEL_OPT 4

typedef size_t(*CURL_WRITEFUNCTION_PTR)(char *, size_t, size_t, std::string *);
typedef std::initializer_list<std::string> StrArgs;
typedef std::tuple<std::string, std::string> StrTuple;

const static StrTuple scheme("http://", "https://");

template<typename S, typename L>
auto concat(S glue, L words) -> std::string
{
    std::string retval = "";
    for (auto &iter: words)
    {
        retval += iter + glue; 
    }
    if (!glue.empty())
        retval.pop_back();
    return retval;
}

template <typename S>
auto checkStrExists(S &match, S &str) -> bool
{
    std::string::size_type sType;
    sType = str.find(match);
    return sType != std::string::npos ? true : false;
}

template<typename C>
void appHeaders(C curl)
{
    struct curl_slist *chunk = NULL;

    chunk = curl_slist_append(chunk, "Content-Type: application/json");
    chunk = curl_slist_append(chunk, "Accept: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
}

template<typename C, typename S>
void appAuth(C curl, S &credentials, S &url)
{
    if (checkStrExists<const std::string>("localhost", url)) 
    {
        curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
        curl_easy_setopt(curl, CURLOPT_USERPWD, credentials.c_str());
    }
}

template<typename C, typename L, typename S>
void appMethod(C curl, L method, S &data)
{
    switch (method)
    {
        case PUT_OPT:
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
            break;

        case DEL_OPT:
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
            break;

        case POST_OPT:
            curl_easy_setopt(curl, CURLOPT_POST, 1L);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
            break;

        default:
            curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
            break;
    }
}

template<typename C, typename S>
void appCert(C curl, S &url)
{
    if (!checkStrExists<const std::string>("localhost", url)) 
        curl_easy_setopt(curl, CURLOPT_CAINFO, "cacert.pem");
}

template<typename T>
void phpCurlError(T code)
{
    switch (code)
    {
        case CURLE_FAILED_INIT:
            zend_throw_error(NULL, "An error occurred at INIT time");
            break;
        case CURLE_URL_MALFORMAT:
            zend_throw_error(NULL, "Malformed URL");
            break;
        case CURLE_COULDNT_CONNECT:
            zend_throw_error(NULL, "Could not connect");
            break;
        case CURLE_REMOTE_ACCESS_DENIED:
            zend_throw_error(NULL, "Access denied");
            break;
        case CURLE_OPERATION_TIMEDOUT:
            zend_throw_error(NULL, "Operation timed out");
            break;
        case CURLE_GOT_NOTHING:
            zend_throw_error(NULL, "Nothing was returned from the server");
            break;
        case CURLE_PEER_FAILED_VERIFICATION:
            zend_throw_error(NULL, "SSL certificate failed verification");
            break;
        case CURLE_SEND_ERROR:
            zend_throw_error(NULL, "Failed sending network data");
            break;
        case CURLE_SSL_CERTPROBLEM:
            zend_throw_error(NULL, "SSL certificate error");
            break;
        case CURLE_SSL_CACERT_BADFILE:
            zend_throw_error(NULL, "SSL file error");
            break;
        default:
            zend_throw_error(NULL, "An error occurred");
            break;
    }
}

template<typename S, typename L>
auto curlRequest(S &url, L method, S &credentials, S &data, L timeout) -> std::string
{
    CURLcode resCode(CURLE_FAILED_INIT);
    CURL *curl = curl_easy_init();
    std::string result;
    curl_global_init(CURL_GLOBAL_ALL);

    if (curl) 
    {
        curl_easy_setopt(curl, 
            CURLOPT_WRITEFUNCTION, 
            static_cast<CURL_WRITEFUNCTION_PTR>([](char * contents, size_t size, size_t nmemb, std::string * data) -> size_t {
                size_t newSize = size * nmemb;
                if (data == NULL)
                    return 0;
                data->append(contents, newSize);
                return newSize;
            }));
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
        appHeaders<CURL *>(curl);
        appAuth<CURL *, const std::string>(curl, credentials, url);
        appMethod<CURL *, long, const std::string>(curl, method, data);
        appCert<CURL *, const std::string>(curl, url);

        resCode = curl_easy_perform(curl);

        if (resCode != CURLE_OK)
            phpCurlError<CURLcode>(resCode);
        
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    return result;
}

template<typename S, typename L>
auto futureCurl(S &url, L method, S &credentials, S &data, L timeout) -> std::string
{
    std::future<std::string> future = std::async(std::launch::async, [=]{
        return curlRequest<const std::string, long>(url, method, credentials, data, timeout); });

    future.wait();
    return future.get();
}

template<typename S, typename L>
auto getRequest(S &url, S &credentials, L timeout) -> std::string
{
    return futureCurl<const std::string, long>(url, GET_OPT, credentials, "", timeout);
}

template<typename S, typename L>
auto putRequest(S &url, S &credentials, S &data, L timeout) -> std::string
{
    return futureCurl<const std::string, long>(url, PUT_OPT, credentials, data, timeout);
}

template<typename S, typename L>
auto delRequest(S &url, S &credentials, L timeout) -> std::string
{
    return futureCurl<const std::string, long>(url, DEL_OPT, credentials, "", timeout);
}

template<typename S, typename L>
auto postRequest(S &url, S &credentials, S &data, L timeout) -> std::string
{
    return futureCurl<const std::string, long>(url, POST_OPT, credentials, data, timeout);
}

template<typename S, typename L>
auto uriGen(S host, S user, S pwd, L port) -> std::string
{
    std::string retval = "";
    retval += (host == "localhost") ? 
        concat<std::string, StrArgs>("", {std::get<0>(scheme), 
            concat<std::string, StrArgs>(":", {host, std::to_string(port)})}) :
        concat<std::string, StrArgs>("", {std::get<1>(scheme),
            concat<std::string, StrArgs>("@", {(user + ":" + pwd), host})});
    return retval;
}

template<typename S>
auto remAmpersand(S str) -> std::string
{
    const char &last = str.back();
    if (last == '&')
        str.pop_back();

    return str;
}

Request::Request(const std::string &host, 
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
    baseUri = uriGen<std::string, long>(host, user, pwd, port);
    credentials = concat<std::string, StrArgs>(":", {user, pwd});
}

std::string Request::uuids(long count) const
{
    std::string reqUri = concat<std::string, StrArgs>("?", {(baseUri + "/_uuids"), ("count=" + std::to_string(count))});
    
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
    std::string reqUri = concat<std::string, StrArgs>("/", {baseUri, "_all_dbs"});

    return getRequest<const std::string, long>(reqUri, credentials, timeout);
}

std::string Request::allDocs(const std::string &database, const std::string &params) const
{
    std::string reqUri = concat<std::string, StrArgs>("", {(baseUri + "/" + database + "/"), 
        concat<std::string, StrArgs>("?", {"_all_docs", remAmpersand<std::string>(params)})});

    return getRequest<const std::string, long>(reqUri, credentials, timeout);
}

bool Request::insertDocs(const std::string &database, const std::string &data) const
{
    std::string reqUri = concat<std::string, StrArgs>("/", {baseUri, database, "_bulk_docs"});
    std::string result = postRequest<const std::string, long>(reqUri, credentials, data, timeout);

    return checkStrExists<const std::string>("\"ok\"", result);
}

std::string Request::search(const std::string &database, const std::string &query) const
{
    std::string reqUri = concat<std::string, StrArgs>("/", {baseUri, database, "_find"});

    return postRequest<const std::string, long>(reqUri, credentials, query, timeout);
}

bool Request::createDdoc(const std::string &database, 
    const std::string &ddoc, 
    const std::string &docData) const
{
    std::string reqUri = concat<std::string, StrArgs>("/", {baseUri, database, "_design", ddoc});
    std::string result = putRequest<const std::string, long>(reqUri, credentials, docData, timeout);
    
    return checkStrExists<const std::string>("\"ok\"", result);
}

std::string Request::queryView(const std::string &database, 
    const std::string &ddoc,
    const std::string &view,
    const std::string &params) const
{
    std::string reqUri = concat<std::string, StrArgs>("/", {baseUri, 
        database, 
        "_design", 
        ddoc, 
        "_view", 
        concat<std::string, StrArgs>("?", {view, remAmpersand<std::string>(params)})});

    return getRequest<const std::string, long>(reqUri, credentials, timeout);
}

bool Request::createDb(const std::string &database) const
{
    std::string reqUri = concat<std::string, StrArgs>("/", {baseUri, database});
    std::string result = putRequest<const std::string, long>(reqUri, credentials, "", timeout);

    return checkStrExists<const std::string>("\"ok\"", result);
}

bool Request::deleteOpt(const std::string &endpoint) const
{
    std::string reqUri = concat<std::string, StrArgs>("/", {baseUri, endpoint});
    std::string result = delRequest<const std::string, long>(reqUri, credentials, timeout);

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