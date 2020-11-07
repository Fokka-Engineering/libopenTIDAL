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
  cJSON *artistLimit = cJSON_GetObjectItem(artists, "limit");
  cJSON *artistOffset = cJSON_GetObjectItem(artists, "offset");
  Value.artists.limit = artistLimit->valueint;
  Value.artists.offset = artistOffset->valueint;
  Value.artists.totalNumberOfItems = artistTotalNumberOfItems->valueint;
  Value.artists.arraySize = cJSON_GetArraySize(artistsItems);

  cJSON *albums = cJSON_GetObjectItem(input_json, "albums");
  cJSON *albumsItems = cJSON_GetObjectItem(albums, "items");
  cJSON *albumTotalNumberOfItems = cJSON_GetObjectItem(albums, "totalNumberOfItems");
  cJSON *albumsItem = NULL;
  cJSON *albumLimit = cJSON_GetObjectItem(albums, "limit");
  cJSON *albumOffset = cJSON_GetObjectItem(albums, "offset");
  Value.albums.limit = albumLimit->valueint;
  Value.albums.offset = albumOffset->valueint;
  Value.albums.totalNumberOfItems = albumTotalNumberOfItems->valueint;
  Value.albums.arraySize = cJSON_GetArraySize(albumsItems);

  cJSON *playlists = cJSON_GetObjectItem(input_json, "playlists");
  cJSON *playlistsItems = cJSON_GetObjectItem(playlists, "items");
  cJSON *playlistsTotalNumberOfItems = cJSON_GetObjectItem(playlists, "totalNumberOfItems");
  cJSON *playlistsItem = NULL;
  cJSON *playlistsLimit = cJSON_GetObjectItem(playlists, "limit");
  cJSON *playlistsOffset = cJSON_GetObjectItem(playlists, "offset");
  Value.playlists.limit = playlistsLimit->valueint;
  Value.playlists.offset = playlistsOffset->valueint;
  Value.playlists.totalNumberOfItems = playlistsTotalNumberOfItems->valueint;
  Value.playlists.arraySize = cJSON_GetArraySize(playlistsItems);
 
  cJSON *tracks = cJSON_GetObjectItem(input_json, "tracks");
  cJSON *tracksItems = cJSON_GetObjectItem(tracks, "items");
  cJSON *tracksTotalNumberOfItems = cJSON_GetObjectItem(tracks, "totalNumberOfItems");
  cJSON *tracksItem = NULL;
  cJSON *tracksLimit = cJSON_GetObjectItem(tracks, "limit");
  cJSON *tracksOffset = cJSON_GetObjectItem(tracks, "offset");
  Value.tracks.limit = tracksLimit->valueint;
  Value.tracks.offset = tracksOffset->valueint;
  Value.tracks.totalNumberOfItems = tracksTotalNumberOfItems->valueint;
  Value.tracks.arraySize = cJSON_GetArraySize(tracksItems);
 
  cJSON *videos = cJSON_GetObjectItem(input_json, "videos");
  cJSON *videosItems = cJSON_GetObjectItem(videos, "items");
  cJSON *videosTotalNumberOfItems = cJSON_GetObjectItem(videos, "totalNumberOfItems");
  cJSON *videosItem = NULL;
  cJSON *videosLimit = cJSON_GetObjectItem(videos, "limit");
  cJSON *videosOffset = cJSON_GetObjectItem(videos, "offset");
  Value.videos.limit = videosLimit->valueint;
  Value.videos.offset = videosOffset->valueint;
  Value.videos.totalNumberOfItems = videosTotalNumberOfItems->valueint;
  Value.videos.arraySize = cJSON_GetArraySize(videosItems);
 
  cJSON *topHit = cJSON_GetObjectItemCaseSensitive(input_json, "topHit");
  cJSON *topHitValue = cJSON_GetObjectItemCaseSensitive(topHit, "value");
  Value.isTopAlbum = 0;
  Value.isTopArtist = 0;
  Value.isTopTrack = 0;
  Value.isTopPlaylist = 0;
  Value.isTopVideo = 0;
  Value.status = 1;

  if (artistTotalNumberOfItems != 0)
  {
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
      cJSON *releaseDate = cJSON_GetObjectItemCaseSensitive(albumsItem, "releaseDate");
      cJSON *explicitItem = cJSON_GetObjectItem(albumsItem, "explicit");
      cJSON *allowStreaming = cJSON_GetObjectItem(albumsItem, "allowStreaming");
      cJSON *streamReady = cJSON_GetObjectItem(albumsItem, "streamReady");
      cJSON *audioQuality = cJSON_GetObjectItemCaseSensitive(albumsItem, "audioQuality");
      cJSON *copyright = cJSON_GetObjectItemCaseSensitive(albumsItem, "copyright");
      cJSON *cover = cJSON_GetObjectItemCaseSensitive(albumsItem, "cover");
      cJSON *popularity = cJSON_GetObjectItem(albumsItem, "popularity");
      cJSON *artist = cJSON_GetObjectItem(albumsItem, "artists");

      Value.albums.id[al] = id->valueint;
      strncpy(Value.albums.title[al], title->valuestring, sizeof(Value.albums.title[al]));
      Value.albums.duration[al] = duration->valueint;
      Value.albums.numberOfTracks[al] = numberOfTracks->valueint;
      Value.albums.numberOfVideos[al] = numberOfVideos->valueint;
      Value.albums.numberOfVolumes[al] = numberOfVolumes->valueint;
      Value.albums.explicitItem[al] = cJSON_IsTrue(explicitItem);
      Value.albums.allowStreaming[al] = cJSON_IsTrue(allowStreaming);
      Value.albums.streamReady[al] = cJSON_IsTrue(streamReady);
      strncpy(Value.albums.releaseDate[al], releaseDate->valuestring, sizeof(Value.albums.releaseDate[al]));
      strncpy(Value.albums.quality[al], audioQuality->valuestring, sizeof(Value.albums.quality[al]));
      strncpy(Value.albums.copyright[al], copyright->valuestring, sizeof(Value.albums.copyright[al]));
      strncpy(Value.albums.cover[al], cover->valuestring, sizeof(Value.albums.cover[al]));
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
      cJSON *publicPlaylist = cJSON_GetObjectItem(playlistsItem, "publicPlaylist");
      cJSON *type = cJSON_GetObjectItemCaseSensitive(playlistsItem, "type");
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
      Value.playlists.publicPlaylist[pl] = cJSON_IsTrue(publicPlaylist);
      strncpy(Value.playlists.type[pl], type->valuestring, sizeof(Value.playlists.type[pl]));
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
      cJSON *explicitItem = cJSON_GetObjectItem(tracksItem, "explicit");
      cJSON *allowStreaming = cJSON_GetObjectItem(tracksItem, "allowStreaming");
      cJSON *streamReady = cJSON_GetObjectItem(tracksItem, "streamReady");
      cJSON *replayGain = cJSON_GetObjectItem(tracksItem, "replayGain");
      cJSON *peak = cJSON_GetObjectItem(tracksItem, "peak");
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
      Value.tracks.explicitItem[tr] = cJSON_IsTrue(explicitItem);
      Value.tracks.allowStreaming[tr] = cJSON_IsTrue(allowStreaming);
      Value.tracks.streamReady[tr] = cJSON_IsTrue(streamReady);
      Value.tracks.replayGain[tr] = replayGain->valuedouble;
      Value.tracks.peak[tr] = peak->valuedouble;
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
    cJSON_ArrayForEach(videosItem, videosItems)
    {
      int i = 0;
      cJSON *subItem = NULL;
      cJSON *id = cJSON_GetObjectItem(videosItem, "id");
      cJSON *title = cJSON_GetObjectItemCaseSensitive(videosItem, "title");
      cJSON *duration = cJSON_GetObjectItem(videosItem, "duration");
      cJSON *imageId = cJSON_GetObjectItemCaseSensitive(videosItem, "imageId");
      cJSON *releaseDate = cJSON_GetObjectItemCaseSensitive(videosItem, "releaseDate");
      cJSON *explicitItem = cJSON_GetObjectItem(videosItem, "explicit");
      cJSON *allowStreaming = cJSON_GetObjectItem(videosItem, "allowStreaming");
      cJSON *streamReady = cJSON_GetObjectItem(videosItem, "streamReady");
      cJSON *popularity = cJSON_GetObjectItemCaseSensitive(videosItem, "popularity");
      cJSON *artist = cJSON_GetObjectItemCaseSensitive(videosItem, "artists");

      Value.videos.id[vi] = id->valueint;
      strncpy(Value.videos.title[vi], title->valuestring, sizeof(Value.videos.title[vi]));
      Value.videos.duration[vi] = duration->valueint;
      Value.videos.popularity[vi] = popularity->valueint;
      strncpy(Value.videos.cover[vi], imageId->valuestring, sizeof(Value.videos.cover[vi]));
      strncpy(Value.videos.releaseDate[vi], releaseDate->valuestring, sizeof(Value.videos.releaseDate[vi]));
      Value.videos.explicitItem[vi] = cJSON_IsTrue(explicitItem);
      Value.videos.allowStreaming[vi] = cJSON_IsTrue(allowStreaming);
      Value.videos.streamReady[vi] = cJSON_IsTrue(streamReady);
      Value.videos.subArraySize[vi] = cJSON_GetArraySize(artist);
      cJSON_ArrayForEach(subItem, artist)
      {
        cJSON *artistId = cJSON_GetObjectItem(subItem, "id");
        cJSON *artistName = cJSON_GetObjectItemCaseSensitive(subItem, "name");

        Value.videos.artistId[vi][i] = artistId->valueint;
        strncpy(Value.videos.artistName[vi][i], artistName->valuestring, sizeof(Value.videos.artistName[vi][i]));
        i += 1;
      }
      vi += 1;
    }
  }
  /* Parse TopHit Entry  */
  if (cJSON_IsObject(topHit) && cJSON_IsNull(topHit) != 1)
  { 
    strncpy(Value.topHitType, cJSON_GetObjectItemCaseSensitive(topHit, "type")->valuestring, sizeof(Value.topHitType));
  }
  else
  {
    strcpy(Value.topHitType, "0");
  }
  if (strcmp(Value.topHitType, "ARTISTS") == 0)
  {
    cJSON *topArtistsId = cJSON_GetObjectItem(topHitValue, "id");
    cJSON *topArtistsName = cJSON_GetObjectItemCaseSensitive(topHitValue, "name");
    cJSON *topArtistsPicture = cJSON_GetObjectItemCaseSensitive(topHitValue, "picture");
    cJSON *topArtistsPopularity = cJSON_GetObjectItem(topHitValue, "popularity");

    Value.isTopArtist = 1;
    Value.topArtist.id[0] = topArtistsId->valueint;
    strncpy(Value.topArtist.name[0], topArtistsName->valuestring, sizeof(Value.topArtist.name[0]));
    Value.topArtist.hasPicture[0] = 0;
    if (cJSON_IsNull(topArtistsPicture) != 1)
    {
      Value.topArtist.hasPicture[0] = 1;
      strncpy(Value.topArtist.picture[0], topArtistsPicture->valuestring, sizeof(Value.topArtist.picture[0]));
    }
    Value.topArtist.popularity[0] = topArtistsPopularity->valueint;
  }
  else if (strcmp(Value.topHitType, "ALBUMS") == 0)
  {
    int i = 0; 
    cJSON *topAlbumSubItem = NULL;
    cJSON *topAlbumId = cJSON_GetObjectItem(topHitValue, "id");
    cJSON *topAlbumTitle = cJSON_GetObjectItemCaseSensitive(topHitValue, "title");
    cJSON *topAlbumDuration = cJSON_GetObjectItem(topHitValue, "duration");
    cJSON *topAlbumNumberOfTracks = cJSON_GetObjectItem(topHitValue, "numberOfTracks");
    cJSON *topAlbumNumberOfVideos = cJSON_GetObjectItem(topHitValue, "numberOfVideos");
    cJSON *topAlbumNumberOfVolumes = cJSON_GetObjectItem(topHitValue, "numberOfVolumes");
    cJSON *topAlbumCopyright = cJSON_GetObjectItemCaseSensitive(topHitValue, "copyright");
    cJSON *topAlbumCover = cJSON_GetObjectItemCaseSensitive(topHitValue, "cover");
    cJSON *topAlbumPopularity = cJSON_GetObjectItem(topHitValue, "popularity");
    cJSON *topAlbumExplicitItem = cJSON_GetObjectItem(topHitValue, "explicit");
    cJSON *topAlbumAllowStreaming = cJSON_GetObjectItem(topHitValue, "allowStreaming");
    cJSON *topAlbumStreamReady = cJSON_GetObjectItem(topHitValue, "streamReady");
    cJSON *topAlbumAudioQuality = cJSON_GetObjectItem(topHitValue, "audioQuality");
    cJSON *topAlbumVersion = cJSON_GetObjectItemCaseSensitive(topHitValue, "version");
    cJSON *topAlbumReleaseDate = cJSON_GetObjectItem(topHitValue, "releaseDate");
    cJSON *topAlbumArtist = cJSON_GetObjectItem(topHitValue, "artists");

    Value.isTopAlbum = 1;
    Value.topAlbum.id[0] = topAlbumId->valueint;
    strncpy(Value.topAlbum.title[0], topAlbumTitle->valuestring, sizeof(Value.topAlbum.title[0]));
    Value.topAlbum.duration[0] = topAlbumDuration->valueint;
    Value.topAlbum.numberOfTracks[0] = topAlbumNumberOfTracks->valueint;
    Value.topAlbum.numberOfVideos[0] = topAlbumNumberOfVideos->valueint;
    Value.topAlbum.numberOfVolumes[0] = topAlbumNumberOfVolumes->valueint;
    Value.topAlbum.explicitItem[0] = cJSON_IsTrue(topAlbumExplicitItem);
    Value.topAlbum.allowStreaming[0] = cJSON_IsTrue(topAlbumAllowStreaming);
    Value.topAlbum.streamReady[0] = cJSON_IsTrue(topAlbumStreamReady);
    Value.topAlbum.hasVersion[0] = 0;
    strncpy(Value.topAlbum.quality[0], topAlbumAudioQuality->valuestring, sizeof(Value.topAlbum.quality[0]));
    strncpy(Value.topAlbum.releaseDate[0], topAlbumReleaseDate->valuestring, sizeof(Value.topAlbum.releaseDate[0]));
    strncpy(Value.topAlbum.copyright[0], topAlbumCopyright->valuestring, sizeof(Value.topAlbum.copyright[0]));
    strncpy(Value.topAlbum.cover[0], topAlbumCover->valuestring, sizeof(Value.topAlbum.cover[0]));
    strncpy(Value.topAlbum.releaseDate[0], topAlbumReleaseDate->valuestring, sizeof(Value.topAlbum.releaseDate[0]));
    Value.topAlbum.popularity[0] = topAlbumPopularity->valueint;
    if (cJSON_IsObject(topAlbumVersion) && cJSON_IsNull(topAlbumVersion) != 1)
    {
      Value.topAlbum.hasVersion[0] = 1;
      strncpy(Value.topAlbum.version[0], topAlbumVersion->valuestring, sizeof(Value.topAlbum.version[0]));
    } 

    Value.topAlbum.subArraySize[0] = cJSON_GetArraySize(topAlbumArtist);
    cJSON_ArrayForEach(topAlbumSubItem, topAlbumArtist)
    {
      cJSON *topAlbumArtistId = cJSON_GetObjectItemCaseSensitive(topAlbumSubItem, "id");
      cJSON *topAlbumArtistName = cJSON_GetObjectItemCaseSensitive(topAlbumSubItem, "name");

      Value.topAlbum.artistId[0][i] = topAlbumArtistId->valueint;
      strncpy(Value.topAlbum.artistName[0][i], topAlbumArtistName->valuestring, sizeof(Value.topAlbum.artistName[0][i]));
      i += 1;
    }
  }
  else if (strcmp(Value.topHitType, "TRACKS") == 0)
  {
    int i = 0;
    cJSON *topTrackSubItem = NULL;
    cJSON *topTrackId = cJSON_GetObjectItem(topHitValue, "id");
    cJSON *topTrackTitle = cJSON_GetObjectItemCaseSensitive(topHitValue, "title");
    cJSON *topTrackDuration = cJSON_GetObjectItem(topHitValue, "duration");
    cJSON *topTrackAlbum = cJSON_GetObjectItem(topHitValue, "album");
    cJSON *topTrackReleaseDate = cJSON_GetObjectItemCaseSensitive(topTrackAlbum, "releaseDate");
    cJSON *topTrackPopularity = cJSON_GetObjectItemCaseSensitive(topHitValue, "popularity");
    cJSON *topTrackVersion = cJSON_GetObjectItemCaseSensitive(topHitValue, "version");
    cJSON *topTrackAlbumId = cJSON_GetObjectItem(topTrackAlbum, "id");
    cJSON *topTrackAlbumTitle = cJSON_GetObjectItemCaseSensitive(topTrackAlbum, "title");
    cJSON *topTrackAlbumCover = cJSON_GetObjectItemCaseSensitive(topTrackAlbum, "cover");
    cJSON *topTrackArtist = cJSON_GetObjectItem(topHitValue, "artists");

    Value.isTopTrack = 1;
    Value.topTrack.id[0] = topTrackId->valueint;
    strncpy(Value.topTrack.title[0], topTrackTitle->valuestring, sizeof(Value.topTrack.title[0]));
    Value.topTrack.duration[0] = topTrackDuration->valueint;
    Value.topTrack.popularity[0] = topTrackPopularity->valueint;
    Value.topTrack.hasVersion[0] = 0;
    if (cJSON_IsObject(topTrackVersion) && cJSON_IsNull(topTrackVersion) != 1)
    {
      Value.topTrack.hasVersion[0] = 1;
      strncpy(Value.topTrack.version[0], topTrackVersion->valuestring, sizeof(Value.topTrack.version[0]));
    }
    Value.topTrack.albumId[0] = topTrackAlbumId->valueint;
    strncpy(Value.topTrack.albumTitle[0], topTrackAlbumTitle->valuestring, sizeof(Value.topTrack.albumTitle[0]));
    strncpy(Value.topTrack.cover[0], topTrackAlbumCover->valuestring, sizeof(Value.topTrack.cover[0]));
    strncpy(Value.topTrack.releaseDate[0], topTrackReleaseDate->valuestring, sizeof(Value.topTrack.releaseDate[0]));
      
    Value.topTrack.subArraySize[0] = cJSON_GetArraySize(topTrackArtist);
    cJSON_ArrayForEach(topTrackSubItem, topTrackArtist)
    {
      cJSON *topTrackArtistId = cJSON_GetObjectItem(topTrackSubItem, "id");
      cJSON *topTrackArtistName = cJSON_GetObjectItemCaseSensitive(topTrackSubItem, "name");

      Value.topTrack.artistId[0][i] = topTrackArtistId->valueint;
      strncpy(Value.topTrack.artistName[0][i], topTrackArtistName->valuestring, sizeof(Value.topTrack.artistName[0][i]));
      i += 1;
    }
  }
  else if (strcmp(Value.topHitType, "VIDEOS") == 0)
  {
    int i = 0;
    cJSON *topVideoItem = NULL;
    cJSON *topVideoId = cJSON_GetObjectItem(topHitValue, "id");
    cJSON *topVideoTitle = cJSON_GetObjectItemCaseSensitive(topHitValue, "title");
    cJSON *topVideoDuration = cJSON_GetObjectItem(topHitValue, "duration");
    cJSON *topVideoImageId = cJSON_GetObjectItemCaseSensitive(topHitValue, "imageId");
    cJSON *topVideoReleaseDate = cJSON_GetObjectItemCaseSensitive(topHitValue, "releaseDate");
    cJSON *topVideoPopularity = cJSON_GetObjectItemCaseSensitive(topHitValue, "popularity");
    cJSON *topVideoArtist = cJSON_GetObjectItemCaseSensitive(topHitValue, "artists");

    Value.isTopVideo = 1;
    Value.topVideo.id[0] = topVideoId->valueint;
    strncpy(Value.topVideo.title[0], topVideoTitle->valuestring, sizeof(Value.topVideo.title[0]));
    Value.topVideo.duration[0] = topVideoDuration->valueint;
    Value.topVideo.popularity[0] = topVideoPopularity->valueint;
    strncpy(Value.topVideo.cover[0], topVideoImageId->valuestring, sizeof(Value.topVideo.cover[0]));
    strncpy(Value.topVideo.releaseDate[0], topVideoReleaseDate->valuestring, sizeof(Value.topVideo.releaseDate[0]));
    
    Value.topVideo.subArraySize[0] = cJSON_GetArraySize(topVideoArtist);
    cJSON_ArrayForEach(topVideoItem, topVideoArtist)
    {
      cJSON *topVideoArtistId = cJSON_GetObjectItem(topVideoItem, "id");
      cJSON *topVideoArtistName = cJSON_GetObjectItemCaseSensitive(topVideoItem, "name");

      Value.topVideo.artistId[0][i] = topVideoArtistId->valueint;
      strncpy(Value.topVideo.artistName[0][i], topVideoArtistName->valuestring, sizeof(Value.topVideo.artistName[0][i]));
      i += 1;
    }
  }
  else if (strcmp(Value.topHitType, "PLAYLISTS") == 0)
  {
    cJSON *topPlaylistUuid = cJSON_GetObjectItemCaseSensitive(topHitValue, "uuid");
    cJSON *topPlaylistTitle = cJSON_GetObjectItemCaseSensitive(topHitValue, "title");
    cJSON *topPlaylistDescription = cJSON_GetObjectItemCaseSensitive(topHitValue, "description");
    cJSON *topPlaylistImage = cJSON_GetObjectItemCaseSensitive(topHitValue, "image");
    cJSON *topPlaylistDuration = cJSON_GetObjectItem(topHitValue, "duration");
    cJSON *topPlaylistSquareImage = cJSON_GetObjectItemCaseSensitive(topHitValue, "squareImage");
    cJSON *topPlaylistNumberOfTracks = cJSON_GetObjectItem(topHitValue, "numberOfTracks");
    cJSON *topPlaylistNumberOfVideos = cJSON_GetObjectItem(topHitValue, "numberOfVideos");
    cJSON *topPlaylistPopularity = cJSON_GetObjectItem(topHitValue, "popularity");
    cJSON *topPlaylistCreated = cJSON_GetObjectItemCaseSensitive(topHitValue, "created");
    cJSON *topPlaylistLastUpdated = cJSON_GetObjectItemCaseSensitive(topHitValue, "lastUpdated");

    Value.isTopPlaylist = 1;
    strncpy(Value.topPlaylist.uuid[0], topPlaylistUuid->valuestring, sizeof(Value.topPlaylist.uuid[0]));
    strncpy(Value.topPlaylist.title[0], topPlaylistTitle->valuestring, sizeof(Value.topPlaylist.title[0]));
    Value.topPlaylist.hasDescription[0] = 0;
    if (cJSON_IsNull(topPlaylistDescription) != 1)
    {
      Value.topPlaylist.hasDescription[0] = 1;
      strncpy(Value.topPlaylist.description[0], topPlaylistDescription->valuestring, sizeof(Value.topPlaylist.description[0]));
    }
    strncpy(Value.topPlaylist.image[0], topPlaylistImage->valuestring, sizeof(Value.topPlaylist.image[0]));
    Value.topPlaylist.duration[0] = topPlaylistDuration->valueint;
    strncpy(Value.topPlaylist.squareImage[0], topPlaylistSquareImage->valuestring, sizeof(Value.topPlaylist.squareImage[0]));
    strncpy(Value.topPlaylist.created[0], topPlaylistCreated->valuestring, sizeof(Value.topPlaylist.created[0]));
    strncpy(Value.topPlaylist.lastUpdated[0], topPlaylistLastUpdated->valuestring, sizeof(Value.topPlaylist.lastUpdated[0]));
    Value.topPlaylist.numberOfTracks[0] = topPlaylistNumberOfTracks->valueint;
    Value.topPlaylist.numberOfVideos[0] = topPlaylistNumberOfVideos->valueint;
    Value.topPlaylist.popularity[0] = topPlaylistPopularity->valueint;

  }
  return Value;
}
