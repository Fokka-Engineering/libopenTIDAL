#include <stdio.h>
#include <stdlib.h>
#include "../include/openTIDAL.h"

json_playlist_model json_parse_playlist(cJSON *directObject)
{
  json_playlist_model Value;

  Value.uuid = cJSON_GetObjectItemCaseSensitive(directObject, "uuid");
  Value.title = cJSON_GetObjectItemCaseSensitive(directObject, "title");
  Value.description = cJSON_GetObjectItemCaseSensitive(directObject, "description");
  Value.popularity = cJSON_GetObjectItemCaseSensitive(directObject, "popularity");
  Value.duration = cJSON_GetObjectItemCaseSensitive(directObject, "duration");
  Value.created = cJSON_GetObjectItemCaseSensitive(directObject, "created");
  Value.lastUpdated = cJSON_GetObjectItemCaseSensitive(directObject, "lastUpdated");
  Value.numberOfTracks = cJSON_GetObjectItem(directObject, "numberOfTracks");
  Value.numberOfVideos = cJSON_GetObjectItem(directObject, "numberOfVideos");
  Value.publicPlaylist = cJSON_GetObjectItem(directObject, "publicPlaylist");
  Value.image = cJSON_GetObjectItemCaseSensitive(directObject, "image");
  Value.squareImage = cJSON_GetObjectItemCaseSensitive(directObject, "squareImage");
  Value.type = cJSON_GetObjectItemCaseSensitive(directObject, "type");

  return Value;
}

json_items_model json_parse_items(cJSON *directObject)
{
  json_items_model Value;
  
  cJSON *album = cJSON_GetObjectItem(directObject, "album");

  Value.id = cJSON_GetObjectItem(directObject, "id");
  Value.title = cJSON_GetObjectItemCaseSensitive(directObject, "title");
  Value.duration = cJSON_GetObjectItem(directObject, "duration");
  Value.popularity = cJSON_GetObjectItem(directObject, "popularity");
  Value.trackNumber = cJSON_GetObjectItem(directObject, "trackNumber");
  Value.volumeNumber = cJSON_GetObjectItem(directObject, "volumeNumber");
  Value.version = cJSON_GetObjectItemCaseSensitive(directObject, "version");
  Value.releaseDate = cJSON_GetObjectItem(directObject, "releaseDate");
  Value.cover = cJSON_GetObjectItemCaseSensitive(album, "cover");
  Value.videoCover = cJSON_GetObjectItemCaseSensitive(album, "videoCover");
  Value.imageId = cJSON_GetObjectItemCaseSensitive(directObject, "imageId");
  Value.audioQuality = cJSON_GetObjectItemCaseSensitive(directObject, "audioQuality");
  Value.quality = cJSON_GetObjectItemCaseSensitive(directObject, "quality");
  Value.explicitItem = cJSON_GetObjectItem(directObject, "explicit");
  Value.allowStreaming = cJSON_GetObjectItem(directObject, "allowStreaming");
  Value.streamReady = cJSON_GetObjectItem(directObject, "streamReady");
  Value.replayGain = cJSON_GetObjectItem(directObject, "replayGain");
  Value.peak = cJSON_GetObjectItem(directObject, "peak");
  Value.artists = cJSON_GetObjectItem(directObject, "artists");
  Value.albumId = cJSON_GetObjectItem(album, "id");
  Value.albumTitle = cJSON_GetObjectItemCaseSensitive(album, "title");
  Value.type = cJSON_GetObjectItemCaseSensitive(directObject, "type");

  return Value;
}

json_album_model json_parse_album(cJSON *directObject)
{
  json_album_model Value;
  
  Value.id = cJSON_GetObjectItem(directObject, "id");
  Value.title = cJSON_GetObjectItemCaseSensitive(directObject, "title");
  Value.duration = cJSON_GetObjectItem(directObject, "duration");
  Value.popularity = cJSON_GetObjectItem(directObject, "popularity");
  Value.copyright = cJSON_GetObjectItemCaseSensitive(directObject, "copyright");
  Value.quality = cJSON_GetObjectItemCaseSensitive(directObject, "audioQuality");
  Value.cover = cJSON_GetObjectItemCaseSensitive(directObject, "cover");
  Value.videoCover = cJSON_GetObjectItemCaseSensitive(directObject, "videoCover");
  Value.releaseDate = cJSON_GetObjectItemCaseSensitive(directObject, "releaseDate");
  Value.version = cJSON_GetObjectItemCaseSensitive(directObject, "version");
  Value.explicitItem = cJSON_GetObjectItem(directObject, "explicit");
  Value.allowStreaming = cJSON_GetObjectItem(directObject, "allowStreaming");
  Value.streamReady = cJSON_GetObjectItem(directObject, "streamReady");
  Value.numberOfTracks = cJSON_GetObjectItem(directObject, "numberOfTracks");
  Value.numberOfVideos = cJSON_GetObjectItem(directObject, "numberOfVideos");
  Value.numberOfVolumes = cJSON_GetObjectItem(directObject, "numberOfVolumes");
  Value.artists = cJSON_GetObjectItem(directObject, "artists");

  return Value;
}

json_artist_model json_parse_artist(cJSON *directObject)
{
  json_artist_model Value;

  Value.id = cJSON_GetObjectItem(directObject, "id");
  Value.name = cJSON_GetObjectItemCaseSensitive(directObject, "name");
  Value.picture = cJSON_GetObjectItemCaseSensitive(directObject, "picture");
  Value.popularity = cJSON_GetObjectItem(directObject, "popularity");

  return Value;
}

json_links_model json_parse_links(cJSON *directObject)
{
  json_links_model Value;

  Value.url = cJSON_GetObjectItemCaseSensitive(directObject, "url");
  Value.siteName = cJSON_GetObjectItemCaseSensitive(directObject, "siteName");

  return Value;
}

json_credit_model json_parse_credits(cJSON *directObject)
{
  json_credit_model Value;

  Value.type = cJSON_GetObjectItemCaseSensitive(directObject, "type");
  Value.name = cJSON_GetObjectItemCaseSensitive(directObject, "name");
  Value.id = cJSON_GetObjectItemCaseSensitive(directObject, "id");

  return Value;
}

json_contributor_model json_parse_contributors(cJSON *directObject)
{
  json_contributor_model Value;

  Value.name = cJSON_GetObjectItemCaseSensitive(directObject, "name");
  Value.role = cJSON_GetObjectItemCaseSensitive(directObject, "role"); 

  return Value;
}

json_mix_model json_parse_mix(cJSON *directObject)
{
  json_mix_model Value;

  Value.id = cJSON_GetObjectItemCaseSensitive(directObject, "id");

  return Value;
}

json_stream_model json_parse_stream(cJSON *directObject)
{
  json_stream_model Value;

  Value.trackId = cJSON_GetObjectItem(directObject, "trackId");
  Value.videoId = cJSON_GetObjectItem(directObject, "videoId");
  Value.assetPresentation = cJSON_GetObjectItemCaseSensitive(directObject, "assetPresentation");
  Value.audioQuality = cJSON_GetObjectItemCaseSensitive(directObject, "audioQuality");
  Value.audioMode = cJSON_GetObjectItemCaseSensitive(directObject, "audioMode");
  Value.videoQuality = cJSON_GetObjectItemCaseSensitive(directObject, "videoQuality");
  Value.manifestMimeType = cJSON_GetObjectItemCaseSensitive(directObject, "manifestMimeType");
  Value.manifest = cJSON_GetObjectItemCaseSensitive(directObject, "manifest");

  return Value;
}

json_manifest_model json_parse_manifest(cJSON *directObject)
{
  json_manifest_model Value;

  Value.mimeType = cJSON_GetObjectItemCaseSensitive(directObject, "mimeType");
  Value.codec = cJSON_GetObjectItemCaseSensitive(directObject, "codecs");
  Value.encryptionType = cJSON_GetObjectItemCaseSensitive(directObject, "encryptionType");
  Value.urls = cJSON_GetObjectItemCaseSensitive(directObject, "urls");
  Value.url = cJSON_GetArrayItem(Value.urls, 0);

  return Value;
}
