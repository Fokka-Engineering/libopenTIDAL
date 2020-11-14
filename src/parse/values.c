#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/parse.h"

/* TODO: Add Credit Parser */

login_code_model parse_login_code_values(json_login_code_model json)
{
  login_code_model Value;

  parse_string(json.deviceCode, Value.deviceCode, sizeof(Value.deviceCode));
  parse_string(json.userCode, Value.userCode, sizeof(Value.userCode));
  parse_string(json.verificationUri, Value.verificationUri, sizeof(Value.verificationUri));
  parse_string(json.verificationUriComplete, Value.verificationUriComplete, sizeof(Value.verificationUriComplete));
  parse_number(json.expiresIn, &Value.timeFrame);
  parse_number(json.interval, &Value.interval);

  return Value;
}

login_token_model parse_login_token_values(json_login_token_model json)
{
  login_token_model Value;

  parse_string(json.access_token, Value.access_token, sizeof(Value.access_token));
  parse_string(json.refresh_token, Value.refresh_token, sizeof(Value.refresh_token));
  parse_string(json.token_type, Value.token_type, sizeof(Value.token_type));
  parse_number(json.expires_in, &Value.timeFrame);
  parse_number(json.userId, &Value.userId);
  parse_string(json.email, Value.email, sizeof(Value.email));
  parse_string(json.countryCode, Value.countryCode, sizeof(Value.countryCode));
  parse_string(json.fullName, Value.fullName, sizeof(Value.fullName));
  parse_string(json.firstName, Value.firstName, sizeof(Value.firstName));
  parse_string(json.lastName, Value.lastName, sizeof(Value.lastName));
  parse_string(json.nickname, Value.nickname, sizeof(Value.nickname));
  parse_string(json.username, Value.username, sizeof(Value.username));
  parse_string(json.imageId, Value.imageId, sizeof(Value.imageId));
  parse_number(json.created, &Value.created);
  parse_number(json.updated, &Value.updated);
  parse_number(json.facebookUid, &Value.facebookUid);
  parse_string(json.appleUid, Value.appleUid, sizeof(Value.appleUid));

  return Value;
}

user_model parse_user_values(json_user_model json)
{
  user_model Value;

  parse_number(json.id, &Value.id);
  parse_string(json.username, Value.username, sizeof(Value.username));
  parse_string(json.firstName, Value.firstName, sizeof(Value.firstName));
  parse_string(json.lastName, Value.lastName, sizeof(Value.lastName));
  parse_string(json.email, Value.email, sizeof(Value.email));
  parse_string(json.countryCode, Value.countryCode, sizeof(Value.countryCode));
  parse_string(json.created, Value.created, sizeof(Value.created));
  parse_string(json.picture, Value.picture, sizeof(Value.picture));
  parse_bool(json.newsletter, &Value.newsletter);
  parse_bool(json.acceptedEULA, &Value.acceptedEULA);
  parse_string(json.gender, Value.gender, sizeof(Value.gender));
  parse_string(json.dateOfBirth, Value.dateOfBirth, sizeof(Value.dateOfBirth));
  parse_number(json.facebookUid, &Value.facebookUid);
  parse_string(json.appleUid, Value.appleUid, sizeof(Value.appleUid));

  return Value;
}

user_subscription_model parse_user_subscription_values(json_user_subscription_model json)
{
  user_subscription_model Value;

  parse_string(json.validUntil, Value.validUntil, sizeof(Value.validUntil));
  parse_string(json.status, Value.accountStatus, sizeof(Value.accountStatus));
  parse_string(json.type, Value.type, sizeof(Value.type));
  parse_number(json.offlineGracePeriod, &Value.offlineGracePeriod);
  parse_string(json.highestSoundQuality, Value.highestSoundQuality, sizeof(Value.highestSoundQuality));
  parse_bool(json.premiumAccess, &Value.premiumAccess);
  parse_bool(json.canGetTrial, &Value.canGetTrial);
  parse_string(json.paymentType, Value.paymentType, sizeof(Value.paymentType));

  return Value;
}

playlist_model parse_playlist_values(json_playlist_model json, size_t i)
{
  playlist_model Value;
  
  parse_string(json.uuid, Value.uuid[i], sizeof(Value.uuid[i]));
  parse_string(json.title, Value.title[i], sizeof(Value.title[i]));
  parse_string(json.description, Value.description[i], sizeof(Value.description[i]));
  parse_number(json.popularity, &Value.popularity[i]);
  parse_number(json.duration, &Value.duration[i]);
  parse_string(json.created, Value.created[i], sizeof(Value.created[i]));
  parse_string(json.lastUpdated, Value.lastUpdated[i], sizeof(Value.lastUpdated[i]));
  parse_number(json.numberOfTracks, &Value.numberOfTracks[i]);
  parse_number(json.numberOfVideos, &Value.numberOfVideos[i]);
  parse_bool(json.publicPlaylist, &Value.publicPlaylist[i]);
  parse_string(json.image, Value.image[i], sizeof(Value.image[i]));
  parse_string(json.squareImage, Value.squareImage[i], sizeof(Value.squareImage[i]));
  parse_string(json.type, Value.type[i], sizeof(Value.type[i]));

  return Value;
}

items_model parse_items_values(json_items_model json, size_t i)
{
  items_model Value;
  size_t ac = 0;

  parse_number(json.id, &Value.id[i]);
  parse_string(json.title, Value.title[i], sizeof(Value.title[i]));
  parse_number(json.duration, &Value.duration[i]);
  parse_number(json.popularity, &Value.popularity[i]);
  parse_number(json.trackNumber, &Value.trackNumber[i]);
  parse_number(json.volumeNumber, &Value.volumeNumber[i]);
  parse_string(json.version, Value.version[i], sizeof(Value.version[i]));
  parse_string(json.cover, Value.cover[i], sizeof(Value.cover[i]));
  parse_string(json.videoCover, Value.videoCover[i], sizeof(Value.videoCover[i]));
  parse_string(json.imageId, Value.imageId[i], sizeof(Value.imageId[i]));
  parse_string(json.audioQuality, Value.audioQuality[i], sizeof(Value.audioQuality[i]));
  parse_string(json.quality, Value.quality[i], sizeof(Value.quality[i]));
  parse_bool(json.explicitItem, &Value.explicitItem[i]);
  parse_bool(json.allowStreaming, &Value.allowStreaming[i]);
  parse_bool(json.streamReady, &Value.streamReady[i]);
  parse_double(json.replayGain, &Value.replayGain[i]);
  parse_double(json.peak, &Value.peak[i]);
  parse_number(json.albumId, &Value.albumId[i]);
  parse_string(json.albumTitle, Value.albumTitle[i], sizeof(Value.albumTitle[i]));
  parse_string(json.type, Value.type[i], sizeof(Value.type[i]));

  if (cJSON_IsArray(json.artists))
  { 
    Value.subArraySize[i] = cJSON_GetArraySize(json.artists);
    for (ac = 0; ac < Value.subArraySize[i]; ++ac)
    {
      parse_number(json.artistId[ac], &Value.artistId[i][ac]);
      parse_string(json.artistName[ac], Value.artistName[i][ac], sizeof(Value.artistName[i][ac]));
    }
  }
  else
  {
    Value.subArraySize[i] = 0;
  }

  return Value;
}

album_model parse_album_values(json_album_model json, size_t i)
{
  album_model Value;
  size_t ac = 0;

  parse_number(json.id, &Value.id[i]);
  parse_string(json.title, Value.title[i], sizeof(Value.title[i]));
  parse_number(json.duration, &Value.duration[i]);
  parse_number(json.popularity, &Value.popularity[i]);
  parse_string(json.copyright, Value.copyright[i], sizeof(Value.copyright[i]));
  parse_string(json.quality, Value.quality[i], sizeof(Value.quality[i]));
  parse_string(json.cover, Value.cover[i], sizeof(Value.cover[i]));
  parse_string(json.videoCover, Value.videoCover[i], sizeof(Value.videoCover[i]));
  parse_string(json.releaseDate, Value.releaseDate[i], sizeof(Value.releaseDate[i]));
  parse_string(json.version, Value.version[i], sizeof(Value.version[i]));
  parse_bool(json.explicitItem, &Value.explicitItem[i]);
  parse_bool(json.allowStreaming, &Value.allowStreaming[i]);
  parse_bool(json.streamReady, &Value.streamReady[i]);
  parse_number(json.numberOfTracks, &Value.numberOfTracks[i]);
  parse_number(json.numberOfVideos, &Value.numberOfVideos[i]);
  parse_number(json.numberOfVolumes, &Value.numberOfVolumes[i]);

  if (cJSON_IsArray(json.artists))
  { 
    Value.subArraySize[i] = cJSON_GetArraySize(json.artists);
    for (ac = 0; ac < Value.subArraySize[i]; ++ac)
    {
      parse_number(json.artistId[ac], &Value.artistId[i][ac]);
      parse_string(json.artistName[ac], Value.artistName[i][ac], sizeof(Value.artistName[i][ac]));
    }
  }
  else
  {
    Value.subArraySize[i] = 0;
  }

  return Value;
}

artist_model parse_artist_values(json_artist_model json, size_t i)
{
  artist_model Value;
  
  parse_number(json.id, &Value.id[i]);
  parse_string(json.name, Value.name[i], sizeof(Value.name[i]));
  parse_string(json.picture, Value.picture[i], sizeof(Value.picture[i]));
  parse_number(json.popularity, &Value.popularity[i]);

  return Value;
}

artist_link_model parse_link_values(json_links_model json, size_t i)
{
  artist_link_model Value;

  parse_string(json.url, Value.url[i], sizeof(Value.url[i]));
  parse_string(json.siteName, Value.siteName[i], sizeof(Value.siteName[i]));

  return Value;
}

contributor_model parse_contributor_values(json_contributor_model json, size_t i)
{
  contributor_model Value;

  parse_string(json.name, Value.name[i], sizeof(Value.name[i]));
  parse_string(json.role, Value.role[i], sizeof(Value.role[i]));

  return Value;
}

mix_model parse_mix_values(json_mix_model json)
{
  mix_model Value;

  parse_string(json.id, Value.id, sizeof(Value.id));

  return Value;
}

page_mix_model parse_page_mix_values(json_page_mix_model json, size_t i)
{
  page_mix_model Value;

  parse_string(json.id, Value.id[i], sizeof(Value.id[i]));
  parse_string(json.title, Value.title[i], sizeof(Value.title[i]));
  parse_string(json.subTitle, Value.subTitle[i], sizeof(Value.subTitle[i]));
  parse_number(json.smallImageWidth, &Value.smallImageWidth[i]);
  parse_number(json.smallImageHeight, &Value.smallImageHeight[i]);
  parse_string(json.smallImageUrl, Value.smallImageUrl[i], sizeof(Value.smallImageUrl[i]));
  parse_number(json.mediumImageWidth, &Value.mediumImageWidth[i]);
  parse_number(json.mediumImageHeight, &Value.mediumImageHeight[i]);
  parse_string(json.mediumImageUrl, Value.mediumImageUrl[i], sizeof(Value.mediumImageUrl[i]));
  parse_number(json.largeImageWidth, &Value.largeImageWidth[i]);
  parse_number(json.largeImageHeight, &Value.largeImageHeight[i]);
  parse_string(json.largeImageUrl, Value.largeImageUrl[i], sizeof(Value.largeImageUrl[i]));
  parse_string(json.mixType, Value.mixType[i], sizeof(Value.mixType[i]));

  return Value;
}

stream_model parse_stream_values(json_stream_model json)
{
  stream_model Value;

  parse_number(json.trackId, &Value.trackId);
  parse_number(json.videoId, &Value.videoId);
  parse_string(json.assetPresentation, Value.assetPresentation, sizeof(Value.assetPresentation));
  parse_string(json.audioQuality, Value.audioQuality, sizeof(Value.audioQuality));
  parse_string(json.audioMode, Value.audioMode, sizeof(Value.audioMode));
  parse_string(json.videoQuality, Value.videoQuality, sizeof(Value.videoQuality));
  parse_string(json.manifestMimeType, Value.manifestMimeType, sizeof(Value.manifestMimeType));
  
  /* Pointing to temporary allocated json value */ 
  if (cJSON_IsString(json.manifest) && (!cJSON_IsNull(json.manifest)))
  {
    Value.manifest = json.manifest->valuestring;
  }
  else
  {
    Value.manifest = NULL;
  }

  return Value;
}
