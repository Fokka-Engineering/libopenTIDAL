#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "include/cTidal.h"

const char *baseUrl = "https://api.tidal.com/v1/";
const char *authUrl = "https://auth.tidal.com/v1/";

/*libcURL Base Functions*/
struct MemoryStruct {
  char *memory;
  size_t size;
};

static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;

  char *ptr = realloc(mem->memory, mem->size + realsize + 1);
  if(ptr == NULL) {
    /* out of memory! */
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }

  mem->memory = ptr;
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  return realsize;
}

int curl_init = 0;
CURL *curl;

CURL *curl_session()
{
  curl = curl_easy_init();
  return curl;
}

void curl_exit()
{
  curl_easy_cleanup(curl);
}

/* Persistent cURL Handle for API GET Requests */
curl_model curl_get(char *endpoint, char *data)
{
  if(curl_init == 0)
    {
      curl = curl_session();
      curl_init = 1;
    }
  CURLcode res;
  curl_model model;

  struct MemoryStruct response;
  response.memory = malloc(1);  /* will be grown as needed by the realloc above */
  response.size = 0;    /* no data at this point */
  
  /*Char concatenation*/
  char *url;
  url = malloc(strlen(baseUrl)+1+strlen(endpoint)+1+strlen(data));
  strcpy(url, baseUrl);
  strcat(url, endpoint);
  strcat(url, "?");
  strcat(url, data);

  /* Create authorization header */
  char *header;
  header = malloc(strlen(access_token)+23+1);
  strcpy(header, "authorization: Bearer ");
  strcat(header, access_token);

  if(curl_init == 1) {
    struct curl_slist *chunk = NULL;
    chunk = curl_slist_append(chunk, header);
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
    
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    //curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1);
    res = curl_easy_perform(curl);
    curl_slist_free_all(chunk);

  }
  if (res != CURLE_OK) 
  {
    model.status = -1;
    return model;
  }
  else
  {
    model.body = response.memory;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &model.responseCode);
    return model;
  }

  free(url);
  free(header);
}

/* Persistent cURL Handle for API POST Requests */
curl_model curl_post(char *endpoint, char *data, char *optHeader)
{
  if(curl_init == 0)
    {
      curl = curl_session();
      curl_init = 1;
    }
  CURLcode res;
  curl_model model;

  struct MemoryStruct response;
  response.memory = malloc(1);  /* will be grown as needed by the realloc above */
  response.size = 0;    /* no data at this point */

  /*Char concatenation*/
  char *url;
  url = malloc(strlen(baseUrl)+1+strlen(endpoint));
  strcpy(url, baseUrl);
  strcat(url, endpoint);

  char *header;
  header = malloc(strlen(access_token)+23+1);
  strcpy(header, "authorization: Bearer ");
  strcat(header, access_token);

  if(curl_init == 1) {
    struct curl_slist *chunk = NULL;
    chunk = curl_slist_append(chunk, header); /*POST Header*/
    chunk = curl_slist_append(chunk, optHeader);
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk); /*Append POST Header*/
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data); /*Append (x-www-form-urlencoded) parameter*/
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    //curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1);
    res = curl_easy_perform(curl);
  }
  if (res != CURLE_OK)
  {
    model.status = -1;
    return model;
  }
  else
  {
    model.body = response.memory;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &model.responseCode);
    return model;
  }
  free(url);
  free(header);
}

/* Persistent cURL Handle for API DELETE Requests */
curl_model curl_delete(char *endpoint, char *data, char *optHeader)
{
  if(curl_init == 0)
    {
      curl = curl_session();
      curl_init = 1;
    }
  CURLcode res;
  curl_model model;

  struct MemoryStruct response;

  response.memory = malloc(1);   /* will be grown as needed by the realloc above */
  response.size = 0;     /* no data at this point */

  /*Char concatenation*/
  char *url;
  url = malloc(strlen(baseUrl)+1+strlen(endpoint)+1+strlen(data));
  strcpy(url, baseUrl);
  strcat(url, endpoint);

  /* Create authorization header */
  char *header;
  header = malloc(strlen(access_token)+23+1);
  strcpy(header, "authorization: Bearer ");
  strcat(header, access_token);

  if(curl_init == 1) {
    struct curl_slist *chunk = NULL;
    chunk = curl_slist_append(chunk, header);
    chunk = curl_slist_append(chunk, optHeader);
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response);
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
    res = curl_easy_perform(curl);
    curl_slist_free_all(chunk);

  }
  if (res != CURLE_OK)
  {
    model.status = -1;
    return model;
  }
  else
  {
    model.body = response.memory;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &model.responseCode);
    return model;
  }
  free(url);
  free(header);
}

/* Persistent cURL Handle for API HEAD Requests */
curl_model curl_head(char *endpoint, char *data)
{
  if(curl_init == 0)
    {
      curl = curl_session();
      curl_init = 1;
    }
  CURLcode res;
  curl_model model;
  struct MemoryStruct headerResponse;
  
  headerResponse.memory = malloc(1);
  headerResponse.size = 0;
  /*Char concatenation*/
  char *url;
  url = malloc(strlen(baseUrl)+1+strlen(endpoint)+1+strlen(data));
  strcpy(url, baseUrl);
  strcat(url, endpoint);
  strcat(url, "?");
  strcat(url, data);

  /* Create authorization header */
  char *header;
  header = malloc(strlen(access_token)+23+1);
  strcpy(header, "authorization: Bearer ");
  strcat(header, access_token);

  if(curl_init == 1) {
    struct curl_slist *chunk = NULL;
    chunk = curl_slist_append(chunk, header);
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &headerResponse); 
    //curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1);
    res = curl_easy_perform(curl);
    curl_slist_free_all(chunk);

  }
  if (res != CURLE_OK)
  {
    model.status = -1;
    return model;
  }
  else
  {
    model.header = headerResponse.memory;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &model.responseCode);
    return model;
  }
  free(url);
  free(header);
}
/* AUTH Handle */

int curl_init_auth = 0;
CURL *curl_auth;

CURL *curl_session_auth()
{
  curl_auth = curl_easy_init();
  return curl_auth;
}

void curl_exit_auth()
{
  curl_easy_cleanup(curl_auth);
}

curl_model curl_post_auth(char *endpoint, char *data)
{
  if(curl_init_auth == 0)
    {
      curl_auth = curl_session_auth();
      curl_init_auth = 1;
    }
  CURLcode res;
  curl_model model;
  struct MemoryStruct response;

  response.memory = malloc(1);  /* will be grown as needed by the realloc above */
  response.size = 0;    /* no data at this point */

  /*Char concatenation*/
  char *url;
  url = malloc(strlen(authUrl)+1+strlen(endpoint));
  strcpy(url, authUrl);
  strcat(url, endpoint);

  if(curl_init_auth == 1) {
    curl_easy_setopt(curl_auth, CURLOPT_URL, url);
    curl_easy_setopt(curl_auth, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl_auth, CURLOPT_POSTFIELDS, data); /*Append (x-www-form-urlencoded) parameter*/
    curl_easy_setopt(curl_auth, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl_auth, CURLOPT_USERNAME, client_id);
    curl_easy_setopt(curl_auth, CURLOPT_PASSWORD, client_secret);
    //curl_easy_setopt(curl_auth, CURLOPT_FAILONERROR, 1);
    res = curl_easy_perform(curl_auth);
  }
  free(url);
  model.body = response.memory;
  return model;
}
