#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/parse.h"

void parse_string(cJSON *object, char *string, size_t length)
{
  if (cJSON_IsString(object)&& (!cJSON_IsNull(object)))
  {
    strncpy(string, object->valuestring, length);
  }
  else
  {
    strcpy(string, "\0");
  }
}

void parse_number(cJSON *object, size_t *number)
{
  if (cJSON_IsNumber(object) && (!cJSON_IsNull(object)))
  {
    *number = object->valueint;
  }
  else
  {
    *number = 0;
  }
}

void parse_double(cJSON *object, double *number)
{
  if (cJSON_IsNumber(object) && (!cJSON_IsNull(object)))
  {
    *number = object->valuedouble;
  }
  else
  {
    *number = 0;
  }
}

void parse_bool(cJSON *object, size_t *number)
{
  *number = cJSON_IsTrue(object);
}
