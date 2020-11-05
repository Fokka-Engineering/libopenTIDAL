#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/openTIDAL.h"

/* concatenate endpoint with an id  */
char *url_cat(char *strOne, size_t id, char *strTwo, int appendCountryCode)
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
char *url_cat_str(char *strOne, char *id, char *strTwo)
{
  char *urlcat;
  urlcat = malloc(strlen(strOne)+1+strlen(id)+strlen(strTwo));
  strcpy(urlcat, strOne);
  strcat(urlcat, id);
  strcat(urlcat, strTwo);
  return urlcat;
}

