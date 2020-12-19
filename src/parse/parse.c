/*
  Copyright (c) 2020 Hugo Melder and openTIDAL contributors
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
  
  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.
  
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/parse.h"

void parse_string(cJSON *object, char **string)
{
  if (cJSON_IsString(object) && (!cJSON_IsNull(object)))
  {
    *string = object->valuestring; 
  }
  else
  {
    *string = NULL; 
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
