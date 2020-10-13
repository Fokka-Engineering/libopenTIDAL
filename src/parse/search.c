#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/openTIDAL.h"

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
      cJSON *artistId = cJSON_GetObjectItemCaseSensitive(artistsItem, "id");
      cJSON *artistName = cJSON_GetObjectItemCaseSensitive(artistsItem, "name");
      cJSON *artistPicture = cJSON_GetObjectItemCaseSensitive(artistsItem, "picture");
      cJSON *artistPopularity = cJSON_GetObjectItemCaseSensitive(artistsItem, "popularity");

      Value.artistId[artistsCounter] = artistId->valueint;
      strcpy(Value.artistName[artistsCounter], artistName->valuestring);
      strcpy(Value.artistPicture[artistsCounter], artistPicture->valuestring);
      Value.artistPopularity[artistsCounter] = artistPopularity->valueint;
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
      cJSON *albumId = cJSON_GetObjectItemCaseSensitive(albumsItem, "id");
      cJSON *albumTitle = cJSON_GetObjectItemCaseSensitive(albumsItem, "title");
      cJSON *albumCover = cJSON_GetObjectItemCaseSensitive(albumsItem, "cover");
      cJSON *albumPopularity = cJSON_GetObjectItemCaseSensitive(albumsItem, "popularity");
      cJSON *albumDuration = cJSON_GetObjectItemCaseSensitive(albumsItem, "duration");
      cJSON *albumArtist = cJSON_GetObjectItemCaseSensitive(albumsItem, "artists");
      cJSON *albumArtistArray = cJSON_GetArrayItem(albumArtist, 0); /* TODO: Add 2D-Array for Artists */ 
      cJSON *albumArtistId = cJSON_GetObjectItemCaseSensitive(albumArtistArray, "id");
      cJSON *albumArtistName = cJSON_GetObjectItemCaseSensitive(albumArtistArray, "name");

      Value.albumId[albumsCounter] = albumId->valueint;
      strcpy(Value.albumTitle[albumsCounter], albumTitle->valuestring);
      strcpy(Value.albumCover[albumsCounter], albumCover->valuestring);
      Value.albumPopularity[albumsCounter] = albumPopularity->valueint;
      Value.albumDuration[albumsCounter] = albumDuration->valueint;

      strcpy(Value.albumArtistName[albumsCounter], albumArtistName->valuestring);
      Value.albumArtistId[albumsCounter] = albumArtistId->valueint;
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
      cJSON *playlistUuid = cJSON_GetObjectItemCaseSensitive(playlistsItem, "uuid");
      cJSON *playlistTitle = cJSON_GetObjectItemCaseSensitive(playlistsItem, "title");
      cJSON *playlistImage = cJSON_GetObjectItemCaseSensitive(playlistsItem, "image");
      cJSON *playlistDuration = cJSON_GetObjectItem(playlistsItem, "duration");
      cJSON *playlistSquareImage = cJSON_GetObjectItemCaseSensitive(playlistsItem, "squareImage");
      cJSON *playlistNumberOfTracks = cJSON_GetObjectItemCaseSensitive(playlistsItem, "numberOfTracks");
      cJSON *playlistNumberOfVideos = cJSON_GetObjectItemCaseSensitive(playlistsItem, "numberOfVideos");
      cJSON *playlistPopularity = cJSON_GetObjectItemCaseSensitive(playlistsItem, "popularity");

      strcpy(Value.playlistUUID[playlistsCounter], playlistUuid->valuestring);
      strcpy(Value.playlistTitle[playlistsCounter], playlistTitle->valuestring);
      strcpy(Value.playlistImage[playlistsCounter], playlistImage->valuestring);
      Value.playlistDuration[playlistsCounter] = playlistDuration->valueint;
      strcpy(Value.playlistSquareImage[playlistsCounter], playlistSquareImage->valuestring);
      Value.playlistNumberOfTracks[playlistsCounter] = playlistNumberOfTracks->valueint;
      Value.playlistNumberOfVideos[playlistsCounter] = playlistNumberOfVideos->valueint;
      Value.playlistPopularity[playlistsCounter] = playlistPopularity->valueint;
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
      cJSON *tracksId = cJSON_GetObjectItemCaseSensitive(tracksItem, "id");
      cJSON *tracksTitle = cJSON_GetObjectItemCaseSensitive(tracksItem, "title");
      cJSON *tracksDuration = cJSON_GetObjectItemCaseSensitive(tracksItem, "duration");
      cJSON *tracksAlbum = cJSON_GetObjectItemCaseSensitive(tracksItem, "album");
      cJSON *tracksReleaseDate = cJSON_GetObjectItemCaseSensitive(tracksAlbum, "releaseDate");
      cJSON *tracksAlbumId = cJSON_GetObjectItemCaseSensitive(tracksAlbum, "id");
      cJSON *tracksAlbumTitle = cJSON_GetObjectItemCaseSensitive(tracksAlbum, "title");
      cJSON *tracksAlbumCover = cJSON_GetObjectItemCaseSensitive(tracksAlbum, "cover");
      cJSON *tracksArtist = cJSON_GetObjectItemCaseSensitive(tracksItem, "artists");
      cJSON *tracksArtistArray = cJSON_GetArrayItem(tracksArtist, 0); /* TODO: 2D-Array  */
      cJSON *tracksArtistId = cJSON_GetObjectItemCaseSensitive(tracksArtistArray, "id");
      cJSON *tracksArtistName = cJSON_GetObjectItemCaseSensitive(tracksArtistArray, "name");

      Value.tracksId[tracksCounter] = tracksId->valueint;
      strcpy(Value.tracksTitle[tracksCounter], tracksTitle->valuestring);
      Value.tracksDuration[tracksCounter] = tracksDuration->valueint;
      Value.tracksAlbumId[tracksCounter] = tracksAlbumId->valueint;
      strcpy(Value.tracksAlbumTitle[tracksCounter], tracksAlbumTitle->valuestring);
      strcpy(Value.tracksCover[tracksCounter], tracksAlbumCover->valuestring);
      strcpy(Value.tracksReleaseDate[tracksCounter], tracksReleaseDate->valuestring);
      Value.tracksArtistId[tracksCounter] = tracksArtistId->valueint;
      strcpy(Value.tracksArtistName[tracksCounter], tracksArtistName->valuestring);
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
      cJSON *videoId = cJSON_GetObjectItemCaseSensitive(videosItem, "id");
      cJSON *videoTitle = cJSON_GetObjectItemCaseSensitive(videosItem, "title");
      cJSON *videoDuration = cJSON_GetObjectItemCaseSensitive(videosItem, "duration");
      cJSON *videoImageId = cJSON_GetObjectItemCaseSensitive(videosItem, "imageId");
      cJSON *videoReleaseDate = cJSON_GetObjectItemCaseSensitive(videosItem, "releaseDate");
      cJSON *videoArtist = cJSON_GetObjectItemCaseSensitive(videosItem, "artists");
      cJSON *videoArtistArray = cJSON_GetArrayItem(videoArtist, 0);
      cJSON *videoArtistId = cJSON_GetObjectItemCaseSensitive(videoArtistArray, "id");
      cJSON *videoArtistName = cJSON_GetObjectItemCaseSensitive(videoArtistArray, "name");

      Value.videoId[videosCounter] = videoId->valueint;
      strcpy(Value.videoTitle[videosCounter], videoTitle->valuestring);
      Value.videoDuration[videosCounter] = videoDuration->valueint;
      strcpy(Value.videoImageId[videosCounter], videoImageId->valuestring);
      strcpy(Value.videoReleaseDate[videosCounter], videoReleaseDate->valuestring);
      Value.videoArtistId[videosCounter] = videoArtistId->valueint;
      strcpy(Value.videoArtistName[videosCounter], videoArtistName->valuestring);
      videosCounter = videosCounter + 1;
    }
  }
  const cJSON *topHitValue = cJSON_GetObjectItemCaseSensitive(topHit, "value");
  strcpy(Value.topHitType, cJSON_GetObjectItemCaseSensitive(topHit, "type")->valuestring);
  if (strcmp(Value.topHitType, "ARTISTS") == 0)
  {
    cJSON *artistId = cJSON_GetObjectItemCaseSensitive(topHitValue, "id");
    cJSON *artistName = cJSON_GetObjectItemCaseSensitive(topHitValue, "name");
    cJSON *artistPicture = cJSON_GetObjectItemCaseSensitive(topHitValue, "picture");
    cJSON *artistPopularity = cJSON_GetObjectItemCaseSensitive(topHitValue, "popularity");

    Value.artistId[100] = artistId->valueint;
    strcpy(Value.artistName[100], artistName->valuestring);
    strcpy(Value.artistPicture[100], artistPicture->valuestring);
    Value.artistPopularity[100] = artistPopularity->valueint;
  }
  else if (strcmp(Value.topHitType, "ALBUMS") == 0)
  {
    cJSON *albumId = cJSON_GetObjectItemCaseSensitive(topHitValue, "id");
    cJSON *albumTitle = cJSON_GetObjectItemCaseSensitive(topHitValue, "title");
    cJSON *albumCover = cJSON_GetObjectItemCaseSensitive(topHitValue, "cover");
    cJSON *albumPopularity = cJSON_GetObjectItemCaseSensitive(topHitValue, "popularity");
    cJSON *albumDuration = cJSON_GetObjectItemCaseSensitive(topHitValue, "duration");
    cJSON *albumArtist = cJSON_GetObjectItemCaseSensitive(topHitValue, "artists");
    cJSON *albumArtistArray = cJSON_GetArrayItem(albumArtist, 0); /* TODO: Add 2D-Array for Artists */
    cJSON *albumArtistId = cJSON_GetObjectItemCaseSensitive(albumArtistArray, "id");
    cJSON *albumArtistName = cJSON_GetObjectItemCaseSensitive(albumArtistArray, "name");

    Value.albumId[100] = albumId->valueint;
    strcpy(Value.albumTitle[100], albumTitle->valuestring);
    strcpy(Value.albumCover[100], albumCover->valuestring);
    Value.albumPopularity[100] = albumPopularity->valueint;
    Value.albumDuration[100] = albumDuration->valueint;
    Value.albumArtistId[100] = albumArtistId->valueint;
    strcpy(Value.albumArtistName[100], albumArtistName->valuestring);
  }
  else if (strcmp(Value.topHitType, "TRACKS") == 0)
  {
    cJSON *tracksId = cJSON_GetObjectItemCaseSensitive(topHitValue, "id");
    cJSON *tracksTitle = cJSON_GetObjectItemCaseSensitive(topHitValue, "title");
    cJSON *tracksDuration = cJSON_GetObjectItemCaseSensitive(topHitValue, "duration");
    cJSON *tracksAlbum = cJSON_GetObjectItemCaseSensitive(topHitValue, "album");
    cJSON *tracksReleaseDate = cJSON_GetObjectItemCaseSensitive(tracksAlbum, "releaseDate");
    cJSON *tracksAlbumId = cJSON_GetObjectItemCaseSensitive(tracksAlbum, "id");
    cJSON *tracksAlbumTitle = cJSON_GetObjectItemCaseSensitive(tracksAlbum, "title");
    cJSON *tracksAlbumCover = cJSON_GetObjectItemCaseSensitive(tracksAlbum, "cover");
    cJSON *tracksArtist = cJSON_GetObjectItemCaseSensitive(topHitValue, "artists");
    cJSON *tracksArtistArray = cJSON_GetArrayItem(tracksArtist, 0); /* TODO: 2D-Array  */
    cJSON *tracksArtistId = cJSON_GetObjectItemCaseSensitive(tracksArtistArray, "id");
    cJSON *tracksArtistName = cJSON_GetObjectItemCaseSensitive(tracksArtistArray, "name");

    Value.tracksId[100] = tracksId->valueint;
    strcpy(Value.tracksTitle[100], tracksTitle->valuestring);
    Value.tracksDuration[100] = tracksDuration->valueint;
    Value.tracksAlbumId[100] = tracksAlbumId->valueint;
    strcpy(Value.tracksAlbumTitle[100], tracksAlbumTitle->valuestring);
    strcpy(Value.tracksCover[100], tracksAlbumCover->valuestring);
    strcpy(Value.tracksReleaseDate[100], tracksReleaseDate->valuestring);
    Value.tracksArtistId[100] = tracksArtistId->valueint;
    strcpy(Value.tracksArtistName[100], tracksArtistName->valuestring);
  }
  else if (strcmp(Value.topHitType, "VIDEOS") == 0)
  {
    cJSON *videoId = cJSON_GetObjectItemCaseSensitive(topHitValue, "id");
    cJSON *videoTitle = cJSON_GetObjectItemCaseSensitive(topHitValue, "title");
    cJSON *videoDuration = cJSON_GetObjectItemCaseSensitive(topHitValue, "duration");
    cJSON *videoImageId = cJSON_GetObjectItemCaseSensitive(topHitValue, "imageId");
    cJSON *videoReleaseDate = cJSON_GetObjectItemCaseSensitive(topHitValue, "releaseDate");
    cJSON *videoArtist = cJSON_GetObjectItemCaseSensitive(topHitValue, "artists");
    cJSON *videoArtistArray = cJSON_GetArrayItem(videoArtist, 0);
    cJSON *videoArtistId = cJSON_GetObjectItemCaseSensitive(videoArtistArray, "id");
    cJSON *videoArtistName = cJSON_GetObjectItemCaseSensitive(videoArtistArray, "name");

    Value.videoId[100] = videoId->valueint;
    strcpy(Value.videoTitle[100], videoTitle->valuestring);
    strcpy(Value.videoImageId[100], videoImageId->valuestring);
    strcpy(Value.videoReleaseDate[100], videoReleaseDate->valuestring);
    Value.videoDuration[100] = videoDuration->valueint;
    Value.videoArtistId[100] = videoArtistId->valueint;
    strcpy(Value.videoArtistName[100], videoArtistName->valuestring);
  }
  else if (strcmp(Value.topHitType, "PLAYLISTS") == 0)
  {
    cJSON *playlistUuid = cJSON_GetObjectItemCaseSensitive(topHitValue, "uuid");
    cJSON *playlistTitle = cJSON_GetObjectItemCaseSensitive(topHitValue, "title");
    cJSON *playlistImage = cJSON_GetObjectItemCaseSensitive(topHitValue, "image");
    cJSON *playlistDuration = cJSON_GetObjectItem(topHitValue, "duration");
    cJSON *playlistSquareImage = cJSON_GetObjectItemCaseSensitive(topHitValue, "squareImage");
    cJSON *playlistNumberOfTracks = cJSON_GetObjectItemCaseSensitive(topHitValue, "numberOfTracks");
    cJSON *playlistNumberOfVideos = cJSON_GetObjectItemCaseSensitive(topHitValue, "numberOfVideos");
    cJSON *playlistPopularity = cJSON_GetObjectItemCaseSensitive(topHitValue, "popularity");

    strcpy(Value.playlistUUID[100], playlistUuid->valuestring);
    strcpy(Value.playlistTitle[100], playlistTitle->valuestring);
    strcpy(Value.playlistImage[100], playlistImage->valuestring);
    Value.playlistDuration[100] = playlistDuration->valueint;
    strcpy(Value.playlistSquareImage[100], playlistSquareImage->valuestring);
    Value.playlistNumberOfTracks[100] = playlistNumberOfTracks->valueint;
    Value.playlistNumberOfVideos[100] = playlistNumberOfVideos->valueint;
    Value.playlistPopularity[100] = playlistPopularity->valueint;
  }
  return Value;
}
