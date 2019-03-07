#include <curl/curl.h>
#include <string>
#include <iostream>
#include <phpcpp.h>
#define GET_REQUEST 1
#define POST_REQUEST 2
#define PUT_REQUEST 3

const static std::string errMsg = "Request terminated with an error of ";

static size_t printFn(char *content, size_t size, size_t nmemb, std::string *data)
{
    if (data == NULL)
        return 0;
    
    data->append(content, size * nmemb);

    return size * nmemb;
}

void curlHeader(CURL *curl)
{
    struct curl_slist *chunk = NULL;

    chunk = curl_slist_append(chunk, "Content-Type: application/json");
    chunk = curl_slist_append(chunk, "Accept: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
}

void verbOpt(CURL *curl, int method = 1, const std::string &postData = "")
{
    switch (method) {
        case GET_REQUEST:
            curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
            break;
        case POST_REQUEST:
            curl_easy_setopt(curl, CURLOPT_POST, 1L);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());
            break;
        case PUT_REQUEST:
            curl_easy_setopt(curl, CURLOPT_PUT, 1L);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());
            break;
    }
}

void authOpt(CURL *curl, const std::string &credentials = "")
{
    if (!credentials.empty()) 
    { 
        curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
        curl_easy_setopt(curl, CURLOPT_USERPWD, credentials.c_str());
    }    
}

std::string curlRequest(
    const std::string &uri, 
    int method = 1, 
    const std::string &postData = "", 
    const std::string &credentials = "", 
    long timeout = 60)
{
    CURLcode resCode;
    CURL *curl = curl_easy_init();
    std::string result;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, printFn);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
        curl_easy_setopt(curl, CURLOPT_CAINFO, "cacert.pem");
        curl_easy_setopt(curl, CURLOPT_URL, uri.c_str()); //convert URI to C string
        curlHeader(curl);
        authOpt(curl, credentials);
        verbOpt(curl, method);

        resCode = curl_easy_perform(curl);

        if (CURLE_OK != resCode) {
            Php::Exception(errMsg + curl_easy_strerror(resCode));
        }

        curl_easy_cleanup(curl);
    }

    return result;
}
