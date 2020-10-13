#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/openTIDAL.h"

user_model get_user(size_t userid) /* TODO: Remove email or check if != NULL  */
{
  user_model Value;
  char *endpoint = url_cat("users/", userid, "", 0); /*concatenate endpoint url*/
  char *baseparams = param_cat("", "", "");
  curl_model req = curl_get(endpoint, baseparams); /*send request and pipe response into json parser*/
  free(endpoint);
  free(baseparams);
  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    Value.status = 1;
    Value.id = cJSON_GetObjectItemCaseSensitive(input_json, "id")->valueint; /*parse JSON objects into data structure*/
    strncpy(Value.username, cJSON_GetObjectItemCaseSensitive(input_json, "username")->valuestring, sizeof(Value.username));
    strncpy(Value.firstName, cJSON_GetObjectItemCaseSensitive(input_json, "firstName")->valuestring, sizeof(Value.firstName));
    strncpy(Value.lastName, cJSON_GetObjectItemCaseSensitive(input_json, "lastName")->valuestring, sizeof(Value.lastName));
    //strncpy(Value.email, cJSON_GetObjectItemCaseSensitive(input_json, "email")->valuestring, sizeof(Value.email));
    strncpy(Value.countryCode, cJSON_GetObjectItemCaseSensitive(input_json, "countryCode")->valuestring, sizeof(Value.countryCode));
    strncpy(Value.created, cJSON_GetObjectItemCaseSensitive(input_json, "created")->valuestring, sizeof(Value.created));
    strncpy(Value.picture, cJSON_GetObjectItemCaseSensitive(input_json, "picture")->valuestring, sizeof(Value.picture));
    strncpy(Value.gender, cJSON_GetObjectItemCaseSensitive(input_json, "gender")->valuestring, sizeof(Value.gender));
    strncpy(Value.dateOfBirth, cJSON_GetObjectItemCaseSensitive(input_json, "dateOfBirth")->valuestring, sizeof(Value.dateOfBirth));
    
    cJSON_Delete(input_json);
    free(req.body);
    return Value; /*return data structure*/
  }
  else
  {
    printf("%s\n", "Request Error: cURL returned a 4xx status code. Authorization Error.");
    Value.status = -1;
    free(req.body);
    return Value;
  }
}

items_model get_user_album(size_t userid)
{
  char *endpoint = url_cat("users/", userid, "/favorites/albums", 0);
  char *baseparams = param_cat("100", "", "");
  curl_model req = curl_get(endpoint, baseparams);
  free(endpoint);
  free(baseparams);

  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    items_model parse = parse_items(input_json, 0, 1);
    free(req.body);
    cJSON_Delete(input_json);
    return parse;
  }
  else
  {
    printf("%s\n", "Request Error: cURL returned a 4xx status code. Authorization Error.");
    items_model Value;
    Value.status = -1;
    free(req.body);
    return Value;
  }
}

artist_model get_user_artist(size_t userid)
{
  char *endpoint = url_cat("users/", userid, "/favorites/artists", 0);
  char *baseparams = param_cat("100", "", "");
  curl_model req = curl_get(endpoint, baseparams);
  free(endpoint);
  free(baseparams);
  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    artist_model parse = parse_artist(input_json, 1);
    cJSON_Delete(input_json);
    free(req.body);
    return parse;
  }
  else
  {
    printf("%s\n", "Request Error: cURL returned a 4xx status code. Authorization Error.");
    artist_model Value;
    free(req.body);
    Value.status = -1;
    return Value;
  }
}

playlist_model get_user_playlist(size_t userid)
{
  char *endpoint = url_cat("users/", userid, "/playlistsAndFavoritePlaylists", 0);
  char *baseparams = param_cat("50", "", ""); /* API Limit is 50 */
  curl_model req = curl_get(endpoint, baseparams);
  free(endpoint);
  free(baseparams);
  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    playlist_model parse = parse_playlist(input_json, 2);
    cJSON_Delete(input_json);
    free(req.body);
    return parse;
  }
else
  {
    printf("%s\n", "Request Error: cURL returned a 4xx status code. Authorization Error.");
    playlist_model Value;
    Value.status = -1;
    free(req.body);
    return Value;
  }
}

items_model get_user_tracks(size_t userid)
{
  char *endpoint = url_cat("users/", userid, "/favorites/tracks", 0);
  char *baseparams = param_cat("100", "", "");
  curl_model req = curl_get(endpoint, baseparams);
  free(endpoint);
  free(baseparams);
  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    items_model parse = parse_items(input_json, 2, 0);
    free(req.body);
    cJSON_Delete(input_json);
    return parse;
  }
  else
  {
    printf("%s\n", "Request Error: cURL returned a 4xx status code. Authorization Error.");
    items_model Value;
    Value.status = -1;
    free(req.body);
    return Value;
  }
}

items_model get_user_videos(size_t userid)
{
  char *endpoint = url_cat("users/", userid, "/favorites/videos", 0);
  char *baseparams = param_cat("100", "", "");
  curl_model req = curl_get(endpoint, baseparams);
  free(endpoint);
  free(baseparams);
  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    items_model parse = parse_items(input_json, 2, 1);
    free(req.body);
    cJSON_Delete(input_json);
    return parse;
  }
  else
  {
    printf("%s\n", "Request Error: cURL returned a 4xx status code. Authorization Error.");
    items_model Value;
    Value.status = -1;
    free(req.body);
    return Value;
  }
}

page_mix_model get_user_mixes()
{
  page_mix_model Value;
  char *endpoint = "pages/my_collection_my_mixes";
  char *baseparams = param_cat("100", "", "deviceType=BROWSER");
  curl_model req = curl_get(endpoint, baseparams);
  free(baseparams);

  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    if (req.responseCode == 200)
    {
      cJSON *rows = cJSON_GetObjectItem(input_json, "rows");
      cJSON *rowsArray = cJSON_GetArrayItem(rows, 0);
      cJSON *modules = cJSON_GetObjectItem(rowsArray, "modules");
      cJSON *modulesArray = cJSON_GetArrayItem(modules, 0);
      cJSON *type = cJSON_GetObjectItemCaseSensitive(modulesArray, "type");
      cJSON *pagedList = cJSON_GetObjectItem(modulesArray, "pagedList");
      cJSON *limit = cJSON_GetObjectItem(pagedList, "limit");
      cJSON *offset = cJSON_GetObjectItem(pagedList, "offset");
      cJSON *totalNumberOfItems = cJSON_GetObjectItem(pagedList, "totalNumberOfItems");
      cJSON *items = cJSON_GetObjectItem(pagedList, "items");
      cJSON *item = NULL;
      int i;
      
      Value.status = 1;
      Value.arraySize = cJSON_GetArraySize(items);
      Value.limit = limit->valueint;
      Value.offset = offset->valueint;
      Value.totalNumberOfItems = totalNumberOfItems->valueint;
      cJSON_ArrayForEach(item, items)
      {
        cJSON *id = cJSON_GetObjectItemCaseSensitive(item, "id");
	cJSON *title = cJSON_GetObjectItemCaseSensitive(item, "title");
	cJSON *subTitle = cJSON_GetObjectItemCaseSensitive(item, "subTitle");
	cJSON *images = cJSON_GetObjectItem(item, "images");
	cJSON *small = cJSON_GetObjectItem(images, "SMALL");
	cJSON *medium = cJSON_GetObjectItem(images, "MEDIUM");
	cJSON *large = cJSON_GetObjectItem(images, "LARGE");
        cJSON *smallImageUrl = cJSON_GetObjectItemCaseSensitive(small, "url");
        cJSON *mediumImageUrl = cJSON_GetObjectItemCaseSensitive(medium, "url");
        cJSON *largeImageUrl = cJSON_GetObjectItemCaseSensitive(large, "url");
        
	strncpy(Value.id[i], id->valuestring, sizeof(Value.id[i]));
	strncpy(Value.title[i], title->valuestring, sizeof(Value.title[i]));
	strncpy(Value.subTitle[i], subTitle->valuestring, sizeof(Value.subTitle[i]));
	strncpy(Value.smallImageUrl[i], smallImageUrl->valuestring, sizeof(Value.smallImageUrl[i]));
	strncpy(Value.mediumImageUrl[i], mediumImageUrl->valuestring, sizeof(Value.mediumImageUrl[i]));
	strncpy(Value.largeImageUrl[i], largeImageUrl->valuestring, sizeof(Value.largeImageUrl[i]));
	i += 1;
      }
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
    else if (req.responseCode == 401)
    {
      Value.status = parse_unauthorized(input_json, 0);
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
    else if (req.responseCode == 404)
    {
      Value.status = parse_notfound(input_json, 0, "mymixes");
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
    else
    {
      Value.status = 0;
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
  }
  else
  {
    printf("%s\n", "Request Error: cURL returned a 4xx status code. Authorization Error.");
    Value.status = -1;
    free(req.body);
    return Value;
  }
}

/* Create/Manipulate/Delete favorites */

int add_user_album(size_t userid, size_t albumid)
{
  char *endpoint = url_cat("users/", userid, "/favorites/albums", 1);

  char buffer[60];
  snprintf(buffer, 60, "albumIds=%zu&onArtifactNotFound=FAIL", albumid);

  curl_model req = curl_post(endpoint, buffer, "");
  free(endpoint);
  free(req.body);
  if (req.status != -1)
  {
    return 1;
  }
  else
  {
    return -1;
  }
}

int add_user_artist(size_t userid, size_t artistid)
{
  char *endpoint = url_cat("users/", userid, "/favorites/artists", 1);

  char buffer[60];
  snprintf(buffer, 60, "artistIds=%zu&onArtifactNotFound=FAIL", artistid);

  curl_model req = curl_post(endpoint, buffer, "");
  free(endpoint);
  free(req.body);
  if (req.status != -1)
  {
    return 1;
  }
  else
  {
    return -1;
  }
}

int add_user_playlist(size_t userid, char *playlistid)
{
  char *endpoint = url_cat("users/", userid, "/favorites/playlists", 1);

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
    return 1;
  }
  else
  {
    return -1;
  }
}

int add_user_track(size_t userid, size_t trackid)
{
  char *endpoint = url_cat("users/", userid, "/favorites/tracks", 1);
  
  char buffer[60];
  snprintf(buffer, 60, "trackIds=%zu&onArtifactNotFound=FAIL", trackid);

  curl_model req = curl_post(endpoint, buffer, "");
  free(endpoint);
  free(req.body);
  if (req.status != -1)
  {
    return 1;
  }
  else
  {
    return -1;
  }
}

int add_user_video(size_t userid, size_t videoid)
{
  char *endpoint = url_cat("users/", userid, "/favorites/videos", 1);

  char buffer[60];
  snprintf(buffer, 60, "videoIds=%zu&onArtifactNotFound=FAIL", videoid);
  
  curl_model req = curl_post(endpoint, buffer, "");
  /*Cleanup*/
  free(endpoint);
  free(req.body);

  if (req.status != -1)
  {
    return 1;
  }
  else
  {
    return -1;
  }
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
  /*Cleanup*/
  free(endpoint);
  free(data);

  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    playlist_model parse = parse_playlist(input_json, 0);
    free(req.body);
    cJSON_Delete(input_json);
    return parse;
  }
  else
  {
    printf("%s\n", "Request Error: cURL returned a 4xx status code. Authorization Error.");
    playlist_model Value;
    Value.status = -1;
    free(req.body);
    return Value;
  }
}

/* DELETE */

int delete_user_album(size_t userid, size_t albumid)
{
  char buffer[80];
  snprintf(buffer, 80, "users/%zu/favorites/albums/%zu?countryCode=%s", userid, albumid, countryCode); /* Similar to url_cat */
  
  curl_model req = curl_delete(buffer, "", "");
  /*Cleanup*/
  free(req.body);

  if (req.status != -1)
  {
    return 1;
  }
  else
  {
    return -1;
  }
}

int delete_user_artist(size_t userid, size_t artistid)
{
  char buffer[80];
  snprintf(buffer, 80, "users/%zu/favorites/artists/%zu?countryCode=%s", userid, artistid, countryCode);

  curl_model req = curl_delete(buffer, "", "");
  /*Cleanup*/
  free(req.body);

  if (req.status != -1)
  {
    return 1;
  }
  else
  {
    return -1;
  }
}

int delete_user_playlist(size_t userid, char *playlistid)
{
  char buffer[80];
  snprintf(buffer, 80, "users/%zu/favorites/playlists/%s?countryCode=%s", userid, playlistid, countryCode);

  curl_model req = curl_delete(buffer, "", "");
  /*Cleanup*/
  free(req.body);

  if (req.status != -1)
  {
    return 1;
  }
  else
  {
    return -1;
  }
}

int delete_user_track(size_t userid, size_t trackid)
{
  char buffer[80];
  snprintf(buffer, 80, "users/%zu/favorites/tracks/%zu?countryCode=%s", userid, trackid, countryCode);

  curl_model req = curl_delete(buffer, "", "");
  /*Cleanup*/
  free(req.body);

  if (req.status != -1)
  {
    return 1;
  }
  else
  {
    return -1;
  }
}

int delete_user_video(size_t userid, size_t videoid)
{
  char buffer[80];
  snprintf(buffer, 80, "users/%zu/favorites/videos/%zu?countryCode=%s", userid, videoid, countryCode);

  curl_model req = curl_delete(buffer, "", "");
  /*Cleanup*/
  free(req.body);

  if (req.status != -1)
  {
    return 1;
  }
  else
  {
    return -1;
  }
}
