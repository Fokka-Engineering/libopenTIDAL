#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/cTidal.h"

user_model get_user(size_t userid) /* TODO: Remove email or check if != NULL  */
{
  user_model Value;
  char *endpoint = url_cat("users/", userid, "", 0); /*concatenate endpoint url*/
  char *baseparams = param_cat("", "", "");
  curl_model req = curl_get(endpoint, baseparams); /*send request and pipe response into json parser*/
  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    Value.status = 1;
    Value.id = cJSON_GetObjectItemCaseSensitive(input_json, "id")->valueint; /*parse JSON objects into data structure*/
    strcpy(Value.username, cJSON_GetObjectItemCaseSensitive(input_json, "username")->valuestring);
    strcpy(Value.firstName, cJSON_GetObjectItemCaseSensitive(input_json, "firstName")->valuestring);
    strcpy(Value.lastName, cJSON_GetObjectItemCaseSensitive(input_json, "lastName")->valuestring);
    strcpy(Value.email, cJSON_GetObjectItemCaseSensitive(input_json, "email")->valuestring);
    strcpy(Value.countryCode, cJSON_GetObjectItemCaseSensitive(input_json, "countryCode")->valuestring);
    strcpy(Value.created, cJSON_GetObjectItemCaseSensitive(input_json, "created")->valuestring);
    strcpy(Value.picture, cJSON_GetObjectItemCaseSensitive(input_json, "picture")->valuestring);
    strcpy(Value.gender, cJSON_GetObjectItemCaseSensitive(input_json, "gender")->valuestring);
    strcpy(Value.dateOfBirth, cJSON_GetObjectItemCaseSensitive(input_json, "dateOfBirth")->valuestring);
    return Value; /*return data structure*/
    /* always cleanup */
    cJSON_Delete(input_json);
  }
  else
  {
    printf("%s\n", "Request Error: cURL returned a 4xx status code. Authorization Error.");
    Value.status = -1;
    return Value;
  }
  /*Cleanup*/
  free(endpoint);
  free(baseparams);
  free(req.body);
}

items_model get_user_album(size_t userid)
{
  char *endpoint = url_cat("users/", userid, "/favorites/albums", 0);
  char *baseparams = param_cat("100", "", "");
  curl_model req = curl_get(endpoint, baseparams);
  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    return parse_items(input_json, 0, 1);
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
  free(req.body);
}

artist_model get_user_artist(size_t userid)
{
  char *endpoint = url_cat("users/", userid, "/favorites/artists", 0);
  char *baseparams = param_cat("100", "", "");
  curl_model req = curl_get(endpoint, baseparams);
  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    return parse_artist(input_json, 1);
    /* always cleanup */
    cJSON_Delete(input_json);
  }
  else
  {
    printf("%s\n", "Request Error: cURL returned a 4xx status code. Authorization Error.");
    artist_model Value;
    Value.status = -1;
    return Value;
  }
  /*Cleanup*/
  free(endpoint);
  free(baseparams);
  free(req.body);
}

playlist_model get_user_playlist(size_t userid)
{
  char *endpoint = url_cat("users/", userid, "/playlistsAndFavoritePlaylists", 0);
  char *baseparams = param_cat("50", "", ""); /* API Limit is 50 */
  curl_model req = curl_get(endpoint, baseparams);
  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    return parse_playlist(input_json, 2);
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
  free(req.body);
}

items_model get_user_tracks(size_t userid)
{
  char *endpoint = url_cat("users/", userid, "/favorites/tracks", 0);
  char *baseparams = param_cat("100", "", "");
  curl_model req = curl_get(endpoint, baseparams);
  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
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
  free(req.body);
}

items_model get_user_videos(size_t userid)
{
  char *endpoint = url_cat("users/", userid, "/favorites/videos", 0);
  char *baseparams = param_cat("100", "", "");
  curl_model req = curl_get(endpoint, baseparams);
  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    return parse_items(input_json, 2, 1);
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
  free(req.body);
}

/* Create/Manipulate/Delete favorites */

int add_user_album(size_t userid, size_t albumid)
{
  char *endpoint = url_cat("users/", userid, "/favorites/albums", 1);

  char buffer[60];
  snprintf(buffer, 60, "albumIds=%zu&onArtifactNotFound=FAIL", albumid);

  curl_model req = curl_post(endpoint, buffer, "");
  if (req.status != -1)
  {
    return 1;
  }
  else
  {
    return -1;
  }
  /*Cleanup*/
  free(endpoint);
  free(req.body);
}

int add_user_artist(size_t userid, size_t artistid)
{
  char *endpoint = url_cat("users/", userid, "/favorites/artists", 1);

  char buffer[60];
  snprintf(buffer, 60, "artistIds=%zu&onArtifactNotFound=FAIL", artistid);

  curl_model req = curl_post(endpoint, buffer, "");
  if (req.status != -1)
  {
    return 1;
  }
  else
  {
    return -1;
  }
  /*Cleanup*/
  free(endpoint);
  free(req.body);
}

int add_user_playlist(size_t userid, char *playlistid)
{
  char *endpoint = url_cat("users/", userid, "/favorites/playlists", 1);

  char *data = malloc(strlen(playlistid)+10+25+1);
  strcpy(data, "uuids=");
  strcat(data, playlistid);
  strcat(data, "&onArtifactNotFound=FAIL");

  curl_model req = curl_post(endpoint, data, "");
  if (req.status != -1)
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
  free(req.body);
}

int add_user_track(size_t userid, size_t trackid)
{
  char *endpoint = url_cat("users/", userid, "/favorites/tracks", 1);
  
  char buffer[60];
  snprintf(buffer, 60, "trackIds=%zu&onArtifactNotFound=FAIL", trackid);

  curl_model req = curl_post(endpoint, buffer, "");
  if (req.status != -1)
  {
    return 1;
  }
  else
  {
    return -1;
  }
  /*Cleanup*/
  free(endpoint);
  free(req.body);
}

int add_user_video(size_t userid, size_t videoid)
{
  char *endpoint = url_cat("users/", userid, "/favorites/videos", 1);

  char buffer[60];
  snprintf(buffer, 60, "videoIds=%zu&onArtifactNotFound=FAIL", videoid);
  
  curl_model req = curl_post(endpoint, buffer, "");
  if (req.status != -1)
  {
    return 1;
  }
  else
  {
    return -1;
  }
  /*Cleanup*/
  free(endpoint);
  free(req.body);
}

playlist_model create_user_playlist(size_t userid, char *title, char *description)
{
  playlist_model Value;
  char *endpoint = url_cat("users/", userid, "/playlists", 1);

  char *data = malloc(strlen(title)+strlen(description)+7+14+1);
  strcpy(data, "title=");
  strcat(data, title);
  strcat(data, "&description=");
  strcat(data, description);

  curl_model req = curl_post(endpoint, data, "");
  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
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
  free(data);
  free(req.body);
}

/* DELETE */

int delete_user_album(size_t userid, size_t albumid)
{
  char buffer[80];
  snprintf(buffer, 80, "users/%zu/favorites/albums/%zu?countryCode=%s", userid, albumid, countryCode); /* Similar to url_cat */
  
  curl_model req = curl_delete(buffer, "", "");
  if (req.status != -1)
  {
    return 1;
  }
  else
  {
    return -1;
  }
  /*Cleanup*/
  free(req.body);
}

int delete_user_artist(size_t userid, size_t artistid)
{
  char buffer[80];
  snprintf(buffer, 80, "users/%zu/favorites/artists/%zu?countryCode=%s", userid, artistid, countryCode);

  curl_model req = curl_delete(buffer, "", "");
  if (req.status != -1)
  {
    return 1;
  }
  else
  {
    return -1;
  }
  /*Cleanup*/
  free(req.body);
}

int delete_user_playlist(size_t userid, char *playlistid)
{
  char buffer[80];
  snprintf(buffer, 80, "users/%zu/favorites/playlists/%s?countryCode=%s", userid, playlistid, countryCode);

  curl_model req = curl_delete(buffer, "", "");
  if (req.status != -1)
  {
    return 1;
  }
  else
  {
    return -1;
  }
  /*Cleanup*/
  free(req.body);
}

int delete_user_track(size_t userid, size_t trackid)
{
  char buffer[80];
  snprintf(buffer, 80, "users/%zu/favorites/tracks/%zu?countryCode=%s", userid, trackid, countryCode);

  curl_model req = curl_delete(buffer, "", "");
  if (req.status != -1)
  {
    return 1;
  }
  else
  {
    return -1;
  }
  /*Cleanup*/
  free(req.body);
}

int delete_user_video(size_t userid, size_t videoid)
{
  char buffer[80];
  snprintf(buffer, 80, "users/%zu/favorites/videos/%zu?countryCode=%s", userid, videoid, countryCode);

  curl_model req = curl_delete(buffer, "", "");
  if (req.status != -1)
  {
    return 1;
  }
  else
  {
    return -1;
  }
  /*Cleanup*/
  free(req.body);
}
