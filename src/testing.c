#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "../src/include/test.h"
#include "../src/include/openTIDAL.h"

CURL *curlTwo = NULL;

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

void http_get(response *memory)
{
    CURLcode res;
    curl_global_init(CURL_GLOBAL_ALL);
    curlTwo = curl_easy_init();
    if(curlTwo) {
        struct memory memchunk;
       
        memchunk.response = malloc(1);
        memchunk.size = 0; 
        /* Add a custom header */
    
        /* set our custom set of headers */

        /* Enable verbose mode */
        curl_easy_setopt(curlTwo, CURLOPT_VERBOSE, 1L);

        /* First set the URL that is about to receive our GET. This URL can
             just as well be a https:// URL if that is what should receive the
             data. */ 
        //curl_easy_setopt(curlTwo, CURLOPT_URL, "https://api.tidal.com/v1/users/102489236/favorites/albums?countryCode=DE");
        curl_easy_setopt(curlTwo, CURLOPT_URL, "https://nuntius.dev");
        curl_easy_setopt(curlTwo, CURLOPT_HTTPGET, 1L);        
        curl_easy_setopt(curlTwo, CURLOPT_CUSTOMREQUEST, NULL);
        curl_easy_setopt(curlTwo, CURLOPT_VERBOSE, 1L);
        /* send all data to this function  */
        curl_easy_setopt(curlTwo, CURLOPT_WRITEFUNCTION, cb);

        /* we pass our 'chunk' struct to the callback function */
        curl_easy_setopt(curlTwo, CURLOPT_WRITEDATA, &memchunk);

        /* Perform the request, res will get the return code */ 
        res = curl_easy_perform(curlTwo);
        /* Check for errors */ 
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                            curl_easy_strerror(res));
 
        /* free the custom headers */
        printf("Pointer %p\n", memory->memory); 
        memory->memory = memchunk.response;
    }
}

