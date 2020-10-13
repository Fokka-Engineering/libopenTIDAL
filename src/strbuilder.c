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

/* concatenate standard url parameter */
char *param_cat(char *limit, char *query, char *extra)
{
  char *paramcat;
  paramcat = malloc(strlen(query)+strlen(extra)+strlen(countryCode)+strlen(soundQuality)+62+1);
  strcat(paramcat, "countryCode=");
  strcat(paramcat, countryCode);
  strcat(paramcat, "&soundQuality=");
  strcat(paramcat, soundQuality);
  if (strlen(limit) != 0)
  {
    strcat(paramcat, "&limit=");
    strcat(paramcat, limit);
  }
  if (strlen(query) != 0)
  {
    strcat(paramcat, "&query=");
    strcat(paramcat, query);
  }
  if (strlen(extra) != 0)
  {
    strcat(paramcat, "&");
    strcat(paramcat, extra);
  }
  return paramcat;
}
