#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <curl/curl.h>
#define COUCH_GETOPT 1
#define COUCH_POSTOPT 2
#define COUCH_PUTOPT 3

struct RetString {
    char *ptr;
    size_t len;
};

void initString(struct RetString *s)
{
    s->len = 0;
    s->ptr = malloc(s->len + 1);

    if (s->ptr == NULL) {
        exit(EXIT_FAILURE);
    }

    s->ptr[0] = '\0';
}

size_t printFunc(void *contents, size_t size, size_t nmemb, struct RetString *s)
{
    size_t newLen = s->len + size * nmemb;
    s->ptr = realloc(s->ptr, newLen + 1);
    memcpy(s->ptr + s->len, contents, size * nmemb);
    s->ptr[newLen] = '\0';
    s->len = newLen;

    return size * nmemb;
}

char *strConcat(int count, ...)
{
    va_list args;
    size_t len = 0;

    va_start(args, count);
    for (int i = 0; i < count; i++)
    {
        const char *str = va_arg(args, char *);
        len += strlen(str);
    }
    va_end(args);

    char *ret = malloc(len + 1);

    char *dst = ret;
    va_start(args, count);
    for (int i = 0; i < count; i++)
    {
        const char *src = va_arg(args, char *);
        while (*dst++ = *src++)
            ;
        dst--;
    }
    va_end(args);
    return ret;
}

void couchHeader(CURL *curl)
{
    struct curl_slist *chunk = NULL;

    chunk = curl_slist_append(chunk, "Content-Type: application/json");
    chunk = curl_slist_append(chunk, "Accept: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
}

void couchMethod(CURL *curl, int method, void *curlData)
{
    switch (method) {
        case COUCH_POSTOPT:
            curl_easy_setopt(curl, CURLOPT_POST, 1L);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, curlData);
            break;
        case COUCH_PUTOPT:
            curl_easy_setopt(curl, CURLOPT_PUT, 1L);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, curlData);
            break;
        case COUCH_GETOPT:
            curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
            break;
    }
}

struct RetString couchRequest(char *url, int method, void *curlData)
{
    CURL *curl;
    CURLcode resCode;
    struct RetString result;
    initString(&result);

    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();

    if (curl) {
        couchMethod(curl, method, curlData);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, printFunc);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
        couchHeader(curl);
        resCode = curl_easy_perform(curl);

        if (resCode != CURLE_OK)
            fprintf(stderr, "An error occured: %s\n", curl_easy_strerror(resCode));

        //printf("Result is %s\n", result.ptr);
        //free(result.ptr);
        curl_easy_cleanup(curl);
    }
    return result;
}

void freeResult(char *s)
{
    free(s);
}
