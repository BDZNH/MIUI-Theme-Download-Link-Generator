#include "HttpRequest.h"
#include <stdio.h>
struct memory {
    char* response;
    size_t size;
};

static size_t cb(void* data, size_t datasize, size_t nmemb, void* userp)
{
    size_t realsize = datasize * nmemb;
    struct memory* mem = (struct memory*)userp;

    char* ptr = (char*)realloc(mem->response, mem->size + realsize + 1);
    if (ptr == NULL)
        return 0;  /* out of memory! */

    mem->response = ptr;
    memcpy(&(mem->response[mem->size]), data, realsize);
    mem->size += realsize;
    mem->response[mem->size] = 0;

    return realsize;
}
bool httpRequestIme(const std::string& url, std::string& resp)
{
    CURL* curl;
    CURLcode res = CURLE_OK;
    curl = curl_easy_init();
    if (curl)
    {
        struct memory chunk = { 0 };
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&chunk);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            resp = curl_easy_strerror(res);
        }
        else
        {
            resp = std::string(chunk.response, chunk.size);
            free(chunk.response);
        }
        curl_easy_cleanup(curl);
        return res == CURLE_OK;
    }
    return false;
}


size_t write_data(void* ptr, size_t size, size_t nmemb, FILE* stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

static int download_progress_callback(void* p,curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)
{
    DownloadCallBack* cb = (DownloadCallBack*)p;
    cb->onProgress(dltotal, dlnow);
    return 0;
}

bool httpRequest(const std::string& url, FILE* file, DownloadCallBack* callback)
{
    CURL* curl;
    CURLcode res = CURLE_OK;

    curl = curl_easy_init();
    if (curl) {

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
        curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, download_progress_callback);
        curl_easy_setopt(curl, CURLOPT_XFERINFODATA, callback);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
        {
            callback->onMessage(curl_easy_strerror(res), DOWNLOAD_FAILED);
        }
        else
        {
            callback->onMessage("œ¬‘ÿ≥…π¶", DOWNLAOD_SUCCESS);
        }

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    return res == CURLE_OK;
}