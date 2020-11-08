#include "cJSON.h"
#pragma once

/* structures used to parse a json response */

typedef struct json_user_models
{
  cJSON *id;
  cJSON *firstName;
  cJSON *lastName;
  cJSON *email;
  cJSON *countryCode;
  cJSON *created;
  cJSON *picture;
  cJSON *gender;
  cJSON *dateOfBirth;
} json_user_model;

typedef struct json_playlist_models
{
  /* Pagination Values */
  cJSON *totalNumberOfItems;
  cJSON *limit;
  cJSON *offset;
  /* Playlist Values */
  cJSON *uuid;
  cJSON *title;
  cJSON *description;
  cJSON *popularity;
  cJSON *duration;
  cJSON *created;
  cJSON *lastUpdated;
  cJSON *numberOfTracks;
  cJSON *numberOfVideos;
  cJSON *publicPlaylist;
  cJSON *image;
  cJSON *squareImage;
  cJSON *type;
} json_playlist_model;

typedef struct json_items_models
{
  /* Pagination Values */
  cJSON *totalNumberOfItems;
  cJSON *limit;
  cJSON *offset;
  /* Items Values */
  cJSON *id;
  cJSON *title;
  cJSON *duration;
  cJSON *popularity;
  cJSON *trackNumber;
  cJSON *volumeNumber;
  cJSON *version;
  cJSON *releaseDate;
  cJSON *cover;
  cJSON *videoCover;
  cJSON *imageId;
  cJSON *quality;
  cJSON *audioQuality;
  cJSON *explicitItem;
  cJSON *allowStreaming;
  cJSON *streamReady;
  cJSON *replayGain;
  cJSON *peak;
  cJSON *artists;
  cJSON *albumId;
  cJSON *albumTitle;
  cJSON *type;
} json_items_model;

typedef struct json_album_models
{
  /* Pagination Values */
  cJSON *totalNumberOfItems;
  cJSON *limit;
  cJSON *offset;
  /* Album Values */
  cJSON *id;
  cJSON *title;
  cJSON *duration;
  cJSON *popularity;
  cJSON *copyright;
  cJSON *quality;
  cJSON *cover;
  cJSON *videoCover;
  cJSON *releaseDate;
  cJSON *version;
  cJSON *explicitItem;
  cJSON *allowStreaming;
  cJSON *streamReady;
  cJSON *numberOfTracks;
  cJSON *numberOfVideos;
  cJSON *numberOfVolumes;
  cJSON *artists;
} json_album_model;

typedef struct json_artist_models
{
  /* Pagination Values */
  cJSON *totalNumberOfItems;
  cJSON *limit;
  cJSON *offset;
  /* Artist Values */
  cJSON *id;
  cJSON *name;
  cJSON *picture;
  cJSON *popularity;
} json_artist_model;

typedef struct json_link_models
{
  /* Pagination Values */
  cJSON *totalNumberOfItems;
  cJSON *limit;
  cJSON *offset;
  /* Link Values */
  cJSON *url;
  cJSON *siteName;
  cJSON *source;
} json_links_model;

typedef struct json_credit_models
{
  /* Pagination Values */
  cJSON *totalNumberOfItems;
  cJSON *limit;
  cJSON *offset;
  /* Credit Values */
  cJSON *type;
  cJSON *name;
  cJSON *id;
} json_credit_model;

typedef struct json_contributor_models
{
  /* Pagination Values */
  cJSON *totalNumberOfItems;
  cJSON *limit;
  cJSON *offset;
  /* Contributor Values */
  cJSON *name;
  cJSON *role;
} json_contributor_model;

typedef struct json_mix_models
{
  cJSON *id;
} json_mix_model;

typedef struct json_stream_models
{
  cJSON *trackId;
  cJSON *videoId;
  cJSON *assetPresentation;
  cJSON *audioQuality;
  cJSON *audioMode;
  cJSON *videoQuality;
  cJSON *manifestMimeType;
  cJSON *manifest;
} json_stream_model;

typedef struct json_manifest_models
{
  cJSON *mimeType;
  cJSON *codec;
  cJSON *encryptionType;
  cJSON *urls;
  cJSON *url;
} json_manifest_model;

typedef struct json_page_mix_models
{
  /* Pagination Values */
  cJSON *totalNumberOfItems;
  cJSON *limit;
  cJSON *offset;
  /* Mix Values */
  cJSON *id;
  cJSON *title;
  cJSON *subTitle;
  cJSON *smallImageUrl;
  cJSON *mediumImageUrl;
  cJSON *largeImageUrl;
} json_page_mix_model;
