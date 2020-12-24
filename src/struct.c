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

#include <stdio.h>
#include <stdlib.h>

#include "include/cJSON.h"
#include "include/openTIDAL.h"

/* Initialise pointer in openTIDAL structure */
void openTIDAL_StructInit(openTIDAL *o)
{
    o->albums = NULL;
    o->items = NULL;
    o->artists = NULL;
    o->playlists = NULL;
    o->mixes = NULL;
    o->contributors = NULL;
    o->credits = NULL;
    o->links = NULL;

    o->home.recentlyPlayed_ArrayTypes = NULL;
    o->home.recentlyPlayed_ArrayPosition = NULL;
    o->home.radioStationsForYou_Start = NULL;
    o->home.radioStationsForYou_Total = NULL;
    o->home.becauseYouListenedTo_Start = NULL;
    o->home.becauseYouListenedTo_Title = NULL;
    o->home.becauseYouListenedTo_Total = NULL;

    o->status = 0;
    o->json = NULL;
    o->jsonManifest = NULL;

    openTIDAL_ParseVerbose("Struct", "Initialise openTIDAL struct", 2);
}

/* Allocate the dynamic array in heap with the initial capacity. 
 * Use the openTIDAL array identifiers to allocate the corrent array.
 * To keep track of the allocated size and the items added to the array,
 * there are two static arrays (capacity and total).
 * The indentifiers are used to access the correct values. */
void openTIDAL_StructAlloc(openTIDAL *o, int index)
{
    switch(index)
    {
        case 0:
            o->capacity[0] = STRUCT_INIT_CAPACITY;
            o->total[0] = 0;
            o->albums = malloc(sizeof(openTIDAL_AlbumModel) * o->capacity[0]);
            break;

        case 1:
            o->capacity[1] = STRUCT_INIT_CAPACITY;
            o->total[1] = 0;
            o->items = malloc(sizeof(openTIDAL_ItemsModel) * o->capacity[1]);
            break; 

        case 2:
            o->capacity[2] = STRUCT_INIT_CAPACITY;
            o->total[2] = 0;
            o->artists = malloc(sizeof(openTIDAL_ArtistModel) * o->capacity[2]);
            break;

        case 3:
            o->capacity[3] = STRUCT_INIT_CAPACITY;
            o->total[3] = 0;
            o->playlists = malloc(sizeof(openTIDAL_PlaylistModel) * o->capacity[3]);
            break;

        case 4:
            o->capacity[4] = STRUCT_INIT_CAPACITY;
            o->total[4] = 0;
            o->mixes = malloc(sizeof(openTIDAL_MixModel) * o->capacity[4]);
         break;

        case 5:
            o->capacity[5] = STRUCT_INIT_CAPACITY;
            o->total[5] = 0;
            o->contributors = malloc(sizeof(openTIDAL_ContributorModel) * o->capacity[5]);
         break;

        case 6:
            o->capacity[6] = STRUCT_INIT_CAPACITY;
            o->total[6] = 0;
            o->credits = malloc(sizeof(openTIDAL_CreditsModel) * o->capacity[6]);
        break;

        case 7:
            o->capacity[7] = STRUCT_INIT_CAPACITY;
            o->total[7] = 0;
            o->links = malloc(sizeof(openTIDAL_LinkModel) * o->capacity[7]);
         break;
    }

    char buffer[50];
    snprintf(buffer, sizeof(buffer), "Allocate array (identifier: %d) in heap", index);
    openTIDAL_ParseVerbose("Struct", buffer, 2);
}

/* Deallocates all dynamic arrays inside the openTIDAL structure.
 * The allocated cJSON-Tree and all it's values will be deallocated
 * too. */
void openTIDAL_StructDelete(openTIDAL *o)
{
    cJSON_Delete((cJSON *) o->json);
    cJSON_Delete((cJSON *) o->jsonManifest);

    if ( o->albums )
    {
        int i;
        for (i = 0; i < o->total[0]; ++i)
        {
            free(o->albums[i].artistId);
            free(o->albums[i].artistName);
        }
    }

    if ( o->items )
    {
        int i;
        for (i = 0; i < o->total[1]; ++i)
        {
            free(o->items[i].artistId);
            free(o->items[i].artistName);
        }

    }

    free(o->home.recentlyPlayed_ArrayTypes);
    free(o->home.recentlyPlayed_ArrayPosition);
    free(o->home.radioStationsForYou_Start);
    free(o->home.radioStationsForYou_Total);
    free(o->home.becauseYouListenedTo_Start);
    free(o->home.becauseYouListenedTo_Title);
    free(o->home.becauseYouListenedTo_Total);

    free(o->albums);
    free(o->items);
    free(o->artists);
    free(o->playlists);
    free(o->mixes);
    free(o->contributors);
    free(o->credits);
    free(o->links);

    openTIDAL_ParseVerbose("Struct", "Deallocate all arrays in structure", 2);
}

/* Resizes a specific dynamic array. Use the openTIDAL array identifiers
 * to resize the correct array. The allocated array will grow by doubling
 * it's capacity (See openTIDAL_StructAdd). This safes system resources */
static void openTIDAL_StructResize(openTIDAL *o, int capacity, int index)
{
    openTIDAL_AlbumModel *albums;
    openTIDAL_ItemsModel *items;
    openTIDAL_ArtistModel *artists;
    openTIDAL_PlaylistModel *playlists;
    openTIDAL_MixModel *mixes;
    openTIDAL_ContributorModel *contributors;
    openTIDAL_CreditsModel *credits;
    openTIDAL_LinkModel *links;

    switch(index)
    {
        case 0:
            albums = realloc(o->albums, sizeof(openTIDAL_AlbumModel) * capacity);
            if( albums )
            {
                o->albums = albums;
                o->capacity[0] = capacity;
            }
            break;

        case 1:
            items = realloc(o->items, sizeof(openTIDAL_ItemsModel) * capacity);
            if( items )
            {
                o->items = items;
                o->capacity[1] = capacity;
            }
         break; 

        case 2:
            artists = realloc(o->artists, sizeof(openTIDAL_ArtistModel) * capacity);
            if( artists )
            {
                o->artists = artists;
                o->capacity[2] = capacity;
            }
            break;

        case 3:
            playlists = realloc(o->playlists, sizeof(openTIDAL_PlaylistModel) * capacity);
            if( playlists )
            {
                o->playlists = playlists;
                o->capacity[3] = capacity;
            }
            break;

        case 4:
            mixes = realloc(o->mixes, sizeof(openTIDAL_MixModel) * capacity);
            if( mixes )
            {
                o->mixes = mixes;
                o->capacity[4] = capacity;
            }
            break;

        case 5:
            contributors = realloc(o->contributors, sizeof(openTIDAL_ContributorModel) * capacity);
            if( contributors )
            {
                o->contributors = contributors;
                o->capacity[5] = capacity;
            }
            break;

        case 6:
            credits = realloc(o->credits, sizeof(openTIDAL_CreditsModel) * capacity);
            if( credits )
            {
                o->credits = credits;
                o->capacity[6] = capacity;
            }
            break;

        case 7:
            links = realloc(o->links, sizeof(openTIDAL_LinkModel) * capacity);
            if( links )
            {
                o->links = links;
                o->capacity[7] = capacity;
            }
            break;
    }

    char buffer[50];
    snprintf(buffer, sizeof(buffer), "Realloc array (identifier %d) from %d to %d", index, capacity / 2, capacity);
    openTIDAL_ParseVerbose("Struct", buffer, 2);
}

void openTIDAL_StructAddAlbum(openTIDAL *o, openTIDAL_AlbumModel album)
{
    int index = 0;
    if ( o->capacity[index] == o->total[index] )
        openTIDAL_StructResize(o, o->capacity[index] * 2, index);
    o->albums[o->total[index]++] = album;

    openTIDAL_ParseVerbose("Struct", "Add item to array (identifier 0)", 2);
}

void openTIDAL_StructAddItem(openTIDAL *o, openTIDAL_ItemsModel item)
{
    int index = 1;
    if ( o->capacity[index] == o->total[index] )
        openTIDAL_StructResize(o, o->capacity[index] * 2, index);
    o->items[o->total[index]++] = item;

    openTIDAL_ParseVerbose("Struct", "Add item to array (identifier 1)", 2);
}

void openTIDAL_StructAddArtist(openTIDAL *o, openTIDAL_ArtistModel artist)
{
    int index = 2;
    if ( o->capacity[index] == o->total[index] )
        openTIDAL_StructResize(o, o->capacity[index] * 2, index);
    o->artists[o->total[index]++] = artist;

    openTIDAL_ParseVerbose("Struct", "Add item to array (identifier 2)", 2);
}

void openTIDAL_StructAddPlaylist(openTIDAL *o, openTIDAL_PlaylistModel playlist)
{
    int index = 3;
    if ( o->capacity[index] == o->total[index] )
        openTIDAL_StructResize(o, o->capacity[index] * 2, index);
    o->playlists[o->total[index]++] = playlist;

    openTIDAL_ParseVerbose("Struct", "Add item to array (identifier 3)", 2);
}

void openTIDAL_StructAddMix(openTIDAL *o, openTIDAL_MixModel mix)
{
    int index = 4;
    if ( o->capacity[index] == o->total[index] )
        openTIDAL_StructResize(o, o->capacity[index] * 2, index);
    o->mixes[o->total[index]++] = mix;

    openTIDAL_ParseVerbose("Struct", "Add item to array (identifier 4)", 2);
}

void openTIDAL_StructAddContributor(openTIDAL *o, openTIDAL_ContributorModel contributor)
{
    int index = 5;
    if ( o->capacity[index] == o->total[index] )
        openTIDAL_StructResize(o, o->capacity[index] * 2, index);
    o->contributors[o->total[index]++] = contributor;

    openTIDAL_ParseVerbose("Struct", "Add item to array (identifier 5)", 2);
}

void openTIDAL_StructAddCredit(openTIDAL *o, openTIDAL_CreditsModel credit)
{
    int index = 6;
    if ( o->capacity[index] == o->total[index] )
        openTIDAL_StructResize(o, o->capacity[index] * 2, index);
    o->credits[o->total[index]++] = credit;

    openTIDAL_ParseVerbose("Struct", "Add item to array (identifier 6)", 2);
}

void openTIDAL_StructAddLink(openTIDAL *o, openTIDAL_LinkModel link)
{
    int index = 7;
    if ( o->capacity[index] == o->total[index] )
        openTIDAL_StructResize(o, o->capacity[index] * 2, index);
    o->links[o->total[index]++] = link;

    openTIDAL_ParseVerbose("Struct", "Add item to array (identifier 7)", 2);
}
