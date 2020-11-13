#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/parse.h"
#include "../include/openTIDAL.h"

search_model parse_search(cJSON *input_json)
{
  search_model Value;
  size_t ar = 0; /* artistCounter  */
  size_t al = 0; /* albumsCounter  */
  size_t pl = 0; /* playlistsCounter */
  size_t tr = 0; /* tracksCounter  */
  size_t vi = 0; /* videosCounter  */

  /* artist objects */
  cJSON *artists = cJSON_GetObjectItem(input_json, "artists");
  cJSON *artistsLimit = cJSON_GetObjectItem(artists, "limit");
  cJSON *artistsOffset = cJSON_GetObjectItem(artists, "offset");
  cJSON *artistsTotalNumberOfItems = cJSON_GetObjectItem(artists, "totalNumberOfItems");
  cJSON *artistsItems = cJSON_GetObjectItem(artists, "items");
  cJSON *artistsItem = NULL;
  
  /* album objects */
  cJSON *albums = cJSON_GetObjectItem(input_json, "albums");
  cJSON *albumsLimit = cJSON_GetObjectItem(albums, "limit");
  cJSON *albumsOffset = cJSON_GetObjectItem(albums, "offset");
  cJSON *albumsTotalNumberOfItems = cJSON_GetObjectItem(albums, "totalNumberOfItems");
  cJSON *albumsItems = cJSON_GetObjectItem(albums, "items");
  cJSON *albumsItem = NULL;
  
  /* playlist objects */
  cJSON *playlists = cJSON_GetObjectItem(input_json, "playlists");
  cJSON *playlistsLimit = cJSON_GetObjectItem(playlists, "limit");
  cJSON *playlistsOffset = cJSON_GetObjectItem(playlists, "offset");
  cJSON *playlistsTotalNumberOfItems = cJSON_GetObjectItem(playlists, "totalNumberOfItems");
  cJSON *playlistsItems = cJSON_GetObjectItem(playlists, "items");
  cJSON *playlistsItem = NULL;
 
  /* track objects */
  cJSON *tracks = cJSON_GetObjectItem(input_json, "tracks");
  cJSON *tracksLimit = cJSON_GetObjectItem(tracks, "limit");
  cJSON *tracksOffset = cJSON_GetObjectItem(tracks, "offset");
  cJSON *tracksTotalNumberOfItems = cJSON_GetObjectItem(tracks, "totalNumberOfItems");
  cJSON *tracksItems = cJSON_GetObjectItem(tracks, "items");
  cJSON *tracksItem = NULL;
  
  /* video objects  */ 
  cJSON *videos = cJSON_GetObjectItem(input_json, "videos");
  cJSON *videosLimit = cJSON_GetObjectItem(videos, "limit");
  cJSON *videosOffset = cJSON_GetObjectItem(videos, "offset");
  cJSON *videosTotalNumberOfItems = cJSON_GetObjectItem(videos, "totalNumberOfItems");
  cJSON *videosItems = cJSON_GetObjectItem(videos, "items");
  cJSON *videosItem = NULL;
  
  /* topHit objects  */
  cJSON *topHit = cJSON_GetObjectItem(input_json, "topHit");
  cJSON *topHitValue = cJSON_GetObjectItem(topHit, "value");
  cJSON *topHitType = cJSON_GetObjectItemCaseSensitive(topHit, "type");

  /* artists */
  if (cJSON_IsArray(artistsItems))
  {
    cJSON_ArrayForEach(artistsItem, artistsItems)
    {
      json_artist_model processed_json = json_parse_artist(artistsItem);
      Value.artists = parse_artist_values(processed_json, ar);
      ar += 1;
    }
  }
      
  parse_number(artistsLimit, &Value.artists.limit);
  parse_number(artistsOffset, &Value.artists.offset);
  parse_number(artistsTotalNumberOfItems, &Value.artists.totalNumberOfItems);
  Value.artists.arraySize = cJSON_GetArraySize(artistsItems);
  Value.artists.status = 1;
  
  /* albums */
  if (cJSON_IsArray(albumsItems))
  {
    cJSON_ArrayForEach(albumsItem, albumsItems)
    {
      json_album_model processed_json = json_parse_album(albumsItem);
      Value.albums = parse_album_values(processed_json, al);
      al += 1;
    }
  }
      
  parse_number(albumsLimit, &Value.albums.limit);
  parse_number(albumsOffset, &Value.albums.offset);
  parse_number(albumsTotalNumberOfItems, &Value.albums.totalNumberOfItems);
  Value.albums.arraySize = cJSON_GetArraySize(albumsItems);
  Value.albums.status = 1;

  /* playlists */
  if (cJSON_IsArray(playlistsItems))
  {
    cJSON_ArrayForEach(playlistsItem, playlistsItems)
    {
      json_playlist_model processed_json = json_parse_playlist(playlistsItem);
      Value.playlists = parse_playlist_values(processed_json, pl);
      pl += 1;
    }
  }
      
  parse_number(playlistsLimit, &Value.playlists.limit);
  parse_number(playlistsOffset, &Value.playlists.offset);
  parse_number(playlistsTotalNumberOfItems, &Value.playlists.totalNumberOfItems);
  Value.playlists.arraySize = cJSON_GetArraySize(playlistsItems);
  Value.playlists.status = 1;

  /* tracks */
  if (cJSON_IsArray(tracksItems))
  {
    cJSON_ArrayForEach(tracksItem, tracksItems)
    {
      json_items_model processed_json = json_parse_items(tracksItem);
      Value.tracks = parse_items_values(processed_json, tr);
      tr += 1;
    }
  }
      
  parse_number(tracksLimit, &Value.tracks.limit);
  parse_number(tracksOffset, &Value.tracks.offset);
  parse_number(tracksTotalNumberOfItems, &Value.tracks.totalNumberOfItems);
  Value.tracks.arraySize = cJSON_GetArraySize(tracksItems);
  Value.tracks.status = 1;

  /* videos */
  if (cJSON_IsArray(videosItems))
  {
    cJSON_ArrayForEach(videosItem, videosItems)
    {
      json_items_model processed_json = json_parse_items(videosItem);
      Value.videos = parse_items_values(processed_json, vi);
      vi += 1;
    }
  }
      
  parse_number(videosLimit, &Value.videos.limit);
  parse_number(videosOffset, &Value.videos.offset);
  parse_number(videosTotalNumberOfItems, &Value.videos.totalNumberOfItems);
  Value.videos.arraySize = cJSON_GetArraySize(videosItems);
  Value.videos.status = 1;

  /* topHit */
  if (cJSON_IsString(topHitType) && (!cJSON_IsNull(topHitType)))
  {
    strncpy(Value.topHitType, topHitType->valuestring, sizeof(Value.topHitType));
  }
  else
  {
    strcpy(Value.topHitType, "\0");
  }

  if (strcmp(Value.topHitType, "ARTISTS") == 0)
  {
    json_artist_model processed_json = json_parse_artist(topHitValue);
    Value.topArtist = parse_artist_values(processed_json, 0);
    Value.topArtist.status = 1;
    Value.topArtist.arraySize = 1;
  }
  else if (strcmp(Value.topHitType, "ALBUMS") == 0)
  {
    json_album_model processed_json = json_parse_album(topHitValue);
    Value.topAlbum = parse_album_values(processed_json, 0);
    Value.topAlbum.status = 1;
    Value.topAlbum.arraySize = 1;
  }
  else if (strcmp(Value.topHitType, "PLAYLISTS") == 0)
  {
    json_playlist_model processed_json = json_parse_playlist(topHitValue);
    Value.topPlaylist = parse_playlist_values(processed_json, 0);
    Value.topPlaylist.status = 1;
    Value.topPlaylist.arraySize = 1;
  }
  else if (strcmp(Value.topHitType, "TRACKS") == 0)
  {
    json_items_model processed_json = json_parse_items(topHitValue);
    Value.topTrack = parse_items_values(processed_json, 0);
    Value.topTrack.status = 1;
    Value.topTrack.arraySize = 1;
  }
  else if (strcmp(Value.topHitType, "VIDEOS") == 0)
  {
    json_items_model processed_json = json_parse_items(topHitValue);
    Value.topVideo = parse_items_values(processed_json, 0);
    Value.topVideo.status = 1;
    Value.topVideo.arraySize = 1;
  }
  
  Value.status = 1;

  return Value;
}
