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
#include "include/openTIDAL.h"

/* concatenate endpoint with an id  */
char *url_cat(const char *strOne, const size_t id, const char *strTwo, int appendCountryCode)
{
  char *urlcat;
  char buffer[20];
  snprintf(buffer, 20, "%zu", id);
  urlcat = malloc(strlen(strOne)+1+sizeof(buffer)+strlen(strTwo));
  strcpy(urlcat, strOne);
  strcat(urlcat, buffer);
  strcat(urlcat, strTwo);

  if (appendCountryCode != 0)
  {
    strcat(urlcat, "?countryCode=");
    strcat(urlcat, countryCode);
  }

  return urlcat;
}

/* concatenate endpoint with an uuid */
char *url_cat_str(const char *strOne, const char *id, const char *strTwo)
{
  char *urlcat;
  urlcat = malloc(strlen(strOne)+1+strlen(id)+strlen(strTwo));
  strcpy(urlcat, strOne);
  strcat(urlcat, id);
  strcat(urlcat, strTwo);
  return urlcat;
}

