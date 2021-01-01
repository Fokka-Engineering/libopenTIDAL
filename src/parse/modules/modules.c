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

#include "modules.h"
#include "../../external/cJSON.h"
#include "../../helper/struct_helper.h"
#include "../../openTIDAL.h"
#include "../parse.h"
#include "arrayHelper.h"
#include <stdio.h>
#include <string.h>

/* Module types that are available and can be parsed by openTIDAL. MULTIPLE_TOP_PROMOTIONS is left
 * out because it's rubbish. */
static const char *moduleTypes[6]
    = { "MIXED_TYPES_LIST", "ALBUM_LIST", "TRACK_LIST", "VIDEO_LIST", "PLAYLIST_LIST", "MIX_LIST" };
/* Item types that are available in MIXED_TYPES_LIST or just exist. */
static const char *itemTypes[6] = { "ALBUM", "TRACK", "VIDEO", "ARTIST", "PLAYLIST", "MIX" };

struct moduleStruct {
    cJSON *row;
    cJSON *rows;
    cJSON *modules;
    cJSON *module;
    cJSON *title;
    cJSON *preTitle;
    cJSON *type;
    cJSON *pagedList;
    cJSON *items;
    cJSON *item;
    char *titleString;
    char *preTitleString;
    char *typeString;
};

/* Predeclare these prototypes. */
static int openTIDAL_ParseModuleMixedList (openTIDAL_ContentContainer *o, struct moduleStruct *m);
static int openTIDAL_ParseModuleAlbumList (openTIDAL_ContentContainer *o, struct moduleStruct *m);
static int openTIDAL_ParseModuleTrackList (openTIDAL_ContentContainer *o, struct moduleStruct *m);
static int openTIDAL_ParseModuleVideoList (openTIDAL_ContentContainer *o, struct moduleStruct *m);
static int openTIDAL_ParseModulePlaylistList (openTIDAL_ContentContainer *o,
                                              struct moduleStruct *m);
static int openTIDAL_ParseModuleMixList (openTIDAL_ContentContainer *o, struct moduleStruct *m);

int
openTIDAL_ParseModules (openTIDAL_ContentContainer *o, cJSON *input_json)
{
    struct moduleStruct m;
    int i;
    int status;
    m.rows = cJSON_GetObjectItem (input_json, "rows");
    m.row = NULL;

    status = openTIDAL_ParseModuleAllocHelper (o);
    if (status == -1) return -1;

    if (cJSON_IsArray (m.rows)) cJSON_ArrayForEach (m.row, m.rows)
        {
            m.item = NULL;
            m.modules = cJSON_GetObjectItem (m.row, "modules");
            m.module = cJSON_GetArrayItem (m.modules, 0);
            m.title = cJSON_GetObjectItemCaseSensitive (m.module, "title");
            m.preTitle = cJSON_GetObjectItemCaseSensitive (m.module, "preTitle");
            m.type = cJSON_GetObjectItemCaseSensitive (m.module, "type");
            m.pagedList = cJSON_GetObjectItem (m.module, "pagedList");
            m.items = cJSON_GetObjectItem (m.pagedList, "items");

            parse_string (m.title, &m.titleString);
            parse_string (m.preTitle, &m.preTitleString);
            parse_string (m.type, &m.typeString);

            /* Set Offset and Total to 0
             * Check for Reallocation error once since this is a common array.  */
            status = openTIDAL_ParseModuleAdd (o, 2, NULL, 0);
            if (status == -1) return -1;
            openTIDAL_ParseModuleAdd (o, 3, NULL, 0);

            /* Determine the type of the module. */
            if (m.typeString)
                for (i = 0; i < 6; i++) {
                    int type = -2;
                    if (strcmp (moduleTypes[i], m.typeString) == 0) {

                        if (status == -1) return -1;

                        switch (i) {
                        case 0: // MIXED_TYPES_LIST / ID = -1
                            type = -1;
                            break;
                        case 1: // ALBUM_LIST
                            type = 0;
                            openTIDAL_ParseModuleAdd (o, 2, NULL, o->total[type]);
                            openTIDAL_ParseModuleAlbumList (o, &m);
                            break;
                        case 2: // TRACK_LIST
                            type = 1;
                            openTIDAL_ParseModuleAdd (o, 2, NULL, o->total[type]);
                            openTIDAL_ParseModuleTrackList (o, &m);
                            break;
                        case 3: // VIDEO_LIST
                            type = 1;
                            openTIDAL_ParseModuleAdd (o, 2, NULL, o->total[type]);
                            openTIDAL_ParseModuleVideoList (o, &m);
                            break;
                        case 4: // PLAYLIST_LIST
                            type = 3;
                            openTIDAL_ParseModuleAdd (o, 2, NULL, o->total[type]);
                            openTIDAL_ParseModulePlaylistList (o, &m);
                            break;
                        case 5: // MIX_LIST
                            type = 4;
                            openTIDAL_ParseModuleAdd (o, 2, NULL, o->total[type]);
                            openTIDAL_ParseModuleMixList (o, &m);
                            break;
                        }

                        openTIDAL_ParseModuleAdd (o, 1, NULL, type);
                        openTIDAL_ParseModuleAdd (o, 0, m.typeString, 0);
                        openTIDAL_ParseModuleAdd (o, 4, m.titleString, 0);
                        openTIDAL_ParseModuleAdd (o, 5, m.preTitleString, 0);
                        o->modules->arraySize++;
                        break;
                    }
                };
        };
    return 0;
}

static int
openTIDAL_ParseModuleMixedList (openTIDAL_ContentContainer *o, struct moduleStruct *m)
{
    if (cJSON_IsArray (m->items)) cJSON_ArrayForEach (m->item, m->items)
        {
            cJSON *type;
            cJSON *innerItem;
            char *typeString;
            int i;
            int status = 0;
            /* Can't define variables in switch statement */
            openTIDAL_AlbumContainer album;
            json_album_model processed_album;
            openTIDAL_ItemsContainer item;
            json_items_model processed_item;
            openTIDAL_ArtistContainer artist;
            json_artist_model processed_artist;
            openTIDAL_PlaylistContainer playlist;
            json_playlist_model processed_playlist;
            openTIDAL_MixContainer mix;
            json_mix_model processed_mix;

            type = cJSON_GetObjectItemCaseSensitive (m->item, "type");
            innerItem = cJSON_GetObjectItem (m->item, "item");
            parse_string (type, &typeString);

            /* Determine the type of the item */
            if (typeString)
                for (i = 0; i < sizeof (itemTypes); i++) {
                    if (strcmp (typeString, itemTypes[i]) == 0) {
                        switch (i) {
                        case 0: // ALBUMS
                            processed_album = json_parse_album (innerItem);
                            parse_album_values (&album, &processed_album);
                            status = openTIDAL_StructAddAlbum (o, album);
                            break;
                        case 1: // TRACKS
                            processed_item = json_parse_items (innerItem);
                            parse_items_values (&item, &processed_item);
                            status = openTIDAL_StructAddItem (o, item);
                            break;
                        case 2: // VIDEOS
                            processed_item = json_parse_items (innerItem);
                            parse_items_values (&item, &processed_item);
                            status = openTIDAL_StructAddItem (o, item);
                            break;
                        case 3: // ARTISTS
                            processed_artist = json_parse_artist (innerItem);
                            parse_artist_values (&artist, &processed_artist);
                            status = openTIDAL_StructAddArtist (o, artist);
                            break;
                        case 4: // PLAYLISTS
                            processed_playlist = json_parse_playlist (innerItem);
                            parse_playlist_values (&playlist, &processed_playlist);
                            status = openTIDAL_StructAddPlaylist (o, playlist);
                            break;
                        case 5: // MIX
                            processed_mix = json_parse_mix (innerItem);
                            parse_mix_values (&mix, &processed_mix);
                            status = openTIDAL_StructAddMix (o, mix);
                            break;
                        }
                        if (status == -1) return -1;
                        break;
                    }
                };
        };
    return 0;
}

static int
openTIDAL_ParseModuleAlbumList (openTIDAL_ContentContainer *o, struct moduleStruct *m)
{
    if (cJSON_IsArray (m->items)) cJSON_ArrayForEach (m->item, m->items)
        {
            int status;
            openTIDAL_AlbumContainer album;

            json_album_model processed_json = json_parse_album (m->item);
            parse_album_values (&album, &processed_json);
            status = openTIDAL_StructAddAlbum (o, album);
            if (status == -1) return -1;

            o->modules->total[o->modules->arraySize]++;
        };
    return 0;
}

static int
openTIDAL_ParseModuleTrackList (openTIDAL_ContentContainer *o, struct moduleStruct *m)
{
    if (cJSON_IsArray (m->items)) cJSON_ArrayForEach (m->item, m->items)
        {
            int status;
            openTIDAL_ItemsContainer item;

            json_items_model processed_json = json_parse_items (m->item);
            parse_items_values (&item, &processed_json);
            status = openTIDAL_StructAddItem (o, item);
            if (status == -1) return -1;

            o->modules->total[o->modules->arraySize]++;
        };
    return 0;
}

static int
openTIDAL_ParseModuleVideoList (openTIDAL_ContentContainer *o, struct moduleStruct *m)
{
    if (cJSON_IsArray (m->items)) cJSON_ArrayForEach (m->item, m->items)
        {
            int status;
            openTIDAL_ItemsContainer item;

            json_items_model processed_json = json_parse_items (m->item);
            parse_items_values (&item, &processed_json);
            status = openTIDAL_StructAddItem (o, item);
            if (status == -1) return -1;

            o->modules->total[o->modules->arraySize]++;
        };
    return 0;
}

static int
openTIDAL_ParseModulePlaylistList (openTIDAL_ContentContainer *o, struct moduleStruct *m)
{
    if (cJSON_IsArray (m->items)) cJSON_ArrayForEach (m->item, m->items)
        {
            int status;
            openTIDAL_PlaylistContainer playlist;

            json_playlist_model processed_json = json_parse_playlist (m->item);
            parse_playlist_values (&playlist, &processed_json);
            status = openTIDAL_StructAddPlaylist (o, playlist);
            if (status == -1) return -1;

            o->modules->total[o->modules->arraySize]++;
        };
    return 0;
}

static int
openTIDAL_ParseModuleMixList (openTIDAL_ContentContainer *o, struct moduleStruct *m)
{
    if (cJSON_IsArray (m->items)) cJSON_ArrayForEach (m->item, m->items)
        {
            int status;
            openTIDAL_MixContainer mix;

            json_mix_model processed_json = json_parse_mix (m->item);
            parse_mix_values (&mix, &processed_json);
            status = openTIDAL_StructAddMix (o, mix);
            if (status == -1) return -1;

            o->modules->total[o->modules->arraySize]++;
        };
    return 0;
}
