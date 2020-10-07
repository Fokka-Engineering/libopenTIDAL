#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/cTidal.h"

playlist_model get_playlist(char *playlistid)
{
  char *endpoint = url_cat_str("playlists/", playlistid, "");
  char *baseparams = param_cat("100", "", "");
  char *req = curl_get(endpoint, baseparams, 0);
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
  char *req = curl_get(endpoint, baseparams, 0);
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

int delete_playlist(char *playlistid)
{
  char buffer[80];
  snprintf(buffer, 80, "playlists/%s?countryCode=%s", playlistid, countryCode);

  char *req = curl_delete(buffer, "");
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

int delete_playlist_item(char *playlistid, int index)
{
  char *preEndpoint = url_cat_str("playlists/", playlistid, "");
  char *baseparams = param_cat("100", "", "");
  char *req = curl_get(preEndpoint, baseparams, 1); /* Returns Header with eTag */
  
  int i = 0; /* Counter for Buffer (Header) Splitter  */
  int e; /* Counter for ETag-String Extraction  */
  char *p = strtok (req, "\n"); 
  char *array[30]; /* Buffer for splitted HTTP-Header  */
  char eTag[20];
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
      for (charCounter = 7; charCounter < strlen(array[e]); ++charCounter) /* Extract ETag Values */
      {
        if (array[e][charCounter] != '"')
        {
	  //strcat(&eTag[eTagCounter], &array[e][charCounter]);
	  printf("%c", array[e][charCounter]);
        }
      }
      //strcat(eTag, "\0");
      printf("\n");
    }
  }
  //printf("%s\n", eTag);

  /*Cleanup*/
  free(req);
  return 0;
}
