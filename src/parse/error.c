#include <stdio.h>
#include <stdlib.h>
#include "../include/openTIDAL.h"

int parse_unauthorized(cJSON *input_json, size_t id)
{
  int status;
  const cJSON *subStatus = NULL;
  subStatus = cJSON_GetObjectItem(input_json, "subStatus");
  if (cJSON_IsNumber(subStatus))
  {
    if (subStatus->valueint == 4005)
    {
      status = -3;
      fprintf(stderr, "[401] Asset %zu is not ready for playback\n", id);
    }
    else if (subStatus->valueint == 6005)
    {
      status = -7;
      fprintf(stderr, "[401] Missing authorization header\n");
    }
    else if (subStatus->valueint == 11002)
    {
      status = -6;
      fprintf(stderr, "[401] Authorization header is invalid\n");
    }
    else if (subStatus->valueint == 11003)
    {
      status = -5;
      fprintf(stderr, "[401] The token has expired. (Expired on time)\n");
    }
    else
    {
      status = 0;
      fprintf(stderr, "[401] Unauthorized");
    }
  }
  return status;
}

int parse_notfound(cJSON *input_json, size_t id, char *uuid)
{
  int status;
  const cJSON *subStatus = NULL;
  subStatus = cJSON_GetObjectItem(input_json, "subStatus");
  if (cJSON_IsNumber(subStatus))
  {
    if (subStatus->valueint == 2001)
    {
      status = -2;
      if (uuid == NULL)
      {
        fprintf(stderr, "[404] Resource %zu not found\n", id);
      }
      else
      {
        fprintf(stderr, "[404] Resource %s not found\n", uuid);
      }
    }
    else
    {
      status = 0;
      fprintf(stderr, "[404] Not Found");
    }
  }
  return status;
}

int parse_preconditionfailed(cJSON *input_json, size_t id, char *uuid)
{
  int status;
  const cJSON *subStatus = NULL;
  subStatus = cJSON_GetObjectItem(input_json, "subStatus");
  if (cJSON_IsNumber(subStatus))
  {
    if (subStatus->valueint == 7002)
    {
      status = -4;
      if (uuid == NULL)
      {
        fprintf(stderr, "[412] If-None-Match (eTag) failed for %zu\n", id);
      }
      else
      {
        fprintf(stderr, "[412] If-None-Match (eTag) failed for%s\n", uuid);
      }
    }
    else
    {
      status = 0;
      fprintf(stderr, "[412] Precondition Failed");
    }
  }
  return status;
}
