#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/openTIDAL.h"

/* TODO: Remove email or check if != NULL & Improve Error Handling & Check if picture is != NULL */
user_model get_user(size_t userid)
{
  user_model Value;
  char *endpoint = url_cat("users/", userid, "", 0); /*concatenate endpoint url*/
  
  char baseparams[20];
  snprintf(baseparams, 20, "countryCode=%s", countryCode);

  curl_model req = curl_get(endpoint, baseparams); /*send request and pipe response into json parser*/
  free(endpoint);
  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    if (req.responseCode == 200)
    {
      cJSON *id = cJSON_GetObjectItemCaseSensitive(input_json, "id");
      cJSON *username = cJSON_GetObjectItemCaseSensitive(input_json, "username");
      cJSON *firstName = cJSON_GetObjectItemCaseSensitive(input_json, "firstName");
      cJSON *lastName = cJSON_GetObjectItemCaseSensitive(input_json, "lastName");
      cJSON *email = cJSON_GetObjectItemCaseSensitive(input_json, "email");
      cJSON *countryCode = cJSON_GetObjectItemCaseSensitive(input_json, "countryCode");
      cJSON *created = cJSON_GetObjectItemCaseSensitive(input_json, "created");
      cJSON *picture = cJSON_GetObjectItemCaseSensitive(input_json, "picture");
      cJSON *gender = cJSON_GetObjectItemCaseSensitive(input_json, "gender");
      cJSON *dateOfBirth = cJSON_GetObjectItemCaseSensitive(input_json, "dateOfBirth"); 
    
      Value.id = id->valueint;
      if (cJSON_IsNull(firstName) &&  cJSON_IsNull(lastName))
      {
        Value.status = -12;
      }
      else
      {
        Value.status = 1;
        strncpy(Value.username, username->valuestring, sizeof(Value.username));
        strncpy(Value.firstName, firstName->valuestring, sizeof(Value.firstName));
        strncpy(Value.lastName, lastName->valuestring, sizeof(Value.lastName));
        strncpy(Value.email, email->valuestring, sizeof(Value.email));
        strncpy(Value.countryCode, countryCode->valuestring, sizeof(Value.countryCode));
        strncpy(Value.created, created->valuestring, sizeof(Value.created));
        strncpy(Value.picture, picture->valuestring, sizeof(Value.picture));
        strncpy(Value.gender, gender->valuestring, sizeof(Value.gender));
        strncpy(Value.dateOfBirth, dateOfBirth->valuestring, sizeof(Value.dateOfBirth));
      }
      cJSON_Delete(input_json);
      free(req.body);
      return Value; /*return data structure*/
    }
    else if (req.responseCode == 400)
    {
      Value.status = parse_badrequest(input_json, userid, NULL);
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
    else if (req.responseCode == 401)
    {
      Value.status = parse_unauthorized(input_json, userid);
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
    else if (req.responseCode == 404)
    {
      Value.status = parse_notfound(input_json, userid, NULL);
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
    Value.status = -1;
    free(req.body);
    fprintf(stderr, "[Request Error] User %zu: CURLE_OK Check failed.", userid);
    return Value;
  }
}

album_model
get_user_album(size_t userid, size_t limit, size_t offset, char *order, char *orderDirection)
{
  album_model Value;
  char *endpoint = url_cat("users/", userid, "/favorites/albums", 0);
  
  char baseparams[150];
  snprintf(baseparams, 150, "countryCode=%s&limit=%zu&offset%zu&order%s&orderDirection=%s",
             countryCode, limit, offset, order, orderDirection);
  
  curl_model req = curl_get(endpoint, baseparams);
  free(endpoint);

  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    if (req.responseCode == 200)
    {
      album_model parse = parse_album(input_json);
      free(req.body);
      cJSON_Delete(input_json);
      return parse;
    }
    else if (req.responseCode == 400)
    {
      Value.status = parse_badrequest(input_json, userid, NULL);
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
    else if (req.responseCode == 401)
    {
      Value.status = parse_unauthorized(input_json, userid);
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
    else if (req.responseCode == 404)
    {
      Value.status = parse_notfound(input_json, userid, NULL);
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
    Value.status = -1;
    free(req.body);
    fprintf(stderr, "[Request Error] User %zu: CURLE_OK Check failed.", userid);
    return Value;
  }
}

artist_model
get_user_artist(size_t userid, size_t limit, size_t offset, char *order, char *orderDirection)
{
  artist_model Value;
  char *endpoint = url_cat("users/", userid, "/favorites/artists", 0);
  
  char baseparams[150];
  snprintf(baseparams, 150, "countryCode=%s&limit=%zu&offset%zu&order%s&orderDirection=%s",
             countryCode, limit, offset, order, orderDirection);

  curl_model req = curl_get(endpoint, baseparams);
  free(endpoint);
  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    if (req.responseCode == 200)
    {
      artist_model parse = parse_artist(input_json);
      cJSON_Delete(input_json);
      free(req.body);
      return parse;
    }
    else if (req.responseCode == 400)
    {
      Value.status = parse_badrequest(input_json, userid, NULL);
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
    else if (req.responseCode == 401)
    {
      Value.status = parse_unauthorized(input_json, userid);
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
    else if (req.responseCode == 404)
    {
      Value.status = parse_notfound(input_json, userid, NULL);
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
    Value.status = -1;
    free(req.body);
    fprintf(stderr, "[Request Error] User %zu: CURLE_OK Check failed.", userid);
    return Value;
  }
}

playlist_model
get_user_playlist(size_t userid, size_t limit, size_t offset, char *order, char *orderDirection)
{
  playlist_model Value;
  char *endpoint = url_cat("users/", userid, "/playlistsAndFavoritePlaylists", 0);
  
  char baseparams[150];
  snprintf(baseparams, 150, "countryCode=%s&limit=%zu&offset%zu&order%s&orderDirection=%s",
             countryCode, limit, offset, order, orderDirection);

  curl_model req = curl_get(endpoint, baseparams);
  free(endpoint);
  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    if (req.responseCode == 200)
    {
      playlist_model parse = parse_playlist(input_json);
      cJSON_Delete(input_json);
      free(req.body);
      return parse;
    }
    else if (req.responseCode == 400)
    {
      Value.status = parse_badrequest(input_json, userid, NULL);
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
    else if (req.responseCode == 401)
    {
      Value.status = parse_unauthorized(input_json, userid);
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
    else if (req.responseCode == 404)
    {
      Value.status = parse_notfound(input_json, userid, NULL);
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
    Value.status = -1;
    free(req.body);
    fprintf(stderr, "[Request Error] User %zu: CURLE_OK Check failed.", userid);
    return Value;
  }
}

items_model
get_user_tracks(size_t userid, size_t limit, size_t offset, char *order, char *orderDirection)
{
  items_model Value;
  char *endpoint = url_cat("users/", userid, "/favorites/tracks", 0);
  
  char baseparams[150];
  snprintf(baseparams, 150, "countryCode=%s&limit=%zu&offset%zu&order%s&orderDirection=%s",
             countryCode, limit, offset, order, orderDirection);

  curl_model req = curl_get(endpoint, baseparams);
  free(endpoint);
  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    if (req.responseCode == 200)
    {
      items_model parse = parse_items(input_json);
      free(req.body);
      cJSON_Delete(input_json);
      return parse;
    }
    else if (req.responseCode == 400)
    {
      Value.status = parse_badrequest(input_json, userid, NULL);
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
    else if (req.responseCode == 401)
    {
      Value.status = parse_unauthorized(input_json, userid);
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
    else if (req.responseCode == 404)
    {
      Value.status = parse_notfound(input_json, userid, NULL);
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
    Value.status = -1;
    free(req.body);
    fprintf(stderr, "[Request Error] User %zu: CURLE_OK Check failed.", userid);
    return Value;
  }
}

items_model
get_user_videos(size_t userid, size_t limit, size_t offset, char *order, char *orderDirection)
{
  items_model Value;
  char *endpoint = url_cat("users/", userid, "/favorites/videos", 0);
  
  char baseparams[150];
  snprintf(baseparams, 150, "countryCode=%s&limit=%zu&offset%zu&order%s&orderDirection=%s",
             countryCode, limit, offset, order, orderDirection);
  
  curl_model req = curl_get(endpoint, baseparams);
  free(endpoint);
  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    if (req.responseCode == 200)
    {
      items_model parse = parse_items(input_json);
      free(req.body);
      cJSON_Delete(input_json);
      return parse;
    }
    else if (req.responseCode == 400)
    {
      Value.status = parse_badrequest(input_json, userid, NULL);
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
    else if (req.responseCode == 401)
    {
      Value.status = parse_unauthorized(input_json, userid);
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
    else if (req.responseCode == 404)
    {
      Value.status = parse_notfound(input_json, userid, NULL);
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
    Value.status = -1;
    free(req.body);
    fprintf(stderr, "[Request Error] User %zu: CURLE_OK Check failed.", userid);
    return Value;
  }
}

page_mix_model get_user_mixes()
{
  page_mix_model Value;
  char *endpoint = "pages/my_collection_my_mixes";
  
  char baseparams[40];
  snprintf(baseparams, 40, "countryCode=%s&deviceType=BROWSER", countryCode);
  
  curl_model req = curl_get(endpoint, baseparams);

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
    else if (req.responseCode == 400)
    {
      Value.status = parse_badrequest(input_json, 0, "mymixes");
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

int add_user_artist(size_t userid, size_t artistid)
{
  char *endpoint = url_cat("users/", userid, "/favorites/artists", 1);
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

int add_user_playlist(size_t userid, char *playlistid)
{
  char *endpoint = url_cat("users/", userid, "/favorites/playlists", 1);
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

int add_user_track(size_t userid, size_t trackid)
{
  char *endpoint = url_cat("users/", userid, "/favorites/tracks", 1);
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

int add_user_video(size_t userid, size_t videoid)
{
  char *endpoint = url_cat("users/", userid, "/favorites/videos", 1);
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
    if (req.responseCode == 201 || req.responseCode == 200)
    {
      playlist_model parse = parse_playlist(input_json);
      free(req.body);
      cJSON_Delete(input_json);
      return parse;
    }
    else if (req.responseCode == 400)
    {
      Value.status = parse_badrequest(input_json, userid, NULL);
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
      Value.status = parse_notfound(input_json, userid, NULL);
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
    playlist_model Value;
    Value.status = -1;
    free(req.body);
    return Value;
  }
}

/* DELETE */

int delete_user_album(size_t userid, size_t albumid)
{
  int status;
  char buffer[80];
  snprintf(buffer, 80, "users/%zu/favorites/albums/%zu?countryCode=%s", userid,
            albumid, countryCode);
  
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

int delete_user_artist(size_t userid, size_t artistid)
{
  int status;
  char buffer[80];
  snprintf(buffer, 80, "users/%zu/favorites/artists/%zu?countryCode=%s", userid, artistid, countryCode);

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

int delete_user_playlist(size_t userid, char *playlistid)
{
  int status;
  char buffer[80];
  snprintf(buffer, 80, "users/%zu/favorites/playlists/%s?countryCode=%s", userid, playlistid, countryCode);

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

int delete_user_track(size_t userid, size_t trackid)
{
  int status;
  char buffer[80];
  snprintf(buffer, 80, "users/%zu/favorites/tracks/%zu?countryCode=%s", userid, trackid, countryCode);

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

int delete_user_video(size_t userid, size_t videoid)
{
  int status;
  char buffer[80];
  snprintf(buffer, 80, "users/%zu/favorites/videos/%zu?countryCode=%s", userid, videoid, countryCode);

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
