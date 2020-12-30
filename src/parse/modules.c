/*
    Copyright (c) 2020 Hugo Melder and openTIDAL contributors

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

#include "../include/cJSON.h"
#include "../include/openTIDAL.h"
#include "../include/parse.h"
#include <stdio.h>
#include <string.h>

/* Module types that are available and can be parsed by openTIDAL. MULTIPLE_TOP_PROMOTIONS is left
 * out because it's rubbish. */
static const char *moduleTypes[6]
    = { "MIXED_TYPES_LIST", "ALBUM_LIST", "TRACK_LIST", "VIDEO_LIST", "PLAYLIST_LIST", "MIX_LIST" };

struct module {
    cJSON *row;
    cJSON *rows;
    cJSON *modules;
    cJSON *module;
    cJSON *title;
    cJSON *preTitle;
    cJSON *type;
    cJSON *pagedList;
    cJSON *items;
    char *titleString;
    char *preTitleString;
    char *typeString;
};

void
openTIDAL_ParseModules (openTIDAL_ContentContainer *o, cJSON *input_json)
{
    struct module m;
    int i;
    m.rows = cJSON_GetObjectItem (input_json, "rows");
    m.row = NULL;

    // if (cJSON_IsArray(m.rows)
    cJSON_ArrayForEach (m.row, m.rows)
    {
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

        if (m.typeString)
            for (i = 0; i < sizeof (moduleTypes); i++) {
                if (strcmp (moduleTypes[i], m.typeString) == 0) {
                    switch (i) {
                    case 0:
                        break;
                    case 1:
                        break;
                    case 2:
                        break;
                    case 3:
                        break;
                    case 4:
                        break;
                    case 5:
                        break;
                    }
                    break;
                }
            };
    };
}

void
openTIDAL_ParseModuleMixedList (struct module *m)
{
}

void
openTIDAL_ParseModuleAlbumList (struct module *m)
{
}

void
openTIDAL_ParseModuleTrackList (struct module *m)
{
}

void
openTIDAL_ParseModuleVideoList (struct module *m)
{
}

void
openTIDAL_ParseModulePlaylistList (struct module *m)
{
}

void
openTIDAL_ParseModuleMixList (struct module *m)
{
}
