#include "request.h"
#include <ext/spl/spl_exceptions.h>
#include <curl/curl.h>
#include <initializer_list>
#include <tuple>
#define GET_OPT 1
#define PUT_OPT 2
#define POST_OPT 3

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

template<typename S, typename L>
auto curlRequest(S &url, L method, L timeout) -> std::string
{
    CURLcode resCode(CURLE_FAILED_INIT);
    CURL *curl = curl_easy_init();
    std::string result;
    curl_global_init(CURL_GLOBAL_ALL);

    if (curl) 
    {
        curl_easy_setopt(
            curl, 
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
        appHeaders<CURL *>(curl);

        resCode = curl_easy_perform(curl);
        if (resCode != CURLE_OK)
        {
            switch (resCode) {
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
                default:
                    zend_throw_error(NULL, "An error occurred");
                    break;
            }
        }
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    return result;
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
}

std::string Request::uuids(long count) const
{
    std::string reqUri = concat<std::string, StrArgs>("?", {(baseUri + "/_uuids"), ("count=" + std::to_string(count))});
    return curlRequest<const std::string, long>(reqUri, 1, timeout);    
}

bool Request::isAvailable() const
{
    std::string reqUri = concat<std::string, StrArgs>("/", {baseUri, "_up"});
    std::string retval = curlRequest<const std::string, long>(reqUri, 1, timeout);
    
    return checkStrExists<const std::string>("\"ok\"", retval);
}
