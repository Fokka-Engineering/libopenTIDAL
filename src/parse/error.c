#include <stdio.h>
#include <stdlib.h>
#include "../include/parse.h"
#include "../include/openTIDAL.h"

int parse_status(cJSON *input_json, curl_model Value, size_t id, char *uuid)
{
  int status = 0;

  if (Value.responseCode == 400)
  {
    status = parse_badrequest(input_json, id, uuid);
  }
  else if (Value.responseCode == 401)
  {
    status = parse_unauthorized(input_json, id);
  }
  else if (Value.responseCode == 404)
  {
    status = parse_notfound(input_json, id, uuid);
  }
  else
  {
    status = 0;
  }

  return status;
}

int parse_unauthorized(cJSON *input_json, size_t id)
{
  int status = 0;
  const cJSON *subStatus = NULL;
  subStatus = cJSON_GetObjectItem(input_json, "subStatus");
  if (cJSON_IsNumber(subStatus) == 1)
  {
    if (subStatus->valueint == 4005)
    {
      status = -3;
      fprintf(stderr, "[401] Asset %zu is not ready for playback\n", id);
    }
    else if (subStatus->valueint == 6001)
    {
      status = -13;
      fprintf(stderr, "[401] User does not have a valid session\n");
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
      fprintf(stderr, "[401] Unauthorized\n");
    }
  }
  return status;
}

int parse_notfound(cJSON *input_json, size_t id, char *uuid)
{
  int status = 0;
  const cJSON *subStatus = NULL;
  subStatus = cJSON_GetObjectItem(input_json, "subStatus");
  if (cJSON_IsNumber(subStatus) == 1)
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
      fprintf(stderr, "[404] Not Found\n");
    }
  }
  return status;
}

int parse_preconditionfailed(cJSON *input_json, size_t id, char *uuid)
{
  int status = 0;
  const cJSON *subStatus = NULL;
  subStatus = cJSON_GetObjectItem(input_json, "subStatus");
  if (cJSON_IsNumber(subStatus) == 1)
  {
    if (subStatus->valueint == 7002)
    {
      status = -4;
      if (uuid == NULL)
      {
        fprintf(stderr, "[412] If-None-Match (eTag) failed for Resource %zu\n", id);
      }
      else
      {
        fprintf(stderr, "[412] If-None-Match (eTag) failed for Resource %s\n", uuid);
      }
    }
    else
    {
      status = 0;
      fprintf(stderr, "[412] Precondition Failed\n");
    }
  }
  return status;
}

int parse_badrequest(cJSON *input_json, size_t id, char *uuid)
{
  int status = 0;
  const cJSON *subStatus = NULL;
  subStatus = cJSON_GetObjectItem(input_json, "subStatus");
  if (cJSON_IsNumber(subStatus) == 1)
  {
    if (subStatus->valueint == 1002)
    {
      status = -4;
      if (uuid == NULL)
      {
        fprintf(stderr, "[400] Parameter missing for Resource %zu\n", id);
      }
      else
      {
        fprintf(stderr, "[400] Parameter missing for Resource %s\n", uuid);
      }
    }
    else if (subStatus->valueint == 1005)
    {
      status = -12;
      if (uuid == NULL)
      {
        fprintf(stderr, "[400] User not found for Resource %zu\n", id);
      }
      else
      {
        fprintf(stderr, "[400] User not found for Resource %s\n", uuid);
      }
    }
    else if (subStatus->valueint == 6003)
    {
      status = -7;
      if (uuid == NULL)
      {
        fprintf(stderr, "[400] Token missing for Resource %zu\n", id);
      }
      else
      {
        fprintf(stderr, "[400] Token missing for Resource %s\n", uuid);
      }
    }
    else
    {
      status = 0;
      fprintf(stderr, "[400] Bad Request\n");
    }
  }
  return status;
}
