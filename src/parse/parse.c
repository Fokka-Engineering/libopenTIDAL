#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/openTIDAL.h"

cJSON *json_parse(char * input)
{
  cJSON *input_json = cJSON_Parse(input);
  int status = 0;
  if (input_json == NULL)
  {
      const char *error_ptr = cJSON_GetErrorPtr();
      if (error_ptr != NULL)
      {
          /*JSON Input empty*/
          fprintf(stderr, "[Request] JSON Input empty\n");
      }
      status = 0;
  }
  return input_json;
}
