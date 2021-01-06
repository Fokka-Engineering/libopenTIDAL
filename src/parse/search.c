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
openTIDAL_ParseSearch (openTIDAL_ContentContainer *o, cJSON *input_json)
{
    int status = 0;
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
            struct openTIDAL_ArtistContainer artist;
            struct openTIDAL_JsonArtistContainer processed_json
                = openTIDAL_ParseJsonArtist (artistsItem);

            openTIDAL_ParseJsonArtistValues (&artist, &processed_json);
            openTIDAL_ParseJsonSignedNumber (artistsLimit, &artist.limit);
            openTIDAL_ParseJsonSignedNumber (artistsOffset, &artist.offset);
            openTIDAL_ParseJsonSignedNumber (artistsTotalNumberOfItems, &artist.totalNumberOfItems);

            status = openTIDAL_StructAddArtist (o, artist);
            if (status == -1) goto end;
        }
    }

    if (cJSON_IsArray (albumsItems)) {
        cJSON_ArrayForEach (albumsItem, albumsItems)
        {
            struct openTIDAL_AlbumContainer album;
            struct openTIDAL_JsonAlbumContainer processed_json
                = openTIDAL_ParseJsonAlbum (albumsItem);

            openTIDAL_ParseJsonSignedNumber (albumsLimit, &album.limit);
            openTIDAL_ParseJsonSignedNumber (albumsOffset, &album.offset);
            openTIDAL_ParseJsonSignedNumber (albumsTotalNumberOfItems, &album.totalNumberOfItems);
            status = openTIDAL_ParseJsonAlbumValues (&album, &processed_json);
            if (status == -1) goto end;

            status = openTIDAL_StructAddAlbum (o, album);
            if (status == -1) goto end;
        }
    }

    if (cJSON_IsArray (playlistsItems)) {
        cJSON_ArrayForEach (playlistsItem, playlistsItems)
        {
            struct openTIDAL_PlaylistContainer playlist;
            struct openTIDAL_JsonPlaylistContainer processed_json
                = openTIDAL_ParseJsonPlaylist (playlistsItem);

            openTIDAL_ParseJsonPlaylistValues (&playlist, &processed_json);
            openTIDAL_ParseJsonSignedNumber (playlistsLimit, &playlist.limit);
            openTIDAL_ParseJsonSignedNumber (playlistsOffset, &playlist.offset);
            openTIDAL_ParseJsonSignedNumber (playlistsTotalNumberOfItems,
                                             &playlist.totalNumberOfItems);

            status = openTIDAL_StructAddPlaylist (o, playlist);
            if (status == -1) goto end;
        }
    }

    if (cJSON_IsArray (tracksItems)) {
        cJSON_ArrayForEach (tracksItem, tracksItems)
        {
            struct openTIDAL_ItemsContainer track;
            struct openTIDAL_JsonItemsContainer processed_json
                = openTIDAL_ParseJsonItems (tracksItem);

            status = openTIDAL_ParseJsonItemsValues (&track, &processed_json);
            if (status == -1) goto end;
            openTIDAL_ParseJsonSignedNumber (tracksLimit, &track.limit);
            openTIDAL_ParseJsonSignedNumber (tracksOffset, &track.offset);
            openTIDAL_ParseJsonSignedNumber (tracksTotalNumberOfItems, &track.totalNumberOfItems);

            status = openTIDAL_StructAddItem (o, track);
            if (status == -1) goto end;
        }
    }

    if (cJSON_IsArray (videosItems)) {
        cJSON_ArrayForEach (videosItem, videosItems)
        {
            struct openTIDAL_ItemsContainer video;
            struct openTIDAL_JsonItemsContainer processed_json
                = openTIDAL_ParseJsonItems (videosItem);

            status = openTIDAL_ParseJsonItemsValues (&video, &processed_json);
            if (status == -1) goto end;
            openTIDAL_ParseJsonSignedNumber (videosLimit, &video.limit);
            openTIDAL_ParseJsonSignedNumber (videosOffset, &video.offset);
            openTIDAL_ParseJsonSignedNumber (videosTotalNumberOfItems, &video.totalNumberOfItems);

            status = openTIDAL_StructAddItem (o, video);
            if (status == -1) goto end;
        }
    }
end:
    o->status = 1;
    if (status == -1) o->status = -14;
}
