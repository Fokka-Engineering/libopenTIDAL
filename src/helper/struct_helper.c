/*
    Copyright (c) 2020-2021 Hugo Melder and openTIDAL_ContentContainer contributors

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

#include "../helper/struct_helper.h"
#include "../external/cJSON.h"
#include "../helper/helper.h"
#include "../openTIDAL.h"
#include <stdio.h>
#include <stdlib.h>

int
openTIDAL_StructMainAlloc (openTIDAL_ContentContainer **o)
{
    *o = malloc (sizeof (openTIDAL_ContentContainer));
    if (!o) {
        openTIDAL_VerboseHelper (
            "Struct", "Memory Allocation of ContentContainer failed. Malloc returned NULL", 1);
        return -1;
    }
    return 0;
}

/* Initialise pointers in openTIDAL_ContentContainer structure call after allocating main */
int
openTIDAL_StructInit (openTIDAL_ContentContainer *o)
{
    o->albums = NULL;
    o->items = NULL;
    o->artists = NULL;
    o->playlists = NULL;
    o->mixes = NULL;
    o->contributors = NULL;
    o->credits = NULL;
    o->links = NULL;

    o->code = NULL;
    o->token = NULL;
    o->user = NULL;
    o->subscription = NULL;
    o->stream = NULL;
    o->modules = NULL;

    o->status = -1;
    o->json = NULL;
    o->jsonManifest = NULL;

    openTIDAL_VerboseHelper ("Struct", "Initialised openTIDAL_ContentContainer struct", 2);

    return 0;
}

/* Allocate a structure in heap. Point the pointer in openTIDAL_ContentContainer to
 * the memory location
 * Index:
 * -1 = struct openTIDAL_LoginCodeContainer
 * -2 = struct openTIDAL_LoginTokenContainer
 * -3 = struct openTIDAL_UserContainer
 * -4 = struct openTIDAL_UserSubscriptionContainer
 * -5 = struct openTIDAL_StreamContainer
 * -6 = openTIDAL_ModuleContainer
 * These identifiers are only used by this helper function. */
int
openTIDAL_StructOneTimeAlloc (openTIDAL_ContentContainer *o, int index)
{
    switch (index) {
    case -1:
        o->code = malloc (sizeof (struct openTIDAL_LoginCodeContainer));
        if (!o->code) {
            openTIDAL_VerboseHelper ("Struct",
                                     "Allocation of CodeContainer failed. Malloc returned NULL", 1);
            return -1;
        }
        break;
    case -2:
        o->token = malloc (sizeof (struct openTIDAL_LoginTokenContainer));
        if (!o->token) {
            openTIDAL_VerboseHelper (
                "Struct", "Allocation of TokenContainer failed. Malloc returned NULL", 1);
            return -1;
        }
        break;
    case -3:
        o->user = malloc (sizeof (struct openTIDAL_UserContainer));
        if (!o->user) {
            openTIDAL_VerboseHelper ("Struct",
                                     "Allocation of UserContainer failed. Malloc returned NULL", 1);
            return -1;
        }
        break;
    case -4:
        o->subscription = malloc (sizeof (struct openTIDAL_UserSubscriptionContainer));
        if (!o->subscription) {
            openTIDAL_VerboseHelper (
                "Struct", "Allocation of UserSubscriptionContainer failed. Malloc returned NULL",
                1);
            return -1;
        }
        break;
    case -5:
        o->stream = malloc (sizeof (struct openTIDAL_StreamContainer));
        if (!o->stream) {
            openTIDAL_VerboseHelper (
                "Struct", "Allocation of StreamContainer failed. Malloc returned NULL", 1);
            return -1;
        }
        break;
    case -6:
        o->modules = malloc (sizeof (struct openTIDAL_ModuleContainer));
        if (!o->modules) {
            openTIDAL_VerboseHelper (
                "Struct", "Allocation of ModuleContainer failed. Malloc returned NULL", 1);
            return -1;
        }
        /* Initialise pointers in structure */
        o->modules->moduleType = NULL;
        o->modules->moduleTitle = NULL;
        o->modules->modulePreTitle = NULL;
        o->modules->arrayType = NULL;
        o->modules->offset = NULL;
        o->modules->total = NULL;
        o->modules->mixedListTypes = NULL;
        o->modules->mixedListOffset = NULL;
        o->modules->mixedListTotal = NULL;
        break;
    }
    return 0;
}

/* Allocate the dynamic array in heap with the initial capacity.
 * Use the openTIDAL_ContentContainer array identifiers to allocate the corrent array.
 * To keep track of the allocated size and the items added to the array,
 * there are two static arrays (capacity and total).
 * The indentifiers are used to access the correct values. */
int
openTIDAL_StructAlloc (openTIDAL_ContentContainer *o, int index)
{
    switch (index) {
    case 0:
        o->capacity[0] = STRUCT_INIT_CAPACITY;
        o->total[0] = 0;
        o->albums = malloc (sizeof (struct openTIDAL_AlbumContainer) * o->capacity[0]);
        if (!o->albums) {
            openTIDAL_VerboseHelper (
                "Struct", "Allocation of AlbumContainer array failed. Malloc returned NULL", 1);
            return -1;
        }
        break;

    case 1:
        o->capacity[1] = STRUCT_INIT_CAPACITY;
        o->total[1] = 0;
        o->items = malloc (sizeof (struct openTIDAL_ItemsContainer) * o->capacity[1]);
        if (!o->items) {
            openTIDAL_VerboseHelper (
                "Struct", "Allocation of ItemContainer array failed. Malloc returned NULL", 1);
            return -1;
        }
        break;

    case 2:
        o->capacity[2] = STRUCT_INIT_CAPACITY;
        o->total[2] = 0;
        o->artists = malloc (sizeof (struct openTIDAL_ArtistContainer) * o->capacity[2]);
        if (!o->artists) {
            openTIDAL_VerboseHelper (
                "Struct", "Allocation of ArtistContainer array failed. Malloc returned NULL", 1);
            return -1;
        }
        break;

    case 3:
        o->capacity[3] = STRUCT_INIT_CAPACITY;
        o->total[3] = 0;
        o->playlists = malloc (sizeof (struct openTIDAL_PlaylistContainer) * o->capacity[3]);
        if (!o->playlists) {
            openTIDAL_VerboseHelper (
                "Struct", "Allocation of PlaylistContainer array failed. Malloc returned NULL", 1);
            return -1;
        }
        break;

    case 4:
        o->capacity[4] = STRUCT_INIT_CAPACITY;
        o->total[4] = 0;
        o->mixes = malloc (sizeof (struct openTIDAL_MixContainer) * o->capacity[4]);
        if (!o->mixes) {
            openTIDAL_VerboseHelper (
                "Struct", "Allocation of MixesContainer array failed. Malloc returned NULL", 1);
            return -1;
        }
        break;

    case 5:
        o->capacity[5] = STRUCT_INIT_CAPACITY;
        o->total[5] = 0;
        o->contributors = malloc (sizeof (struct openTIDAL_ContributorContainer) * o->capacity[5]);
        if (!o->contributors) {
            openTIDAL_VerboseHelper (
                "Struct", "Allocation of ContributorContainer array failed. Malloc returned NULL",
                1);
            return -1;
        }
        break;

    case 6:
        o->capacity[6] = STRUCT_INIT_CAPACITY;
        o->total[6] = 0;
        o->credits = malloc (sizeof (openTIDAL_CreditsContainer) * o->capacity[6]);
        if (!o->credits) {
            openTIDAL_VerboseHelper (
                "Struct", "Allocation of CreditsContainer array failed. Malloc returned NULL", 1);
            return -1;
        }
        break;

    case 7:
        o->capacity[7] = STRUCT_INIT_CAPACITY;
        o->total[7] = 0;
        o->links = malloc (sizeof (struct openTIDAL_LinkContainer) * o->capacity[7]);
        if (!o->links) {
            openTIDAL_VerboseHelper (
                "Struct", "Allocation of LinksContainer array failed. Malloc returned NULL", 1);
            return -1;
        }
        break;
    }

    openTIDAL_VerboseHelper ("Struct", "Allocate array (identifier: %d) in heap", 2, index);

    return 0;
}

/* Deallocates all dynamic arrays inside the openTIDAL_ContentContainer structure.
 * The allocated cJSON-Tree and all it's values will be deallocated
 * too. */
void
openTIDAL_StructDelete (openTIDAL_ContentContainer *o)
{
    cJSON_Delete ((cJSON *)o->json);
    cJSON_Delete ((cJSON *)o->jsonManifest);

    if (o->albums) {
        int i;
        for (i = 0; i < o->total[0]; ++i) {
            free (o->albums[i].artistId);
            free (o->albums[i].artistName);
        }
    }
    if (o->items) {
        int i;
        for (i = 0; i < o->total[1]; ++i) {
            free (o->items[i].artistId);
            free (o->items[i].artistName);
        }
    }
    if (o->modules) {
        free (o->modules->moduleType);
        free (o->modules->moduleTitle);
        free (o->modules->modulePreTitle);
        free (o->modules->arrayType);
        free (o->modules->offset);
        free (o->modules->total);
        free (o->modules->mixedListTypes);
        free (o->modules->mixedListOffset);
        free (o->modules->mixedListTotal);
    }

    free (o->albums);
    free (o->items);
    free (o->artists);
    free (o->playlists);
    free (o->mixes);
    free (o->contributors);
    free (o->credits);
    free (o->links);

    free (o->code);
    free (o->token);
    free (o->user);
    free (o->subscription);
    free (o->stream);
    free (o->modules);

    free (o);
    openTIDAL_VerboseHelper ("Struct", "Deallocate all arrays in structure", 2);
}

/* Resizes a specific dynamic array. Use the openTIDAL_ContentContainer array identifiers
 * to resize the correct array. The allocated array will grow by doubling
 * it's capacity (See openTIDAL_StructAdd). This safes system resources */
static int
openTIDAL_StructResize (openTIDAL_ContentContainer *o, int capacity, int index)
{
    struct openTIDAL_AlbumContainer *albums;
    struct openTIDAL_ItemsContainer *items;
    struct openTIDAL_ArtistContainer *artists;
    struct openTIDAL_PlaylistContainer *playlists;
    struct openTIDAL_MixContainer *mixes;
    struct openTIDAL_ContributorContainer *contributors;
    openTIDAL_CreditsContainer *credits;
    struct openTIDAL_LinkContainer *links;

    switch (index) {
    case 0:
        albums = realloc (o->albums, sizeof (struct openTIDAL_AlbumContainer) * capacity);
        if (!albums) {
            openTIDAL_VerboseHelper (
                "Struct", "Reallocating AlbumContainer-Array failed. Realloc returned NULL.", 1);
            return -1;
        }
        o->albums = albums;
        o->capacity[0] = capacity;
        break;

    case 1:
        items = realloc (o->items, sizeof (struct openTIDAL_ItemsContainer) * capacity);
        if (!items) {
            openTIDAL_VerboseHelper (
                "Struct", "Reallocating ItemContainer-Array failed. Realloc returned NULL.", 1);
            return -1;
        }
        o->items = items;
        o->capacity[1] = capacity;
        break;

    case 2:
        artists = realloc (o->artists, sizeof (struct openTIDAL_ArtistContainer) * capacity);
        if (!artists) {
            openTIDAL_VerboseHelper (
                "Struct", "Reallocating ArtistContainer-Array failed. Realloc returned NULL.", 1);
            return -1;
        }
        o->artists = artists;
        o->capacity[2] = capacity;
        break;

    case 3:
        playlists = realloc (o->playlists, sizeof (struct openTIDAL_PlaylistContainer) * capacity);
        if (!playlists) {
            openTIDAL_VerboseHelper (
                "Struct", "Reallocating PlaylistContainer-Array failed. Realloc returned NULL.", 1);
            return -1;
        }
        o->playlists = playlists;
        o->capacity[3] = capacity;
        break;

    case 4:
        mixes = realloc (o->mixes, sizeof (struct openTIDAL_MixContainer) * capacity);
        if (!mixes) {
            openTIDAL_VerboseHelper (
                "Struct", "Reallocating MixesContainer-Array failed. Realloc returned NULL.", 1);
            return -1;
        }
        o->mixes = mixes;
        o->capacity[4] = capacity;
        break;

    case 5:
        contributors
            = realloc (o->contributors, sizeof (struct openTIDAL_ContributorContainer) * capacity);
        if (!contributors) {
            openTIDAL_VerboseHelper (
                "Struct", "Reallocating ContributorsContainer-Array failed. Realloc returned NULL.",
                1);
            return -1;
        }
        o->contributors = contributors;
        o->capacity[5] = capacity;
        break;

    case 6:
        credits = realloc (o->credits, sizeof (openTIDAL_CreditsContainer) * capacity);
        if (!credits) {
            openTIDAL_VerboseHelper (
                "Struct", "Reallocating CreditContainer-Array failed. Realloc returned NULL.", 1);
            return -1;
        }
        o->credits = credits;
        o->capacity[6] = capacity;
        break;

    case 7:
        links = realloc (o->links, sizeof (struct openTIDAL_LinkContainer) * capacity);
        if (!links) {
            openTIDAL_VerboseHelper (
                "Struct", "Reallocating LinkContainer-Array failed. Realloc returned NULL.", 1);
            return -1;
        }
        o->links = links;
        o->capacity[7] = capacity;
        break;
    }

    openTIDAL_VerboseHelper ("Struct", "Realloc array (identifier %d) from %d to %d", 2, index,
                             capacity / 2, capacity);
    return 0;
}

/* Add items to the dynamic arrays in the ContentContainer */
int
openTIDAL_StructAddAlbum (openTIDAL_ContentContainer *o, struct openTIDAL_AlbumContainer album)
{
    int index = 0;
    if (o->capacity[index] == o->total[index]) {
        int status;
        status = openTIDAL_StructResize (o, o->capacity[index] * 2, index);
        if (status == -1) return -1;
    }
    o->albums[o->total[index]++] = album;

    openTIDAL_VerboseHelper ("Struct",
                             "Add item to array (identifier 0) // Total: %d // Capacity: %d", 2,
                             o->total[index], o->capacity[index]);
    return 0;
}

int
openTIDAL_StructAddItem (openTIDAL_ContentContainer *o, struct openTIDAL_ItemsContainer item)
{
    int index = 1;
    if (o->capacity[index] == o->total[index]) {
        int status;
        status = openTIDAL_StructResize (o, o->capacity[index] * 2, index);
        if (status == -1) return -1;
    }
    o->items[o->total[index]++] = item;
    openTIDAL_VerboseHelper ("Struct",
                             "Add item to array (identifier 1) // Total: %d // Capacity: %d", 2,
                             o->total[index], o->capacity[index]);
    return 0;
}

int
openTIDAL_StructAddArtist (openTIDAL_ContentContainer *o, struct openTIDAL_ArtistContainer artist)
{
    int index = 2;
    if (o->capacity[index] == o->total[index]) {
        int status;
        status = openTIDAL_StructResize (o, o->capacity[index] * 2, index);
        if (status == -1) return -1;
    }
    o->artists[o->total[index]++] = artist;

    openTIDAL_VerboseHelper ("Struct",
                             "Add item to array (identifier 2) // Total: %d // Capacity: %d", 2,
                             o->total[index], o->capacity[index]);
    return 0;
}

int
openTIDAL_StructAddPlaylist (openTIDAL_ContentContainer *o,
                             struct openTIDAL_PlaylistContainer playlist)
{
    int index = 3;
    if (o->capacity[index] == o->total[index]) {
        int status;
        status = openTIDAL_StructResize (o, o->capacity[index] * 2, index);
        if (status == -1) return -1;
    }
    o->playlists[o->total[index]++] = playlist;

    openTIDAL_VerboseHelper ("Struct",
                             "Add item to array (identifier 3) // Total: %d // Capacity: %d", 2,
                             o->total[index], o->capacity[index]);
    return 0;
}

int
openTIDAL_StructAddMix (openTIDAL_ContentContainer *o, struct openTIDAL_MixContainer mix)
{
    int index = 4;
    if (o->capacity[index] == o->total[index]) {
        int status;
        status = openTIDAL_StructResize (o, o->capacity[index] * 2, index);
        if (status == -1) return -1;
    }
    o->mixes[o->total[index]++] = mix;

    openTIDAL_VerboseHelper ("Struct",
                             "Add item to array (identifier 4) // Total: %d // Capacity: %d", 2,
                             o->total[index], o->capacity[index]);
    return 0;
}

int
openTIDAL_StructAddContributor (openTIDAL_ContentContainer *o,
                                struct openTIDAL_ContributorContainer contributor)
{
    int index = 5;
    if (o->capacity[index] == o->total[index]) {
        int status;
        status = openTIDAL_StructResize (o, o->capacity[index] * 2, index);
        if (status == -1) return -1;
    }
    o->contributors[o->total[index]++] = contributor;

    openTIDAL_VerboseHelper ("Struct", "Add item to array (identifier 5)", 2);
    return 0;
}

int
openTIDAL_StructAddCredit (openTIDAL_ContentContainer *o, openTIDAL_CreditsContainer credit)
{
    int index = 6;
    if (o->capacity[index] == o->total[index]) {
        int status;
        status = openTIDAL_StructResize (o, o->capacity[index] * 2, index);
        if (status == -1) return -1;
    }
    o->credits[o->total[index]++] = credit;

    openTIDAL_VerboseHelper ("Struct", "Add item to array (identifier 6)", 2);
    return 0;
}

int
openTIDAL_StructAddLink (openTIDAL_ContentContainer *o, struct openTIDAL_LinkContainer link)
{
    int index = 7;
    if (o->capacity[index] == o->total[index]) {
        int status;
        status = openTIDAL_StructResize (o, o->capacity[index] * 2, index);
        if (status == -1) return -1;
    }
    o->links[o->total[index]++] = link;

    openTIDAL_VerboseHelper ("Struct", "Add item to array (identifier 7)", 2);
    return 0;
}
