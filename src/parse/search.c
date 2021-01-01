/*
    Copyright (c) 2020-2021 Hugo Melder and openTIDAL contributors

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/

#include "../helper/helper.h"
#include "../helper/struct_helper.h"
#include "../openTIDAL.h"
#include "parse.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
parse_search (openTIDAL_ContentContainer *o, cJSON *input_json)
{
    cJSON *artists = cJSON_GetObjectItem (input_json, "artists");
    cJSON *artistsLimit = cJSON_GetObjectItem (artists, "limit");
    cJSON *artistsOffset = cJSON_GetObjectItem (artists, "offset");
    cJSON *artistsTotalNumberOfItems = cJSON_GetObjectItem (artists, "totalNumberOfItems");
    cJSON *artistsItems = cJSON_GetObjectItem (artists, "items");
    cJSON *artistsItem = NULL;

    cJSON *albums = cJSON_GetObjectItem (input_json, "albums");
    cJSON *albumsLimit = cJSON_GetObjectItem (albums, "limit");
    cJSON *albumsOffset = cJSON_GetObjectItem (albums, "offset");
    cJSON *albumsTotalNumberOfItems = cJSON_GetObjectItem (albums, "totalNumberOfItems");
    cJSON *albumsItems = cJSON_GetObjectItem (albums, "items");
    cJSON *albumsItem = NULL;

    cJSON *playlists = cJSON_GetObjectItem (input_json, "playlists");
    cJSON *playlistsLimit = cJSON_GetObjectItem (playlists, "limit");
    cJSON *playlistsOffset = cJSON_GetObjectItem (playlists, "offset");
    cJSON *playlistsTotalNumberOfItems = cJSON_GetObjectItem (playlists, "totalNumberOfItems");
    cJSON *playlistsItems = cJSON_GetObjectItem (playlists, "items");
    cJSON *playlistsItem = NULL;

    cJSON *tracks = cJSON_GetObjectItem (input_json, "tracks");
    cJSON *tracksLimit = cJSON_GetObjectItem (tracks, "limit");
    cJSON *tracksOffset = cJSON_GetObjectItem (tracks, "offset");
    cJSON *tracksTotalNumberOfItems = cJSON_GetObjectItem (tracks, "totalNumberOfItems");
    cJSON *tracksItems = cJSON_GetObjectItem (tracks, "items");
    cJSON *tracksItem = NULL;

    cJSON *videos = cJSON_GetObjectItem (input_json, "videos");
    cJSON *videosLimit = cJSON_GetObjectItem (videos, "limit");
    cJSON *videosOffset = cJSON_GetObjectItem (videos, "offset");
    cJSON *videosTotalNumberOfItems = cJSON_GetObjectItem (videos, "totalNumberOfItems");
    cJSON *videosItems = cJSON_GetObjectItem (videos, "items");
    cJSON *videosItem = NULL;

    if (cJSON_IsArray (artistsItems)) {
        cJSON_ArrayForEach (artistsItem, artistsItems)
        {
            openTIDAL_ArtistContainer artist;
            json_artist_model processed_json = json_parse_artist (artistsItem);

            parse_artist_values (&artist, &processed_json);
            parse_signed_number (artistsLimit, &artist.limit);
            parse_signed_number (artistsOffset, &artist.offset);
            parse_signed_number (artistsTotalNumberOfItems, &artist.totalNumberOfItems);

            openTIDAL_StructAddArtist (o, artist);
        }
    }

    if (cJSON_IsArray (albumsItems)) {
        cJSON_ArrayForEach (albumsItem, albumsItems)
        {
            openTIDAL_AlbumContainer album;
            json_album_model processed_json = json_parse_album (albumsItem);

            parse_signed_number (albumsLimit, &album.limit);
            parse_signed_number (albumsOffset, &album.offset);
            parse_signed_number (albumsTotalNumberOfItems, &album.totalNumberOfItems);
            parse_album_values (&album, &processed_json);

            openTIDAL_StructAddAlbum (o, album);
        }
    }

    if (cJSON_IsArray (playlistsItems)) {
        cJSON_ArrayForEach (playlistsItem, playlistsItems)
        {
            openTIDAL_PlaylistContainer playlist;
            json_playlist_model processed_json = json_parse_playlist (playlistsItem);

            parse_playlist_values (&playlist, &processed_json);
            parse_signed_number (playlistsLimit, &playlist.limit);
            parse_signed_number (playlistsOffset, &playlist.offset);
            parse_signed_number (playlistsTotalNumberOfItems, &playlist.totalNumberOfItems);

            openTIDAL_StructAddPlaylist (o, playlist);
        }
    }

    if (cJSON_IsArray (tracksItems)) {
        cJSON_ArrayForEach (tracksItem, tracksItems)
        {
            openTIDAL_ItemsContainer track;
            json_items_model processed_json = json_parse_items (tracksItem);

            parse_items_values (&track, &processed_json);
            parse_signed_number (tracksLimit, &track.limit);
            parse_signed_number (tracksOffset, &track.offset);
            parse_signed_number (tracksTotalNumberOfItems, &track.totalNumberOfItems);

            openTIDAL_StructAddItem (o, track);
        }
    }

    if (cJSON_IsArray (videosItems)) {
        cJSON_ArrayForEach (videosItem, videosItems)
        {
            openTIDAL_ItemsContainer video;
            json_items_model processed_json = json_parse_items (videosItem);

            parse_items_values (&video, &processed_json);
            parse_signed_number (videosLimit, &video.limit);
            parse_signed_number (videosOffset, &video.offset);
            parse_signed_number (videosTotalNumberOfItems, &video.totalNumberOfItems);

            openTIDAL_StructAddItem (o, video);
        }
    }

    o->status = 1;
}
