#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/cTidal.h"

cJSON *json_parse(const char * input)
{
  cJSON *input_json = cJSON_Parse(input);
  int status = 0;
  if (input_json == NULL)
  {
      const char *error_ptr = cJSON_GetErrorPtr();
      if (error_ptr != NULL)
      {
          /*JSON Input empty*/
          fprintf(stderr, "Error before: %s\n", error_ptr);
      }
      status = 0;
  }
  return input_json;
}

items_model parse_tracks(cJSON *input_json) /* TODO: Add bool types & replayGain/peak  */
{
  items_model Value;
  const cJSON *album = NULL;
  const cJSON *artistItems = NULL;
  const cJSON *artistItem = NULL;
  int artistCounter = 0;

  Value.id[0] = cJSON_GetObjectItem(input_json, "id")->valueint;
  strcpy(Value.title[0], cJSON_GetObjectItemCaseSensitive(input_json, "title")->valuestring);
  Value.duration[0] = cJSON_GetObjectItem(input_json, "duration")->valueint;
  Value.popularity[0] = cJSON_GetObjectItem(input_json, "popularity")->valueint;
  Value.trackNumber[0] = cJSON_GetObjectItem(input_json, "trackNumber")->valueint;
  Value.volumeNumber[0] = cJSON_GetObjectItem(input_json, "volumeNumber")->valueint;
  strcpy(Value.version[0], cJSON_GetObjectItemCaseSensitive(input_json, "version")->valuestring);
  artistItems = cJSON_GetObjectItemCaseSensitive(input_json, "artists");
  Value.subArraySize[0] = cJSON_GetArraySize(artistItems);
  cJSON_ArrayForEach(artistItem, artistItems)
  {
    Value.artistId[0][artistCounter] = cJSON_GetObjectItem(artistItem, "id")->valueint;
    strcpy(Value.artistName[0][artistCounter], cJSON_GetObjectItemCaseSensitive(artistItem, "name")->valuestring);
    artistCounter = artistCounter + 1;
  }
  album = cJSON_GetObjectItemCaseSensitive(input_json, "album");
  Value.albumId[0] = cJSON_GetObjectItem(album, "id")->valueint;
  strcpy(Value.albumTitle[0], cJSON_GetObjectItemCaseSensitive(album, "title")->valuestring);
  strcpy(Value.albumTitle[0], cJSON_GetObjectItemCaseSensitive(album, "title")->valuestring);
  strcpy(Value.cover[0], cJSON_GetObjectItemCaseSensitive(album, "cover")->valuestring);

  return Value;
}

items_model parse_items(cJSON *input_json, int version, int video) /* TODO: Add bool types */
{
  items_model Value;
  const cJSON *versionJSON = NULL;
  const cJSON *item = NULL;
  const cJSON *items = NULL;
  const cJSON *artist = NULL;
  const cJSON *album = NULL;
  
  const cJSON *artistItems = NULL;
  const cJSON *artistItem = NULL;
  
  int i = 0;

  items = cJSON_GetObjectItemCaseSensitive(input_json, "items");
  Value.arraySize = cJSON_GetArraySize(items);
  Value.offset = cJSON_GetObjectItemCaseSensitive(input_json, "offset")->valueint;
  Value.totalNumberOfItems = cJSON_GetObjectItemCaseSensitive(input_json, "totalNumberOfItems")->valueint;
  if (Value.totalNumberOfItems != 0)
  {
    cJSON_ArrayForEach(item, items)
    {
      int artistCounter = 0;
      if (version == 1) /* Sometimes the JSON response varies */
      {
      versionJSON = item;
      }
      else
      {
        versionJSON = cJSON_GetObjectItemCaseSensitive(item, "item");
      }

      Value.id[i] = cJSON_GetObjectItemCaseSensitive(versionJSON, "id")->valueint;
      strcpy(Value.title[i], cJSON_GetObjectItemCaseSensitive(versionJSON, "title")->valuestring);
      Value.duration[i] = cJSON_GetObjectItemCaseSensitive(versionJSON, "duration")->valueint;
      Value.popularity[i] = cJSON_GetObjectItemCaseSensitive(versionJSON, "popularity")->valueint;

      artistItems = cJSON_GetObjectItemCaseSensitive(versionJSON, "artists");
      Value.subArraySize[i] = cJSON_GetArraySize(artistItems);
      
      cJSON_ArrayForEach(artistItem, artistItems)
      {
        Value.artistId[i][artistCounter] = cJSON_GetObjectItemCaseSensitive(artistItem, "id")->valueint;
        strcpy(Value.artistName[i][artistCounter], cJSON_GetObjectItemCaseSensitive(artistItem, "name")->valuestring);
        artistCounter = artistCounter + 1;
      } 

      if (video == 0) /* Videos don't have an album section */
        {
          album = cJSON_GetObjectItemCaseSensitive(versionJSON, "album");
          Value.albumId[i] = cJSON_GetObjectItemCaseSensitive(album, "id")->valueint;
	  strcpy(Value.albumTitle[i], cJSON_GetObjectItemCaseSensitive(album, "title")->valuestring);
          strcpy(Value.albumTitle[i], cJSON_GetObjectItemCaseSensitive(album, "title")->valuestring);
          strcpy(Value.cover[i], cJSON_GetObjectItemCaseSensitive(album, "cover")->valuestring);
        }
      i = i + 1;
    }
  }
  else
  {
    printf("%s\n", "No Items in Response");
  }
   return Value;
}

playlist_model parse_playlist(cJSON *input_json, int version)
{
  const cJSON *item = NULL;
  const cJSON *items = NULL;
  const cJSON *playlist = NULL;
  int i = 0;
  playlist_model Value;
  if (version == 0)
  {
     Value.totalNumberOfItems = 1; /* Quickhack to avoid a Seg Fault (totalNumberOfItems not always present)  */
  }
  else
  {
     Value.totalNumberOfItems = cJSON_GetObjectItemCaseSensitive(input_json, "totalNumberOfItems")->valueint;
  }
  if (Value.totalNumberOfItems != 0)
  {
    if (version == 1)
    {
      items = cJSON_GetObjectItemCaseSensitive(input_json, "items");
      Value.arraySize = cJSON_GetArraySize(items);

      cJSON_ArrayForEach(item, items)
      {
        strcpy(Value.uuid[i], cJSON_GetObjectItemCaseSensitive(item, "uuid")->valuestring);
        strcpy(Value.title[i], cJSON_GetObjectItemCaseSensitive(item, "title")->valuestring);
        strcpy(Value.lastUpdated[i], cJSON_GetObjectItemCaseSensitive(item, "lastUpdated")->valuestring);
        strcpy(Value.created[i], cJSON_GetObjectItemCaseSensitive(item, "created")->valuestring);
        strcpy(Value.image[i], cJSON_GetObjectItemCaseSensitive(item, "image")->valuestring);
        strcpy(Value.squareImage[i], cJSON_GetObjectItemCaseSensitive(item, "squareImage")->valuestring);
        Value.numberOfTracks[i] = cJSON_GetObjectItemCaseSensitive(item, "numberOfTracks")->valueint;
        Value.numberOfVideos[i] = cJSON_GetObjectItemCaseSensitive(item, "numberOfVideos")->valueint;
        Value.duration[i] = cJSON_GetObjectItemCaseSensitive(item, "duration")->valueint;

        cJSON *description = cJSON_GetObjectItemCaseSensitive(item, "description");
        if (cJSON_IsNull(description) != 1)
        {
          strcpy(Value.description[i], description->valuestring);
        }
        i = i + 1;
      }
    }
    else if (version == 2)
    {
      items = cJSON_GetObjectItemCaseSensitive(input_json, "items");
      Value.arraySize = cJSON_GetArraySize(items);

      cJSON_ArrayForEach(item, items)
      {
        playlist = cJSON_GetObjectItemCaseSensitive(item, "playlist");
        strcpy(Value.uuid[i], cJSON_GetObjectItemCaseSensitive(playlist, "uuid")->valuestring);
        strcpy(Value.title[i], cJSON_GetObjectItemCaseSensitive(playlist, "title")->valuestring);
        strcpy(Value.lastUpdated[i], cJSON_GetObjectItemCaseSensitive(playlist, "lastUpdated")->valuestring);
        strcpy(Value.created[i], cJSON_GetObjectItemCaseSensitive(playlist, "created")->valuestring);
        strcpy(Value.image[i], cJSON_GetObjectItemCaseSensitive(playlist, "image")->valuestring);
        strcpy(Value.squareImage[i], cJSON_GetObjectItemCaseSensitive(playlist, "squareImage")->valuestring);
        Value.numberOfTracks[i] = cJSON_GetObjectItemCaseSensitive(playlist, "numberOfTracks")->valueint;
        Value.numberOfVideos[i] = cJSON_GetObjectItemCaseSensitive(playlist, "numberOfVideos")->valueint;
        Value.duration[i] = cJSON_GetObjectItemCaseSensitive(playlist, "duration")->valueint;

        cJSON *description = cJSON_GetObjectItemCaseSensitive(playlist, "description");
        if (cJSON_IsNull(description) != 1)
        {
          strcpy(Value.description[i], description->valuestring);
        }
        i = i + 1;
      }
    }
    else
    {
      strcpy(Value.uuid[0], cJSON_GetObjectItemCaseSensitive(input_json, "uuid")->valuestring);
      strcpy(Value.title[0], cJSON_GetObjectItemCaseSensitive(input_json, "title")->valuestring);
      strcpy(Value.lastUpdated[0], cJSON_GetObjectItemCaseSensitive(input_json, "lastUpdated")->valuestring);
      strcpy(Value.created[0], cJSON_GetObjectItemCaseSensitive(input_json, "created")->valuestring);
      strcpy(Value.image[0], cJSON_GetObjectItemCaseSensitive(input_json, "image")->valuestring);
      strcpy(Value.squareImage[0], cJSON_GetObjectItemCaseSensitive(input_json, "squareImage")->valuestring);
      Value.numberOfTracks[0] = cJSON_GetObjectItemCaseSensitive(input_json, "numberOfTracks")->valueint;
      Value.numberOfVideos[0] = cJSON_GetObjectItemCaseSensitive(input_json, "numberOfVideos")->valueint;
      Value.duration[0] = cJSON_GetObjectItemCaseSensitive(input_json, "duration")->valueint;

      cJSON *description = cJSON_GetObjectItemCaseSensitive(input_json, "description");
      if (cJSON_IsNull(description) != 1)
      {
        strcpy(Value.description[0], description->valuestring);
      }
    }
  }
  else
  {
    printf("%s\n", "No Items in Response");
  }
  return Value;
}

album_model parse_album(cJSON *input_json, int version) /* TODO: videoCover  */
{
  album_model Value;
  const cJSON *artistItems = NULL;
  const cJSON *artistItem = NULL;

  if (version == 0)
  {
    const cJSON *items = NULL;
    const cJSON *item = NULL;
    int i = 0;
    items = cJSON_GetObjectItemCaseSensitive(input_json, "items");
    Value.arraySize = cJSON_GetArraySize(items);
    cJSON_ArrayForEach(item, items)
    {
      int artistCounter = 0;

      Value.albumId[i] = cJSON_GetObjectItemCaseSensitive(item, "id")->valueint;
      strcpy(Value.albumTitle[i], cJSON_GetObjectItemCaseSensitive(item, "title")->valuestring);
      Value.duration[i] = cJSON_GetObjectItemCaseSensitive(item, "duration")->valueint;
      Value.numberOfTracks[i] = cJSON_GetObjectItemCaseSensitive(item, "numberOfTracks")->valueint;
      Value.numberOfVideos[i] = cJSON_GetObjectItemCaseSensitive(item, "numberOfVideos")->valueint;
      Value.numberOfVolumes[i] = cJSON_GetObjectItemCaseSensitive(item, "numberOfVolumes")->valueint;
      strcpy(Value.releaseDate[i], cJSON_GetObjectItemCaseSensitive(item, "releaseDate")->valuestring);
      strcpy(Value.copyright[i], cJSON_GetObjectItemCaseSensitive(item, "copyright")->valuestring);
      strcpy(Value.cover[i], cJSON_GetObjectItemCaseSensitive(item, "cover")->valuestring);
      //strcpy(Value.videoCover[i], cJSON_GetObjectItemCaseSensitive(item, "videoCover")->valuestring);
      Value.popularity[i] = cJSON_GetObjectItemCaseSensitive(item, "popularity")->valueint;
      
      artistItems = cJSON_GetObjectItemCaseSensitive(item, "artists");
      Value.subArraySize[i] = cJSON_GetArraySize(artistItems);
      cJSON_ArrayForEach(artistItem, artistItems)
      {
        Value.artistId[i][artistCounter] = cJSON_GetObjectItemCaseSensitive(artistItem, "id")->valueint;
        strcpy(Value.artistName[i][artistCounter], cJSON_GetObjectItemCaseSensitive(artistItem, "name")->valuestring);
	artistCounter = artistCounter + 1;
      }
      
      i = i + 1;
    }
  }
  else
  {
    int artistCounter = 0;

    Value.albumId[0] = cJSON_GetObjectItemCaseSensitive(input_json, "id")->valueint;
    strcpy(Value.albumTitle[0], cJSON_GetObjectItemCaseSensitive(input_json, "title")->valuestring);
    Value.duration[0] = cJSON_GetObjectItemCaseSensitive(input_json, "duration")->valueint;
    Value.numberOfTracks[0] = cJSON_GetObjectItemCaseSensitive(input_json, "numberOfTracks")->valueint;
    Value.numberOfVideos[0] = cJSON_GetObjectItemCaseSensitive(input_json, "numberOfVideos")->valueint;
    Value.numberOfVolumes[0] = cJSON_GetObjectItemCaseSensitive(input_json, "numberOfVolumes")->valueint;
    strcpy(Value.releaseDate[0], cJSON_GetObjectItemCaseSensitive(input_json, "releaseDate")->valuestring);
    strcpy(Value.copyright[0], cJSON_GetObjectItemCaseSensitive(input_json, "copyright")->valuestring);
    strcpy(Value.cover[0], cJSON_GetObjectItemCaseSensitive(input_json, "cover")->valuestring);
    //strcpy(Value.videoCover[0], cJSON_GetObjectItemCaseSensitive(input_json, "videoCover")->valuestring);
    Value.popularity[0] = cJSON_GetObjectItemCaseSensitive(input_json, "popularity")->valueint;

    artistItems = cJSON_GetObjectItemCaseSensitive(input_json, "artists");
    Value.subArraySize[0] = cJSON_GetArraySize(artistItems);
    cJSON_ArrayForEach(artistItem, artistItems)
    {
      Value.artistId[0][artistCounter] = cJSON_GetObjectItemCaseSensitive(artistItem, "id")->valueint;
      strcpy(Value.artistName[0][artistCounter], cJSON_GetObjectItemCaseSensitive(artistItem, "name")->valuestring);
      artistCounter = artistCounter + 1;
    }
  }
  return Value;
}

artist_model parse_artist(cJSON *input_json, int version)
{
  const cJSON *item = NULL;
  const cJSON *innerItem = NULL;
  const cJSON *items = NULL;
  int i = 0;
  artist_model Value;
  if (version == 1) /* Additional Object in "items"-Array */
  {
    items = cJSON_GetObjectItemCaseSensitive(input_json, "items");
    Value.arraySize = cJSON_GetArraySize(items);
    Value.totalNumberOfItems = cJSON_GetObjectItemCaseSensitive(input_json, "totalNumberOfItems")->valueint;
    Value.offset = cJSON_GetObjectItemCaseSensitive(input_json, "offset")->valueint;

    cJSON_ArrayForEach(item, items)
    {
      innerItem = cJSON_GetObjectItemCaseSensitive(item, "item");
      Value.id[i] = cJSON_GetObjectItemCaseSensitive(innerItem, "id")->valueint;
      strcpy(Value.name[i], cJSON_GetObjectItemCaseSensitive(innerItem, "name")->valuestring);
      strcpy(Value.picture[i], cJSON_GetObjectItemCaseSensitive(innerItem, "picture")->valuestring);
      Value.popularity[i] = cJSON_GetObjectItemCaseSensitive(innerItem, "popularity")->valueint;
      i = i + 1;
    }
    return Value;
  }
  else
  {
    Value.id[0] = cJSON_GetObjectItemCaseSensitive(input_json, "id")->valueint;
    strcpy(Value.name[0], cJSON_GetObjectItemCaseSensitive(input_json, "name")->valuestring);
    strcpy(Value.picture[0], cJSON_GetObjectItemCaseSensitive(input_json, "picture")->valuestring);
    Value.popularity[0] = cJSON_GetObjectItemCaseSensitive(input_json, "popularity")->valueint;
    return Value;
  }


}

search_model parse_search(cJSON *input_json)
{
  search_model Value;
  const cJSON *artists = NULL;
  const cJSON *artistsItems = NULL;
  const cJSON *artistsItem = NULL;
  int artistsCounter = 0;
  const cJSON *albums = NULL;
  const cJSON *albumsItems = NULL;
  const cJSON *albumsItem = NULL;
  int albumsCounter = 0;
  const cJSON *playlists = NULL;
  const cJSON *playlistsItems = NULL;
  const cJSON *playlistsItem = NULL;
  int playlistsCounter = 0;
  const cJSON *tracks = NULL;
  const cJSON *tracksItems = NULL;
  const cJSON *tracksItem = NULL;
  int tracksCounter = 0;
  const cJSON *videos = NULL;
  const cJSON *videosItems = NULL;
  const cJSON *videosItem = NULL;
  const cJSON *topHit;
  int videosCounter = 0;

  artists = cJSON_GetObjectItemCaseSensitive(input_json, "artists");
  artistsItems = cJSON_GetObjectItemCaseSensitive(artists, "items");
  albums = cJSON_GetObjectItemCaseSensitive(input_json, "albums");
  albumsItems = cJSON_GetObjectItemCaseSensitive(albums, "items");
  playlists = cJSON_GetObjectItemCaseSensitive(input_json, "playlists");
  playlistsItems = cJSON_GetObjectItemCaseSensitive(playlists, "items");
  tracks = cJSON_GetObjectItemCaseSensitive(input_json, "tracks");
  tracksItems = cJSON_GetObjectItemCaseSensitive(tracks, "items");
  videos = cJSON_GetObjectItemCaseSensitive(input_json, "videos");
  videosItems = cJSON_GetObjectItemCaseSensitive(videos, "items");
  topHit = cJSON_GetObjectItemCaseSensitive(input_json, "topHit");
  Value.arraySizeArtist = cJSON_GetArraySize(artistsItems);
  Value.offsetArtist = cJSON_GetObjectItemCaseSensitive(artists, "offset")->valueint;
  Value.totalNumberOfItemsArtist = cJSON_GetObjectItemCaseSensitive(artists, "totalNumberOfItems")->valueint;
  if (Value.totalNumberOfItemsArtist != 0)
  {
    cJSON_ArrayForEach(artistsItem, artistsItems)
    {
      Value.artistId[artistsCounter] = cJSON_GetObjectItemCaseSensitive(artistsItem, "id")->valueint;
      strcpy(Value.artistName[artistsCounter], cJSON_GetObjectItemCaseSensitive(artistsItem, "name")->valuestring);
      strcpy(Value.artistPicture[artistsCounter], cJSON_GetObjectItemCaseSensitive(artistsItem, "picture")->valuestring);
      Value.artistPopularity[artistsCounter] = cJSON_GetObjectItemCaseSensitive(artistsItem, "popularity")->valueint;
      artistsCounter = artistsCounter + 1;
    }
  }
  Value.arraySizeAlbum = cJSON_GetArraySize(albumsItems);
  Value.offsetAlbum = cJSON_GetObjectItemCaseSensitive(albums, "offset")->valueint;
  Value.totalNumberOfItemsAlbum = cJSON_GetObjectItemCaseSensitive(albums, "totalNumberOfItems")->valueint;
  if (Value.totalNumberOfItemsAlbum != 0)
  {
    cJSON_ArrayForEach(albumsItem, albumsItems)
    {
      Value.albumId[albumsCounter] = cJSON_GetObjectItemCaseSensitive(albumsItem, "id")->valueint;
      strcpy(Value.albumTitle[albumsCounter], cJSON_GetObjectItemCaseSensitive(albumsItem, "title")->valuestring);
      strcpy(Value.albumCover[albumsCounter], cJSON_GetObjectItemCaseSensitive(albumsItem, "cover")->valuestring);
      Value.albumPopularity[albumsCounter] = cJSON_GetObjectItemCaseSensitive(albumsItem, "popularity")->valueint;
      Value.albumDuration[albumsCounter] = cJSON_GetObjectItemCaseSensitive(albumsItem, "duration")->valueint;
      const cJSON *albumArtist = cJSON_GetObjectItemCaseSensitive(albumsItem, "artists");
      const cJSON * albumArray = cJSON_GetArrayItem(albumArtist, 0);
      strcpy(Value.albumArtistName[albumsCounter], cJSON_GetObjectItemCaseSensitive(albumArray, "name")->valuestring);
      Value.albumArtistId[albumsCounter] = cJSON_GetObjectItemCaseSensitive(albumArray, "id")->valueint;
      albumsCounter = albumsCounter + 1;
    }
  }
  Value.arraySizePlaylist = cJSON_GetArraySize(playlistsItems);
  Value.offsetPlaylist = cJSON_GetObjectItemCaseSensitive(playlists, "offset")->valueint;
  Value.totalNumberOfItemsPlaylist = cJSON_GetObjectItemCaseSensitive(playlists, "totalNumberOfItems")->valueint;
  if (Value.totalNumberOfItemsPlaylist != 0)
  {
    cJSON_ArrayForEach(playlistsItem, playlistsItems)
    {
      strcpy(Value.playlistUUID[playlistsCounter], cJSON_GetObjectItemCaseSensitive(playlistsItem, "uuid")->valuestring);
      strcpy(Value.playlistTitle[playlistsCounter], cJSON_GetObjectItemCaseSensitive(playlistsItem, "title")->valuestring);
      strcpy(Value.playlistImage[playlistsCounter], cJSON_GetObjectItemCaseSensitive(playlistsItem, "image")->valuestring);
      Value.playlistDuration[playlistsCounter] = cJSON_GetObjectItemCaseSensitive(playlistsItem, "duration")->valueint;
      strcpy(Value.playlistSquareImage[playlistsCounter], cJSON_GetObjectItemCaseSensitive(playlistsItem, "squareImage")->valuestring);
      Value.playlistNumberOfTracks[playlistsCounter] = cJSON_GetObjectItemCaseSensitive(playlistsItem, "numberOfTracks")->valueint;
      Value.playlistNumberOfVideos[playlistsCounter] = cJSON_GetObjectItemCaseSensitive(playlistsItem, "numberOfVideos")->valueint;
      Value.playlistPopularity[playlistsCounter] = cJSON_GetObjectItemCaseSensitive(playlistsItem, "popularity")->valueint;
      playlistsCounter = playlistsCounter + 1;
    }
  }
  Value.arraySizeTracks = cJSON_GetArraySize(tracksItems);
  Value.offsetTracks = cJSON_GetObjectItemCaseSensitive(tracks, "offset")->valueint;
  Value.totalNumberOfItemsTracks = cJSON_GetObjectItemCaseSensitive(tracks, "totalNumberOfItems")->valueint;
  if (Value.totalNumberOfItemsTracks != 0)
  {
    cJSON_ArrayForEach(tracksItem, tracksItems)
    {
      Value.tracksId[tracksCounter] = cJSON_GetObjectItemCaseSensitive(tracksItem, "id")->valueint;
      strcpy(Value.tracksTitle[tracksCounter], cJSON_GetObjectItemCaseSensitive(tracksItem, "title")->valuestring);
      Value.tracksDuration[tracksCounter] = cJSON_GetObjectItemCaseSensitive(tracksItem, "duration")->valueint;
      const cJSON *tracksAlbum = cJSON_GetObjectItemCaseSensitive(tracksItem, "album");
      Value.tracksAlbumId[tracksCounter] = cJSON_GetObjectItemCaseSensitive(tracksAlbum, "id")->valueint;
      strcpy(Value.tracksAlbumTitle[tracksCounter], cJSON_GetObjectItemCaseSensitive(tracksAlbum, "title")->valuestring);
      strcpy(Value.tracksCover[tracksCounter], cJSON_GetObjectItemCaseSensitive(tracksAlbum, "cover")->valuestring);
      strcpy(Value.tracksReleaseDate[tracksCounter], cJSON_GetObjectItemCaseSensitive(tracksAlbum, "releaseDate")->valuestring);
      const cJSON *tracksArtist = cJSON_GetObjectItemCaseSensitive(tracksItem, "artists");
      const cJSON * artistArray = cJSON_GetArrayItem(tracksArtist, 0);
      Value.tracksArtistId[tracksCounter] = cJSON_GetObjectItemCaseSensitive(artistArray, "id")->valueint;
      strcpy(Value.tracksArtistName[tracksCounter], cJSON_GetObjectItemCaseSensitive(artistArray, "name")->valuestring);
      tracksCounter = tracksCounter + 1;
    }
  }
  Value.arraySizeVideos = cJSON_GetArraySize(videosItems);
  Value.offsetVideos = cJSON_GetObjectItemCaseSensitive(videos, "offset")->valueint;
  Value.totalNumberOfItemsVideos = cJSON_GetObjectItemCaseSensitive(videos, "totalNumberOfItems")->valueint;
  if (Value.totalNumberOfItemsVideos != 0)
  {
    cJSON_ArrayForEach(videosItem, videosItems)
    {
      Value.videoId[videosCounter] = cJSON_GetObjectItemCaseSensitive(videosItem, "id")->valueint;
      strcpy(Value.videoTitle[videosCounter], cJSON_GetObjectItemCaseSensitive(videosItem, "title")->valuestring);
      Value.videoDuration[videosCounter] = cJSON_GetObjectItemCaseSensitive(videosItem, "duration")->valueint;
      strcpy(Value.videoImageId[videosCounter], cJSON_GetObjectItemCaseSensitive(videosItem, "imageId")->valuestring);
      strcpy(Value.videoReleaseDate[videosCounter], cJSON_GetObjectItemCaseSensitive(videosItem, "releaseDate")->valuestring);
      const cJSON *videoArtist = cJSON_GetObjectItemCaseSensitive(videosItem, "artists");
      const cJSON * videoArtistArray = cJSON_GetArrayItem(videoArtist, 0);
      Value.videoArtistId[videosCounter] = cJSON_GetObjectItemCaseSensitive(videoArtistArray, "id")->valueint;
      strcpy(Value.videoArtistName[videosCounter], cJSON_GetObjectItemCaseSensitive(videoArtistArray, "name")->valuestring);
      videosCounter = videosCounter + 1;
    }
  }
  const cJSON *topHitValue = cJSON_GetObjectItemCaseSensitive(topHit, "value");
  strcpy(Value.topHitType, cJSON_GetObjectItemCaseSensitive(topHit, "type")->valuestring);
  if (strcmp(Value.topHitType, "ARTISTS") == 0)
  {
    Value.artistId[100] = cJSON_GetObjectItemCaseSensitive(topHitValue, "id")->valueint;
    strcpy(Value.artistName[100], cJSON_GetObjectItemCaseSensitive(topHitValue, "name")->valuestring);
    strcpy(Value.artistPicture[100], cJSON_GetObjectItemCaseSensitive(topHitValue, "picture")->valuestring);
    Value.artistPopularity[100] = cJSON_GetObjectItemCaseSensitive(topHitValue, "popularity")->valueint;
  }
  else if (strcmp(Value.topHitType, "ALBUMS") == 0)
  {
    Value.albumId[100] = cJSON_GetObjectItemCaseSensitive(topHitValue, "id")->valueint;
    strcpy(Value.albumTitle[100], cJSON_GetObjectItemCaseSensitive(topHitValue, "title")->valuestring);
    strcpy(Value.albumCover[100], cJSON_GetObjectItemCaseSensitive(topHitValue, "cover")->valuestring);
    Value.albumPopularity[100] = cJSON_GetObjectItemCaseSensitive(topHitValue, "popularity")->valueint;
    Value.albumDuration[100] = cJSON_GetObjectItemCaseSensitive(topHitValue, "duration")->valueint;
    const cJSON *topAlbumArtist = cJSON_GetObjectItemCaseSensitive(topHitValue, "artists");
    const cJSON * topAlbumArray = cJSON_GetArrayItem(topAlbumArtist, 0);
    Value.albumArtistId[100] = cJSON_GetObjectItemCaseSensitive(topAlbumArray, "id")->valueint;
    strcpy(Value.albumArtistName[100], cJSON_GetObjectItemCaseSensitive(topAlbumArray, "name")->valuestring);
  }
  else if (strcmp(Value.topHitType, "TRACKS") == 0)
  {
    Value.tracksId[100] = cJSON_GetObjectItemCaseSensitive(topHitValue, "id")->valueint;
    strcpy(Value.tracksTitle[100], cJSON_GetObjectItemCaseSensitive(topHitValue, "title")->valuestring);
    Value.tracksDuration[100] = cJSON_GetObjectItemCaseSensitive(topHitValue, "duration")->valueint;
    const cJSON *tracksAlbum = cJSON_GetObjectItemCaseSensitive(topHitValue, "album");
    Value.tracksAlbumId[100] = cJSON_GetObjectItemCaseSensitive(tracksAlbum, "id")->valueint;
    strcpy(Value.tracksAlbumTitle[100], cJSON_GetObjectItemCaseSensitive(tracksAlbum, "title")->valuestring);
    strcpy(Value.tracksCover[100], cJSON_GetObjectItemCaseSensitive(tracksAlbum, "cover")->valuestring);
    strcpy(Value.tracksReleaseDate[100], cJSON_GetObjectItemCaseSensitive(tracksAlbum, "releaseDate")->valuestring);
    const cJSON *topTracksArtist = cJSON_GetObjectItemCaseSensitive(topHitValue, "artists");
    const cJSON * topArtistArray = cJSON_GetArrayItem(topTracksArtist, 0);
    Value.tracksArtistId[100] = cJSON_GetObjectItemCaseSensitive(topArtistArray, "id")->valueint;
    strcpy(Value.tracksArtistName[100], cJSON_GetObjectItemCaseSensitive(topArtistArray, "name")->valuestring);
  }
  else if (strcmp(Value.topHitType, "VIDEOS") == 0)
  {
    Value.videoId[100] = cJSON_GetObjectItemCaseSensitive(topHitValue, "id")->valueint;
    strcpy(Value.videoTitle[100], cJSON_GetObjectItemCaseSensitive(topHitValue, "title")->valuestring);
    strcpy(Value.videoImageId[100], cJSON_GetObjectItemCaseSensitive(topHitValue, "imageId")->valuestring);
    strcpy(Value.videoReleaseDate[100], cJSON_GetObjectItemCaseSensitive(topHitValue, "releaseDate")->valuestring);
    Value.videoDuration[100] = cJSON_GetObjectItemCaseSensitive(topHitValue, "duration")->valueint;
    const cJSON *topVideo = cJSON_GetObjectItemCaseSensitive(topHitValue, "artists");
    const cJSON * topVideoArray = cJSON_GetArrayItem(topVideo, 0);
    Value.videoArtistId[100] = cJSON_GetObjectItemCaseSensitive(topVideoArray, "id")->valueint;
    strcpy(Value.videoArtistName[100], cJSON_GetObjectItemCaseSensitive(topVideoArray, "name")->valuestring);
  }
  else if (strcmp(Value.topHitType, "PLAYLISTS") == 0)
  {
    strcpy(Value.playlistUUID[100], cJSON_GetObjectItemCaseSensitive(topHitValue, "uuid")->valuestring);
    strcpy(Value.playlistTitle[100], cJSON_GetObjectItemCaseSensitive(topHitValue, "title")->valuestring);
    strcpy(Value.playlistImage[100], cJSON_GetObjectItemCaseSensitive(topHitValue, "image")->valuestring);
    Value.playlistDuration[100] = cJSON_GetObjectItemCaseSensitive(topHitValue, "duration")->valueint;
    strcpy(Value.playlistSquareImage[100], cJSON_GetObjectItemCaseSensitive(topHitValue, "squareImage")->valuestring);
    Value.playlistNumberOfTracks[100] = cJSON_GetObjectItemCaseSensitive(topHitValue, "numberOfTracks")->valueint;
    Value.playlistNumberOfVideos[100] = cJSON_GetObjectItemCaseSensitive(topHitValue, "numberOfVideos")->valueint;
    Value.playlistPopularity[100] = cJSON_GetObjectItemCaseSensitive(topHitValue, "popularity")->valueint;
  }
  return Value;
}
