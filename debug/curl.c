#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

const char *access_token = "Authorization: Bearer eyJraWQiOiI4UUhqclVzdSIsImFsZyI6IlJTMjU2In0.eyJ0eXBlIjoibzJfYWNjZXNzIiwidWlkIjoxMDI0ODkyMzYsInNjb3BlIjoicl91c3Igd191c3IiLCJnVmVyIjowLCJzVmVyIjowLCJjaWQiOjIyMzUsImN1ayI6IjVjMWEwYTVlLTBkZTItNDk3Zi05OGNhLTQ2MmFkMDI3NzU5NiIsImV4cCI6MTYwODgwMzU5NSwic2lkIjoiZTY2MTRhOTktZjgyZS00MGVhLTg5OTQtZDI3ZjNkYjQ4YjIxIiwiaXNzIjoiaHR0cHM6Ly9hdXRoLnRpZGFsLmNvbS92MSJ9.icIj83KYqX-ykWQRikD2LdW-GndxuebamBqWYQiWNgS-ZHDeaWp3fTjeG0ndOb5jJkD_-SvUCoAcV6SY7TF5mkm-EtDnzrVgUW1ciX_COvc5ZGVq6KZ6ojjai40TIPG373VUup3m5rdVmTSFQ60k7x1OANXvlZoYk6TIeVdQRuv-kl2t1KzVUJfB5BL3GTbCwGlnjQs-J4-RFvPxVpFRwOdGdW6BTcvoD5UZonmEuG6IXl4euRShhwlJwqQNsuUHtASgAybT3o5j58U3Ywtbp8TPoPmePrtyJjvCBu1RTXcYaabLlwxSLRCbEBC9TPRQiZXOMJqPmBnthr6LuCQz4g";

CURL *curl = NULL;

struct memory {
    char *response;
    size_t size;
};

static size_t cb(void *data, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct memory *mem = (struct memory *)userp;
    
    char *ptr = realloc(mem->response, mem->size + realsize + 1);
    if(ptr == NULL)
            return 0;  /* out of memory! */

    mem->response = ptr;
    memcpy(&(mem->response[mem->size]), data, realsize);
    mem->size += realsize;
    mem->response[mem->size] = 0;

    return realsize;
}

size_t cb_dummy(char *ptr, size_t size, size_t nmemb, void *userp) {
        return size * nmemb;
}

void http_get()
{
    CURLcode res;

    if(curl) {
        struct curl_slist *chunk = NULL;
        struct memory memchunk;
        
        /* Add a custom header */
        chunk = curl_slist_append(chunk, access_token);
    
        /* set our custom set of headers */
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);

        /* Enable verbose mode */
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        /* First set the URL that is about to receive our GET. This URL can
             just as well be a https:// URL if that is what should receive the
             data. */ 
        //curl_easy_setopt(curl, CURLOPT_URL, "https://api.tidal.com/v1/users/102489236/favorites/albums?countryCode=DE");
        curl_easy_setopt(curl, CURLOPT_URL, "https://nuntius.dev");
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);        
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, NULL);

        /* send all data to this function  */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);

        /* we pass our 'chunk' struct to the callback function */
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&memchunk);

        /* Perform the request, res will get the return code */ 
        res = curl_easy_perform(curl);
        /* Check for errors */ 
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                            curl_easy_strerror(res));
 
        /* free the custom headers */
        curl_slist_free_all(chunk);

    }
}

void http_post_quiet()
{
    CURLcode res;
         
    if(curl) {
        struct curl_slist *chunk = NULL;
        
        /* Add a custom header */
        chunk = curl_slist_append(chunk, access_token);
        
        /* set our custom set of headers */
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);

        /* Enable verbose mode */
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        /* First set the URL that is about to receive our POST. This URL can
             just as well be a https:// URL if that is what should receive the
             data. */ 
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.tidal.com/v1/users/102489236/favorites/albums?countryCode=DE");
        
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, NULL);
        /* Now specify the POST data */ 
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "albumIds=36305239&onArtifactNotFound=FAIL");

        /* send all data to this function  */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb_dummy);

        /* Perform the request, res will get the return code */ 
        res = curl_easy_perform(curl);
        /* Check for errors */ 
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                            curl_easy_strerror(res));
 
        /* free the custom headers */
        curl_slist_free_all(chunk);
    }

}


int main()
{
    /* In windows, this will init the winsock stuff */ 
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    http_post_quiet();
    http_get(); 
    http_post_quiet(); 
    /* always cleanup */ 
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    return 0;
}
