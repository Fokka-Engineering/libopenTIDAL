#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/openTIDAL.h"

playlist_model get_playlist(char *playlistid)
{
  playlist_model Value;
  char *endpoint = url_cat_str("playlists/", playlistid, "");
  char *baseparams = param_cat("100", "", "");
  curl_model req = curl_get(endpoint, baseparams);

  free(endpoint);
  free(baseparams);
  
  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    if (req.responseCode == 200)
    {
      playlist_model parse = parse_playlist(input_json, 0);
      /* Cleanup  */
      cJSON_Delete(input_json);
      free(req.body);
      return parse;
    }
    else if (req.responseCode == 401)
    {
      Value.status = parse_unauthorized(input_json, 0);
      /* Cleanup  */
      cJSON_Delete(input_json);
      free(req.body);
      return Value;
    }
    else if (req.responseCode == 404)
    {
      Value.status = parse_notfound(input_json, 0, playlistid);
      /* Cleanup  */
      cJSON_Delete(input_json);
      free(req.body);
      return Value;
    }
    else
    {
      Value.status = 0;
      /* Cleanup  */
      cJSON_Delete(input_json);
      free(req.body);
      return Value;
    }
  }
  else
  {
    free(req.body);
    Value.status = -1;
    fprintf(stderr, "[Request Error] Playlist %s: CURLE_OK Check failed.\n", playlistid);
    return Value;
  }
}

items_model get_playlist_items(char *playlistid)
{
  items_model Value;
  char *endpoint = url_cat_str("playlists/", playlistid, "/items");
  char *baseparams = param_cat("100", "", "");
  curl_model req = curl_get(endpoint, baseparams);
  free(endpoint);
  free(baseparams);
  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    if (req.responseCode == 200)
    {
      items_model parse = parse_items(input_json, 2, 0);
      /* Cleanup  */
      cJSON_Delete(input_json);
      free(req.body);
      return parse;
    }
    else if (req.responseCode == 401)
    {
      Value.status = parse_unauthorized(input_json, 0);
      /* Cleanup  */
      cJSON_Delete(input_json);
      free(req.body);
      return Value;
    }
    else if (req.responseCode == 404)
    {
      Value.status = parse_notfound(input_json, 0, playlistid);
      /* Cleanup  */
      cJSON_Delete(input_json);
      free(req.body);
      return Value;
    }
    else
    {
      Value.status = 0;
      /* Cleanup  */
      cJSON_Delete(input_json);
      free(req.body);
      return Value;
    }
  }
  else
  {
    free(req.body);
    Value.status = -1;
    fprintf(stderr, "[Request Error] Playlist %s: CURLE_OK Check failed.\n", playlistid);
    return Value;
  }
}

char *get_playlist_etag(char *playlistid)
{
  /* Request playlist endpoint to scrape eTag Header  */
  char *endpoint = url_cat_str("playlists/", playlistid, "");
  char *baseparams = param_cat("100", "", "");
  curl_model req = curl_head(endpoint, baseparams); /* Returns Header with eTag */
  free(endpoint);
  free(baseparams);
  if (req.status != -1)
  {
    if (req.responseCode == 200)
    {
      int i = 0; /* Counter for Buffer (Header) Splitter  */
      int e; /* Counter for ETag-String Extraction  */
      char *p = strtok (req.header, "\n");
      char eTag[20];
      char *eTagHeader = malloc(30);
      /* Buffer for splitted HTTP-Header  */ 
      char *array[30];
      while (p != NULL)
      {
        array[i++] = p;
	/* Split if char is \n  */
        p = strtok (NULL, "\n");
      }
      for (e = 0; e < i; ++e)
      {
        /* If String begins with ETag  */
        if (strncmp(array[e], "ETag", 4) == 0)
        {
          int charCounter;
          int eTagCounter = 0;
	  /* Extract ETag Values */
          for (charCounter = 7; charCounter < strlen(array[e]); ++charCounter)
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
      snprintf(eTagHeader, 30, "if-none-match: %s", eTag);
      free(req.header);
      return eTagHeader;
    }
    else
    {
      fprintf(stderr, "Could not parse eTag-Header. Not a 200 Response.\n");
      free(req.header);
      return 0;
    }
  }
  else
  {
    free(req.body);
    fprintf(stderr, "[Request Error] Playlist %s: CURLE_OK Check failed.\n", playlistid);
    return 0;
  }
}

int delete_playlist(char *playlistid)
{
  char buffer[80];
  snprintf(buffer, 80, "playlists/%s?countryCode=%s", playlistid, countryCode);

  curl_model req = curl_delete(buffer, "", "");
  if (req.status != -1)
  {
    if (req.responseCode == 200)
    {
      free(req.body);
      return 1;
    }
    else if (req.responseCode == 400)
    {
      free(req.body);
      fprintf(stderr, "[400] Invalid Indices\n");
      return -9;
    }
    else if (req.responseCode == 401)
    {
      free(req.body);
      fprintf(stderr, "[401] Unauthorized\n");
      return -8;
    }
    else if (req.responseCode == 404)
    {
      free(req.body);
      fprintf(stderr, "[404] Resource %s not found\n", playlistid);
      return -2;
    }
    else if (req.responseCode == 412)
    {
      free(req.body);
      fprintf(stderr, "[412] Resource %s eTag invalid\n", playlistid);
      return -4;
    }
    else
    {
      free(req.body);
      return 0;
    }
  }
  else
  {
    free(req.body);
    fprintf(stderr, "[Request Error] Playlist %s: CURLE_OK Check failed.\n", playlistid);
    return -1;
  }  
}

int delete_playlist_item(char *playlistid, size_t index, char *eTagHeader)
{
  char buffer[80];
  snprintf(buffer, 80, "playlists/%s/items/%zu?countryCode=%s", playlistid, index, countryCode);
  curl_model req = curl_delete(buffer, "", eTagHeader);
  
  /* cleanup */
  free(eTagHeader);
  free(req.body);
  if (req.status != -1)
  {
    if (req.responseCode == 200)
    {
      return 1;
    }
    else if (req.responseCode == 400)
    {
      fprintf(stderr, "[400] Invalid Indices\n");
      return -9;
    }
    else if (req.responseCode == 401)
    {
      fprintf(stderr, "[401] Unauthorized\n");
      return -8;
    }
    else if (req.responseCode == 404)
    {
      fprintf(stderr, "[404] Resource %s not found\n", playlistid);
      return -2;
    }
    else if (req.responseCode == 412)
    {
      fprintf(stderr, "[412] Resource %s eTag invalid\n", playlistid);
      return -4;
    }
    else
    {
      return 0;
    }
  }
  else
  {
    fprintf(stderr, "[Request Error] Playlist %s: CURLE_OK Check failed.\n", playlistid);
    return -1;
  }
}

int move_playlist_item(char *playlistid, size_t index, size_t toIndex, char *eTagHeader)
{
  char buffer[80];
  snprintf(buffer, 80, "playlists/%s/items/%zu?countryCode=%s", playlistid, index, countryCode);

  char bufferTwo[20];
  snprintf(bufferTwo, 20, "toIndex=%zu", toIndex);
  curl_model req = curl_post(buffer, bufferTwo, eTagHeader);
  
  /* cleanup */
  free(eTagHeader);
  free(req.body);
  if (req.status != -1)
  {
    if (req.responseCode == 200)
    {
      return 1;
    }
    else if (req.responseCode == 400)
    {
      fprintf(stderr, "[400] Invalid Indices\n");
      return -9;
    }
    else if (req.responseCode == 401)
    {
      fprintf(stderr, "[401] Unauthorized\n");
      return -8;
    }
    else if (req.responseCode == 404)
    {
      fprintf(stderr, "[404] Resource %s not found\n", playlistid);
      return -2;
    }
    else if (req.responseCode == 412)
    {
      fprintf(stderr, "[412] Resource %s eTag invalid\n", playlistid);
      return -4;
    }
    else
    {
      return 0;
    }
  }
  else
  {
    fprintf(stderr, "[Request Error] Playlist %s: CURLE_OK Check failed.\n", playlistid);
    return -1;
  }
}

int add_playlist_item(char *playlistid, size_t trackid, char *onDupes, char *eTagHeader)
{
  char buffer[80];
  snprintf(buffer, 80, "playlists/%s/items?countryCode=%s", playlistid, countryCode);
  /* onDupes = ADD or SKIP  */
  char bufferTwo[60];
  snprintf(bufferTwo, 60, "trackIds=%zu&onDupes=%s&onArtifactNotFound=FAIL", trackid, onDupes);
  curl_model req = curl_post(buffer, bufferTwo, eTagHeader);
  
  /* cleanup */
  free(eTagHeader);
  free(req.body);
  if (req.status != -1)
  {
    if (req.responseCode == 200)
    {
      return 1;
    }
    else if (req.responseCode == 400)
    {
      fprintf(stderr, "[400] Invalid Indices\n");
      return -9;
    }
    else if (req.responseCode == 401)
    {
      fprintf(stderr, "[401] Unauthorized\n");
      return -8;
    }
    else if (req.responseCode == 404)
    {
      fprintf(stderr, "[404] Resource %s not found\n", playlistid);
      return -2;
    }
    else if (req.responseCode == 412)
    {
      fprintf(stderr, "[412] Resource %s eTag invalid\n", playlistid);
      return -4;
    }
    else
    {
      return 0;
    }
  }
  else
  {
    fprintf(stderr, "[Request Error] Playlist %s: CURLE_OK Check failed.\n", playlistid);
    return -1;
  } 
}

int add_playlist_items(char *playlistid, char *trackids, char *onDupes, char *eTagHeader)
{
  char buffer[80];
  snprintf(buffer, 80, "playlists/%s/items?countryCode=%s", playlistid, countryCode);

  char bufferTwo[60];
  snprintf(bufferTwo, 60, "trackIds=%s&onDupes=%s&onArtifactNotFound=FAIL", trackids, onDupes);
  curl_model req = curl_post(buffer, bufferTwo, eTagHeader);
  
  /* cleanup */
  free(eTagHeader);
  free(req.body);
  if (req.status != -1)
  {
    if (req.responseCode == 200)
    {
      return 1;
    }
    else if (req.responseCode == 400)
    {
      fprintf(stderr, "[400] Invalid Indices\n");
      return -9;
    }
    else if (req.responseCode == 401)
    {
      fprintf(stderr, "[401] Unauthorized\n");
      return -8;
    }
    else if (req.responseCode == 404)
    {
      fprintf(stderr, "[404] Resource %s not found\n", playlistid);
      return -2;
    }
    else if (req.responseCode == 412)
    {
      fprintf(stderr, "[412] Resource %s eTag invalid\n", playlistid);
      return -4;
    }
    else
    {
      return 0;
    }
  }
  else
  {
    fprintf(stderr, "[Request Error] Playlist %s: CURLE_OK Check failed.\n", playlistid);
    return -1;
  }
}
