#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/cTidal.h"

playlist_model get_playlist(char *playlistid)
{
  char *endpoint = url_cat_str("playlists/", playlistid, "");
  char *baseparams = param_cat("100", "", "");
  char *req = curl_get(endpoint, baseparams);
  if (req != 0)
  {
    cJSON *input_json = json_parse(req);
    return parse_playlist(input_json, 0);
    /* always cleanup */
    cJSON_Delete(input_json);
  }
  else
  {
    printf("%s\n", "Request Error: cURL returned a 4xx status code. Authorization Error.");
    playlist_model Value;
    Value.status = -1;
    return Value;
  }
  /*Cleanup*/
  free(endpoint);
  free(baseparams);
  free(req);
}

items_model get_playlist_items(char *playlistid)
{
  char *endpoint = url_cat_str("playlists/", playlistid, "/items");
  char *baseparams = param_cat("100", "", "");
  char *req = curl_get(endpoint, baseparams);
  if (req != 0)
  {
    cJSON *input_json = json_parse(req);
    return parse_items(input_json, 2, 0);
    /* always cleanup */
    cJSON_Delete(input_json);
  }
  else
  {
    printf("%s\n", "Request Error: cURL returned a 4xx status code. Authorization Error.");
    items_model Value;
    Value.status = -1;
    return Value;
  }
  /*Cleanup*/
  free(endpoint);
  free(baseparams);
  free(req);
}

char *get_playlist_etag(char *playlistid)
{
  /* Request playlist endpoint to scrape eTag Header  */
  char *endpoint = url_cat_str("playlists/", playlistid, "");
  char *baseparams = param_cat("100", "", "");
  char *req = curl_head(endpoint, baseparams); /* Returns Header with eTag */
  int i = 0; /* Counter for Buffer (Header) Splitter  */
  int e; /* Counter for ETag-String Extraction  */
  char *p = strtok (req, "\n"); 
  char *array[30]; /* Buffer for splitted HTTP-Header  */
  char *eTag = malloc(20);
  while (p != NULL)
  {
    array[i++] = p;
    p = strtok (NULL, "\n"); /* Split if char is \n  */
  }
  for (e = 0; e < i; ++e)
  {
    if (strncmp(array[e], "ETag", 4) == 0) /* If String begins with ETag  */
    {
      int charCounter;
      int eTagCounter = 0;
      for (charCounter = 7; charCounter < strlen(array[e]); ++charCounter) /* Extract ETag Values */
      {
        if (array[e][charCounter] != '"')
        {
	  strcpy(&eTag[eTagCounter], &array[e][charCounter]);
	  eTagCounter = eTagCounter + 1;
        }
      }
      strcat(&eTag[eTagCounter + 1], "\0");
    }
  }
  /*Cleanup*/
  free(endpoint);
  free(baseparams);
  free(req);
  return eTag;  
}

int delete_playlist(char *playlistid)
{
  char buffer[80];
  snprintf(buffer, 80, "playlists/%s?countryCode=%s", playlistid, countryCode);

  char *req = curl_delete(buffer, "", "");
  if (req != 0)
  {
    return 1;
  }
  else
  {
    return -1;
  }
  /*Cleanup*/
  free(req);
}

int delete_playlist_item(char *playlistid, int index, char *eTag)
{
  char *eTagHeader = malloc(strlen(eTag)+16);
  strcpy(eTagHeader, "if-none-match: ");
  strcat(eTagHeader, eTag);
  printf("%s\n", eTagHeader);
  char buffer[80];
  snprintf(buffer, 80, "playlists/%s/items/%d?countryCode=%s", playlistid, index, countryCode);
  char *req = curl_delete(buffer, "", eTagHeader);
  if (req != 0)
  {
    return 1;
  }
  else
  {
    return -1;
  }
  /*Cleanup*/
  free(eTagHeader);
  free(req);
}

int move_playlist_item(char *playlistid, int index, int toIndex, char *eTag)
{
  char buffer[80];
  snprintf(buffer, 80, "playlists/%s/items/%d?countryCode=%s", playlistid, index, countryCode);

  char bufferTwo[20];
  snprintf(bufferTwo, 20, "toIndex=%d", toIndex);
  char *req = curl_post(buffer, bufferTwo, "");
  if (req != 0)
  {
    return 1;
  }
  else
  {
    return -1;
  }
  /*Cleanup*/
  free(req);
}

int add_playlist_item(char *playlistid, int trackid, char *onDupes) /* onDupes = ADD or SKIP  */
{
  char *endpoint = url_cat("playlists/", playlistid, "/items", 1);
  
  char buffer[60];
  snprintf(buffer, 60, "trackIds=%zu&onDupes=%s&onArtifactNotFound=FAIL", trackid, dupes);

  char *req = curl_post(endpoint, buffer, "");
  if (req != 0)
  {
    return 1;
  }
  else
  {
    return -1;
  }
  /*Cleanup*/
  free(endpoint);
  free(data);
  free(req);
}

int add_playlist_items(char *playlistid, char *trackids, char *onDupes)
{
  char *endpoint = url_cat("playlists/", playlistid, "/items", 1);

  char buffer[60];
  snprintf(buffer, 60, "trackIds=%s&onDupes=%s&onArtifactNotFound=FAIL", trackids, dupes);

  char *req = curl_post(endpoint, buffer, "");
  if (req != 0)
  {
    return 1;
  }
  else
  {
    return -1;
  }
  /*Cleanup*/
  free(endpoint);
  free(data);
  free(req);
}
