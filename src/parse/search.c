#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/openTIDAL.h"

search_model parse_search(cJSON *input_json)
{
  search_model Value;
  int ar = 0; /* artistCounter  */
  int al = 0; /* albumsCounter  */
  int pl = 0; /* playlistsCounter  */
  int tr = 0; /* tracksCounter  */
  int vi = 0; /* videosCounter  */

  cJSON *artists = cJSON_GetObjectItem(input_json, "artists");
  cJSON *artistsItems = cJSON_GetObjectItem(artists, "items");
  cJSON *artistTotalNumberOfItems = cJSON_GetObjectItem(artists, "totalNumberOfItems");
  cJSON *artistsItem = NULL;
  
  cJSON *albums = cJSON_GetObjectItem(input_json, "albums");
  cJSON *albumsItems = cJSON_GetObjectItem(albums, "items");
  cJSON *albumTotalNumberOfItems = cJSON_GetObjectItem(albums, "totalNumberOfItems");
  cJSON *albumsItem = NULL;

  cJSON *playlists = cJSON_GetObjectItem(input_json, "playlists");
  cJSON *playlistsItems = cJSON_GetObjectItem(playlists, "items");
  cJSON *playlistsTotalNumberOfItems = cJSON_GetObjectItem(playlists, "totalNumberOfItems");
  cJSON *playlistsItem = NULL;

  cJSON *tracks = cJSON_GetObjectItem(input_json, "tracks");
  cJSON *tracksItems = cJSON_GetObjectItem(tracks, "items");
  cJSON *tracksTotalNumberOfItems = cJSON_GetObjectItem(tracks, "totalNumberOfItems");
  cJSON *tracksItem = NULL;

  cJSON *videos = cJSON_GetObjectItem(input_json, "videos");
  cJSON *videosItems = cJSON_GetObjectItem(videos, "items");
  cJSON *videosTotalNumberOfItems = cJSON_GetObjectItem(videos, "totalNumberOfItems");
  cJSON *videosItem = NULL;
  
  cJSON *topHit = cJSON_GetObjectItemCaseSensitive(input_json, "topHit");
  Value.status = 1;

  if (artistTotalNumberOfItems != 0)
  {
    cJSON *artistLimit = cJSON_GetObjectItem(artists, "limit");
    cJSON *artistOffset = cJSON_GetObjectItem(artists, "offset");
    
    Value.artists.limit = artistLimit->valueint;
    Value.artists.offset = artistOffset->valueint;
    Value.artists.totalNumberOfItems = artistTotalNumberOfItems->valueint;
    Value.artists.arraySize = cJSON_GetArraySize(artistsItems);

    cJSON_ArrayForEach(artistsItem, artistsItems)
    {
      cJSON *id = cJSON_GetObjectItem(artistsItem, "id");
      cJSON *name = cJSON_GetObjectItemCaseSensitive(artistsItem, "name");
      cJSON *picture = cJSON_GetObjectItemCaseSensitive(artistsItem, "picture");
      cJSON *popularity = cJSON_GetObjectItem(artistsItem, "popularity");
      
      Value.artists.id[ar] = id->valueint;
      strncpy(Value.artists.name[ar], name->valuestring, sizeof(Value.artists.name[ar]));
      Value.artists.hasPicture[ar] = 0;
      if (cJSON_IsNull(picture) != 1)
      {
        Value.artists.hasPicture[ar] = 1;
        strncpy(Value.artists.picture[ar], picture->valuestring, sizeof(Value.artists.picture[ar]));
      }
      Value.artists.popularity[ar] = popularity->valueint;
      ar += 1;
    }
  }
  if (albumTotalNumberOfItems != 0)
  {
    cJSON *albumLimit = cJSON_GetObjectItem(albums, "limit");
    cJSON *albumOffset = cJSON_GetObjectItem(albums, "offset");

    Value.albums.limit = albumLimit->valueint;
    Value.albums.offset = albumOffset->valueint;
    Value.albums.totalNumberOfItems = albumTotalNumberOfItems->valueint;
    Value.albums.arraySize = cJSON_GetArraySize(albumsItems);

    cJSON_ArrayForEach(albumsItem, albumsItems)
    {
      int i = 0;
      cJSON *subItem = NULL;
      cJSON *id = cJSON_GetObjectItem(albumsItem, "id");
      cJSON *title = cJSON_GetObjectItemCaseSensitive(albumsItem, "title");
      cJSON *duration = cJSON_GetObjectItem(albumsItem, "duration");
      cJSON *numberOfTracks = cJSON_GetObjectItem(albumsItem, "numberOfTracks");
      cJSON *numberOfVideos = cJSON_GetObjectItem(albumsItem, "numberOfVideos");
      cJSON *numberOfVolumes = cJSON_GetObjectItem(albumsItem, "numberOfVolumes");
      cJSON *copyright = cJSON_GetObjectItemCaseSensitive(albumsItem, "copyright");
      cJSON *cover = cJSON_GetObjectItemCaseSensitive(albumsItem, "cover");
      cJSON *popularity = cJSON_GetObjectItem(albumsItem, "popularity");
      cJSON *releaseDate = cJSON_GetObjectItem(albumsItem, "releaseDate");
      cJSON *artist = cJSON_GetObjectItem(albumsItem, "artists");

      Value.albums.id[al] = id->valueint;
      strncpy(Value.albums.title[al], title->valuestring, sizeof(Value.albums.title[al]));
      Value.albums.duration[al] = duration->valueint;
      Value.albums.numberOfTracks[al] = numberOfTracks->valueint;
      Value.albums.numberOfVideos[al] = numberOfVideos->valueint;
      Value.albums.numberOfVolumes[al] = numberOfVolumes->valueint;
      strncpy(Value.albums.releaseDate[al], releaseDate->valuestring, sizeof(Value.albums.releaseDate[al]));
      strncpy(Value.albums.copyright[al], copyright->valuestring, sizeof(Value.albums.copyright[al]));
      strncpy(Value.albums.cover[al], cover->valuestring, sizeof(Value.albums.cover[al]));
      strncpy(Value.albums.releaseDate[al], releaseDate->valuestring, sizeof(Value.albums.releaseDate[al]));
      Value.albums.popularity[al] = popularity->valueint;
      
      Value.albums.subArraySize[al] = cJSON_GetArraySize(artist);
      cJSON_ArrayForEach(subItem, artist)
      {
        cJSON *artistId = cJSON_GetObjectItemCaseSensitive(subItem, "id");
        cJSON *artistName = cJSON_GetObjectItemCaseSensitive(subItem, "name");

        Value.albums.artistId[al][i] = artistId->valueint;
        strncpy(Value.albums.artistName[al][i], artistName->valuestring, sizeof(Value.albums.artistName[al][i]));
        i += 1;
      }
      al += 1;
    }
  }
  if (playlistsTotalNumberOfItems != 0)
  {
    cJSON *playlistsLimit = cJSON_GetObjectItem(playlists, "limit");
    cJSON *playlistsOffset = cJSON_GetObjectItem(playlists, "offset");

    Value.playlists.limit = playlistsLimit->valueint;
    Value.playlists.offset = playlistsOffset->valueint;
    Value.playlists.totalNumberOfItems = playlistsTotalNumberOfItems->valueint;
    Value.playlists.arraySize = cJSON_GetArraySize(playlistsItems);

    cJSON_ArrayForEach(playlistsItem, playlistsItems)
    {
      cJSON *uuid = cJSON_GetObjectItemCaseSensitive(playlistsItem, "uuid");
      cJSON *title = cJSON_GetObjectItemCaseSensitive(playlistsItem, "title");
      cJSON *description = cJSON_GetObjectItemCaseSensitive(playlistsItem, "description");
      cJSON *image = cJSON_GetObjectItemCaseSensitive(playlistsItem, "image");
      cJSON *duration = cJSON_GetObjectItem(playlistsItem, "duration");
      cJSON *squareImage = cJSON_GetObjectItemCaseSensitive(playlistsItem, "squareImage");
      cJSON *numberOfTracks = cJSON_GetObjectItem(playlistsItem, "numberOfTracks");
      cJSON *numberOfVideos = cJSON_GetObjectItem(playlistsItem, "numberOfVideos");
      cJSON *popularity = cJSON_GetObjectItem(playlistsItem, "popularity");
      cJSON *created = cJSON_GetObjectItemCaseSensitive(playlistsItem, "created");
      cJSON *lastUpdated = cJSON_GetObjectItemCaseSensitive(playlistsItem, "lastUpdated");

      strncpy(Value.playlists.uuid[pl], uuid->valuestring, sizeof(Value.playlists.uuid[pl]));
      strncpy(Value.playlists.title[pl], title->valuestring, sizeof(Value.playlists.title[pl]));
      Value.playlists.hasDescription[pl] = 0;
      if (cJSON_IsNull(description) != 1)
      {
        Value.playlists.hasDescription[pl] = 1;
        strncpy(Value.playlists.description[pl], description->valuestring, sizeof(Value.playlists.description[pl]));
      }
      strncpy(Value.playlists.image[pl], image->valuestring, sizeof(Value.playlists.image[pl]));
      Value.playlists.duration[pl] = duration->valueint;
      strncpy(Value.playlists.squareImage[pl], squareImage->valuestring, sizeof(Value.playlists.squareImage[pl]));
      strncpy(Value.playlists.created[pl], created->valuestring, sizeof(Value.playlists.created[pl]));
      strncpy(Value.playlists.lastUpdated[pl], lastUpdated->valuestring, sizeof(Value.playlists.lastUpdated[pl]));
      Value.playlists.numberOfTracks[pl] = numberOfTracks->valueint;
      Value.playlists.numberOfVideos[pl] = numberOfVideos->valueint;
      Value.playlists.popularity[pl] = popularity->valueint;
      pl += 1;
    }
  }
  if (tracksTotalNumberOfItems != 0)
  {
    cJSON *tracksLimit = cJSON_GetObjectItem(tracks, "limit");
    cJSON *tracksOffset = cJSON_GetObjectItem(tracks, "offset");

    Value.tracks.limit = tracksLimit->valueint;
    Value.tracks.offset = tracksOffset->valueint;
    Value.tracks.totalNumberOfItems = tracksTotalNumberOfItems->valueint;
    Value.tracks.arraySize = cJSON_GetArraySize(tracksItems);

    cJSON_ArrayForEach(tracksItem, tracksItems)
    {
      int i = 0;
      cJSON *subItem = NULL;
      cJSON *id = cJSON_GetObjectItem(tracksItem, "id");
      cJSON *title = cJSON_GetObjectItemCaseSensitive(tracksItem, "title");
      cJSON *duration = cJSON_GetObjectItem(tracksItem, "duration");
      cJSON *album = cJSON_GetObjectItem(tracksItem, "album");
      cJSON *releaseDate = cJSON_GetObjectItemCaseSensitive(album, "releaseDate");
      cJSON *popularity = cJSON_GetObjectItemCaseSensitive(tracksItem, "popularity");
      cJSON *version = cJSON_GetObjectItemCaseSensitive(tracksItem, "version");
      cJSON *albumId = cJSON_GetObjectItem(album, "id");
      cJSON *albumTitle = cJSON_GetObjectItemCaseSensitive(album, "title");
      cJSON *albumCover = cJSON_GetObjectItemCaseSensitive(album, "cover");
      cJSON *artist = cJSON_GetObjectItem(tracksItem, "artists");

      Value.tracks.id[tr] = id->valueint;
      strncpy(Value.tracks.title[tr], title->valuestring, sizeof(Value.tracks.title[tr]));
      Value.tracks.duration[tr] = duration->valueint;
      Value.tracks.popularity[tr] = popularity->valueint;
      Value.tracks.hasVersion[tr] = 0;
      if (cJSON_IsObject(version) && cJSON_IsNull(version) != 1)
      {
        Value.tracks.hasVersion[tr] = 1;
	strncpy(Value.tracks.version[tr], version->valuestring, sizeof(Value.tracks.version[tr]));
      }
      Value.tracks.albumId[tr] = albumId->valueint;
      strncpy(Value.tracks.albumTitle[tr], albumTitle->valuestring, sizeof(Value.tracks.albumTitle[tr]));
      strncpy(Value.tracks.cover[tr], albumCover->valuestring, sizeof(Value.tracks.cover[tr]));
      strncpy(Value.tracks.releaseDate[tr], releaseDate->valuestring, sizeof(Value.tracks.releaseDate[tr]));
      
      Value.tracks.subArraySize[tr] = cJSON_GetArraySize(artist);
      cJSON_ArrayForEach(subItem, artist)
      {
        cJSON *artistId = cJSON_GetObjectItem(subItem, "id");
        cJSON *artistName = cJSON_GetObjectItemCaseSensitive(subItem, "name");

        Value.tracks.artistId[tr][i] = artistId->valueint;
        strncpy(Value.tracks.artistName[tr][i], artistName->valuestring, sizeof(Value.tracks.artistName[tr][i]));
        i += 1;
      }
      tr += 1;
    }
  }
  if (videosTotalNumberOfItems != 0)
  {
    cJSON *videosLimit = cJSON_GetObjectItem(videos, "limit");
    cJSON *videosOffset = cJSON_GetObjectItem(videos, "offset");

    Value.videos.limit = videosLimit->valueint;
    Value.videos.offset = videosOffset->valueint;
    Value.videos.totalNumberOfItems = videosTotalNumberOfItems->valueint;
    Value.videos.arraySize = cJSON_GetArraySize(videosItems);

    cJSON_ArrayForEach(videosItem, videosItems)
    {
      cJSON *id = cJSON_GetObjectItem(videosItem, "id");
      cJSON *title = cJSON_GetObjectItemCaseSensitive(videosItem, "title");
      cJSON *duration = cJSON_GetObjectItem(videosItem, "duration");
      cJSON *imageId = cJSON_GetObjectItemCaseSensitive(videosItem, "imageId");
      cJSON *releaseDate = cJSON_GetObjectItemCaseSensitive(videosItem, "releaseDate");
      cJSON *popularity = cJSON_GetObjectItemCaseSensitive(videosItem, "popularity");
      cJSON *artist = cJSON_GetObjectItemCaseSensitive(videosItem, "artists");

      Value.videos.id[vi] = id->valueint;
      strncpy(Value.videos.title[vi], title->valuestring, sizeof(Value.videos.title[vi]));
      Value.videos.duration[vi] = duration->valueint;
      Value.videos.popularity[vi] = popularity->valueint;
      strncpy(Value.videos.cover[vi], imageId->valuestring, sizeof(Value.videos.cover[vi]));
      strncpy(Value.videos.releaseDate[vi], releaseDate->valuestring, sizeof(Value.videos.releaseDate[vi]));
      vi += 1;
    }
  }
  return Value;
  /*const cJSON *topHitValue = cJSON_GetObjectItemCaseSensitive(topHit, "value");
  strncpy(Value.topHitType, cJSON_GetObjectItemCaseSensitive(topHit, "type")->valuestring, sizeof(Value.topHitType));
  if (strcmp(Value.topHitType, "ARTISTS") == 0)
  {
    cJSON *artistId = cJSON_GetObjectItemCaseSensitive(topHitValue, "id");
    cJSON *artistName = cJSON_GetObjectItemCaseSensitive(topHitValue, "name");
    cJSON *artistPicture = cJSON_GetObjectItemCaseSensitive(topHitValue, "picture");
    cJSON *artistPopularity = cJSON_GetObjectItemCaseSensitive(topHitValue, "popularity");

    Value.artistId[100] = artistId->valueint;
    strncpy(Value.artistName[100], artistName->valuestring, sizeof(Value.artistName[100]));
    strncpy(Value.artistPicture[100], artistPicture->valuestring, sizeof(Value.artistPicture[100]));
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
    cJSON *albumArtistArray = cJSON_GetArrayItem(albumArtist, 0); 
    cJSON *albumArtistId = cJSON_GetObjectItemCaseSensitive(albumArtistArray, "id");
    cJSON *albumArtistName = cJSON_GetObjectItemCaseSensitive(albumArtistArray, "name");

    Value.albumId[100] = albumId->valueint;
    strncpy(Value.albumTitle[100], albumTitle->valuestring, sizeof(Value.albumTitle[100]));
    strncpy(Value.albumCover[100], albumCover->valuestring, sizeof(Value.albumCover[100]));
    Value.albumPopularity[100] = albumPopularity->valueint;
    Value.albumDuration[100] = albumDuration->valueint;
    Value.albumArtistId[100] = albumArtistId->valueint;
    strncpy(Value.albumArtistName[100], albumArtistName->valuestring, sizeof(Value.albumArtistName[100]));
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
    cJSON *tracksArtistArray = cJSON_GetArrayItem(tracksArtist, 0); 
    cJSON *tracksArtistId = cJSON_GetObjectItemCaseSensitive(tracksArtistArray, "id");
    cJSON *tracksArtistName = cJSON_GetObjectItemCaseSensitive(tracksArtistArray, "name");

    Value.tracksId[100] = tracksId->valueint;
    strncpy(Value.tracksTitle[100], tracksTitle->valuestring, sizeof(Value.tracksTitle[100]));
    Value.tracksDuration[100] = tracksDuration->valueint;
    Value.tracksAlbumId[100] = tracksAlbumId->valueint;
    strncpy(Value.tracksAlbumTitle[100], tracksAlbumTitle->valuestring, sizeof(Value.tracksAlbumTitle[100]));
    strncpy(Value.tracksCover[100], tracksAlbumCover->valuestring, sizeof(Value.tracksCover[100]));
    strncpy(Value.tracksReleaseDate[100], tracksReleaseDate->valuestring, sizeof(Value.tracksReleaseDate[100]));
    Value.tracksArtistId[100] = tracksArtistId->valueint;
    strncpy(Value.tracksArtistName[100], tracksArtistName->valuestring, sizeof(Value.tracksArtistName[100]));
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
    strncpy(Value.videoTitle[100], videoTitle->valuestring, sizeof(Value.videoTitle[100]));
    strncpy(Value.videoImageId[100], videoImageId->valuestring, sizeof(Value.videoImageId[100]));
    strncpy(Value.videoReleaseDate[100], videoReleaseDate->valuestring, sizeof(Value.videoReleaseDate[100]));
    Value.videoDuration[100] = videoDuration->valueint;
    Value.videoArtistId[100] = videoArtistId->valueint;
    strncpy(Value.videoArtistName[100], videoArtistName->valuestring, sizeof(Value.videoArtistName[100]));
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

    strncpy(Value.playlistUUID[100], playlistUuid->valuestring, sizeof(Value.playlistUUID[100]));
    strncpy(Value.playlistTitle[100], playlistTitle->valuestring, sizeof(Value.playlistTitle[100]));
    strncpy(Value.playlistImage[100], playlistImage->valuestring, sizeof(Value.playlistImage[100]));
    Value.playlistDuration[100] = playlistDuration->valueint;
    strncpy(Value.playlistSquareImage[100], playlistSquareImage->valuestring, sizeof(Value.playlistSquareImage[100]));
    Value.playlistNumberOfTracks[100] = playlistNumberOfTracks->valueint;
    Value.playlistNumberOfVideos[100] = playlistNumberOfVideos->valueint;
    Value.playlistPopularity[100] = playlistPopularity->valueint;
  }
  return Value;*/
}
