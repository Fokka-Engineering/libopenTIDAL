#include "models.h"

/* baseAPI cURL handle */
curl_model curl_get(char *endpoint, char *data);
curl_model curl_post(char *endpoint, char *data, char *optHeader);
curl_model curl_delete(char *endpoint, char *data, char *optHeader);
curl_model curl_head(char *endpoint, char *data);
void curl_exit();

/* authAPI cURL handle */
curl_model curl_post_auth(char *endpoint, char *data);
void curl_exit_auth();

