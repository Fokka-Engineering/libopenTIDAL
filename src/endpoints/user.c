#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/parse.h"
#include "../include/handles.h"
#include "../include/openTIDAL.h"

user_model get_user()
{
  user_model Value;
  char *endpoint = url_cat("users/", userId, "", 0);
  
  char baseparams[20];
  snprintf(baseparams, 20, "countryCode=%s", countryCode);

  curl_model req = curl_get(endpoint, baseparams);
  free(endpoint);
  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    if (req.responseCode == 200)
    {
      Value.status = 1;
      cJSON_Delete(input_json);
      free(req.body);
      return Value; /*return data structure*/
    }
    else
    {
      Value.status = parse_status(input_json, req, userId, NULL);
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
  }
  else
  {
    Value.status = -1;
    free(req.body);
    fprintf(stderr, "[Request Error] User %zu: CURLE_OK Check failed.", userId);
    return Value;
  }
}

/* Create/Manipulate/Delete favorites */

int add_user_album(size_t albumid)
{
  char *endpoint = url_cat("users/", userId, "/favorites/albums", 1);
  int status;
  char buffer[60];
  snprintf(buffer, 60, "albumIds=%zu&onArtifactNotFound=FAIL", albumid);

  curl_model req = curl_post(endpoint, buffer, "");
  free(endpoint);
  free(req.body);
  if (req.status != -1)
  {
    if (req.responseCode == 200)
    {
      return 1;
    }
    else if (req.responseCode == 400)
    {
      status = -11;
      return status;
    }
    else if (req.responseCode == 401)
    {
      status = -8;
      return status;
    }
    else if (req.responseCode == 404)
    {
      status = -2;
      return status;
    }
    else
    {
      status = 0;
      return status;
    }
  }
  else
  {
    return -1;
  }
}

int add_user_artist(size_t artistid)
{
  char *endpoint = url_cat("users/", userId, "/favorites/artists", 1);
  int status;
  char buffer[60];
  snprintf(buffer, 60, "artistIds=%zu&onArtifactNotFound=FAIL", artistid);

  curl_model req = curl_post(endpoint, buffer, "");
  free(endpoint);
  free(req.body);
  if (req.status != -1)
  {
    if (req.responseCode == 200)
    {
      return 1;
    }
    else if (req.responseCode == 400)
    {
      status = -11;
      return status;
    }
    else if (req.responseCode == 401)
    {
      status = -8;
      return status;
    }
    else if (req.responseCode == 404)
    {
      status = -2;
      return status;
    }
    else
    {
      status = 0;
      return status;
    }
  }
  else
  {
    return -1;
  }
}

int add_user_playlist(char *playlistid)
{
  char *endpoint = url_cat("users/", userId, "/favorites/playlists", 1);
  int status;
  char *data = malloc(strlen(playlistid)+10+25+1);
  strcpy(data, "uuids=");
  strcat(data, playlistid);
  strcat(data, "&onArtifactNotFound=FAIL");

  curl_model req = curl_post(endpoint, data, "");
  free(endpoint);
  free(data);
  free(req.body);
  if (req.status != -1)
  {
    if (req.responseCode == 200)
    {
      return 1;
    }
    else if (req.responseCode == 400)
    {
      status = -11;
      return status;
    }
    else if (req.responseCode == 401)
    {
      status = -8;
      return status;
    }
    else if (req.responseCode == 404)
    {
      status = -2;
      return status;
    }
    else
    {
      status = 0;
      return status;
    }
  }
  else
  {
    return -1;
  }
}

int add_user_track(size_t trackid)
{
  char *endpoint = url_cat("users/", userId, "/favorites/tracks", 1);
  int status;
  char buffer[60];
  snprintf(buffer, 60, "trackIds=%zu&onArtifactNotFound=FAIL", trackid);

  curl_model req = curl_post(endpoint, buffer, "");
  free(endpoint);
  free(req.body);
  if (req.status != -1)
  {
    if (req.responseCode == 200)
    {
      return 1;
    }
    else if (req.responseCode == 400)
    {
      status = -11;
      return status;
    }
    else if (req.responseCode == 401)
    {
      status = -8;
      return status;
    }
    else if (req.responseCode == 404)
    {
      status = -2;
      return status;
    }
    else
    {
      status = 0;
      return status;
    }
  }
  else
  {
    return -1;
  }
}

int add_user_video(size_t videoid)
{
  char *endpoint = url_cat("users/", userId, "/favorites/videos", 1);
  int status;
  char buffer[60];
  snprintf(buffer, 60, "videoIds=%zu&onArtifactNotFound=FAIL", videoid);
  
  curl_model req = curl_post(endpoint, buffer, "");
  /*Cleanup*/
  free(endpoint);
  free(req.body);

  if (req.status != -1)
  {
    if (req.responseCode == 200)
    {
      return 1;
    }
    else if (req.responseCode == 400)
    {
      status = -11;
      return status;
    }
    else if (req.responseCode == 401)
    {
      status = -8;
      return status;
    }
    else if (req.responseCode == 404)
    {
      status = -2;
      return status;
    }
    else
    {
      status = 0;
      return status;
    }
  }
  else
  {
    return -1;
  }
}

playlist_model create_user_playlist(char *title, char *description)
{
  playlist_model Value;
  char *endpoint = url_cat("users/", userId, "/playlists", 1);

  char *data = malloc(strlen(title)+strlen(description)+7+14+1);
  strcpy(data, "title=");
  strcat(data, title);
  strcat(data, "&description=");
  strcat(data, description);

  curl_model req = curl_post(endpoint, data, "");
  /*Cleanup*/
  free(endpoint);
  free(data);

  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    if (req.responseCode == 201 || req.responseCode == 200)
    {
      json_playlist_model processed_json = json_parse_playlist(input_json);
      Value = parse_playlist_values(processed_json, 0);
      Value.status = 1;
      Value.arraySize = 1;

      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
    else
    {
      Value.status = parse_status(input_json, req, userId, NULL);
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
  }
  else
  {
    printf("[Request Error] CURLE_OK Check failed.\n");
    playlist_model Value;
    Value.status = -1;
    free(req.body);
    return Value;
  }
}

/* DELETE */

int delete_user_album(size_t albumid)
{
  int status;
  char buffer[80];
  snprintf(buffer, 80, "users/%zu/favorites/albums/%zu?countryCode=%s", userId,
            albumid, countryCode);
  
  curl_model req = curl_delete(buffer, "", "");

  if (req.status != -1)
  {
    if (req.responseCode == 200)
    {
      return 1;
    }
    else if (req.responseCode == 400)
    {
      status = -11;
      return status;
    }
    else if (req.responseCode == 401)
    {
      status = -8;
      return status;
    }
    else if (req.responseCode == 404)
    {
      status = -2;
      return status;
    }
    else
    {
      status = 0;
      return status;
    }
  }
  else
  {
    return -1;
  }
}

int delete_user_artist(size_t artistid)
{
  int status;
  char buffer[80];
  snprintf(buffer, 80, "users/%zu/favorites/artists/%zu?countryCode=%s", userId, artistid, countryCode);

  curl_model req = curl_delete(buffer, "", "");
  /*Cleanup*/
  free(req.body);

  if (req.status != -1)
  {
    if (req.responseCode == 200)
    {
      return 1;
    }
    else if (req.responseCode == 400)
    {
      status = -11;
      return status;
    }
    else if (req.responseCode == 401)
    {
      status = -8;
      return status;
    }
    else if (req.responseCode == 404)
    {
      status = -2;
      return status;
    }
    else
    {
      status = 0;
      return status;
    }
  }
  else
  {
    return -1;
  }
}

int delete_user_playlist(char *playlistid)
{
  int status;
  char buffer[80];
  snprintf(buffer, 80, "users/%zu/favorites/playlists/%s?countryCode=%s", userId, playlistid, countryCode);

  curl_model req = curl_delete(buffer, "", "");
  /*Cleanup*/
  free(req.body);

  if (req.status != -1)
  {
    if (req.responseCode == 200)
    {
      return 1;
    }
    else if (req.responseCode == 400)
    {
      status = -11;
      return status;
    }
    else if (req.responseCode == 401)
    {
      status = -8;
      return status;
    }
    else if (req.responseCode == 404)
    {
      status = -2;
      return status;
    }
    else
    {
      status = 0;
      return status;
    }
  }
  else
  {
    return -1;
  }
}

int delete_user_track(size_t trackid)
{
  int status;
  char buffer[80];
  snprintf(buffer, 80, "users/%zu/favorites/tracks/%zu?countryCode=%s", userId, trackid, countryCode);

  curl_model req = curl_delete(buffer, "", "");
  /*Cleanup*/
  free(req.body);

  if (req.status != -1)
  {
    if (req.responseCode == 200)
    {
      return 1;
    }
    else if (req.responseCode == 400)
    {
      status = -11;
      return status;
    }
    else if (req.responseCode == 401)
    {
      status = -8;
      return status;
    }
    else if (req.responseCode == 404)
    {
      status = -2;
      return status;
    }
    else
    {
      status = 0;
      return status;
    }
  }
  else
  {
    return -1;
  }
}

int delete_user_video(size_t videoid)
{
  int status;
  char buffer[80];
  snprintf(buffer, 80, "users/%zu/favorites/videos/%zu?countryCode=%s", userId, videoid, countryCode);

  curl_model req = curl_delete(buffer, "", "");
  /*Cleanup*/
  free(req.body);

  if (req.status != -1)
  {
    if (req.responseCode == 200)
    {
      return 1;
    }
    else if (req.responseCode == 400)
    {
      status = -11;
      return status;
    }
    else if (req.responseCode == 401)
    {
      status = -8;
      return status;
    }
    else if (req.responseCode == 404)
    {
      status = -2;
      return status;
    }
    else
    {
      status = 0;
      return status;
    }
  }
  else
  {
    return -1;
  }
}
