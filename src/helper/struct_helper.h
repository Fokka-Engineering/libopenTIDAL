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

#ifndef STRUCT__h
#define STRUCT__h

#include "../openTIDAL.h"

/* Memory management of main struct (Heap Allocation & Deallocation)
 * pointer to struct parameter to avoid a copy in stack  */
/* initialize struct and it's content */
int openTIDAL_StructMainAlloc (openTIDAL_ContentContainer **o);
int openTIDAL_StructInit (openTIDAL_ContentContainer *o);

/* Allocate sepcified structure in heap */
int openTIDAL_StructOneTimeAlloc (openTIDAL_ContentContainer *o, int index);
/* Allocate specified array in heap */
int openTIDAL_StructAlloc (openTIDAL_ContentContainer *o, int index);

/* Add model to array (Allocate the correct array before adding!) */
int openTIDAL_StructAddAlbum (openTIDAL_ContentContainer *o, struct openTIDAL_AlbumContainer album);
int openTIDAL_StructAddItem (openTIDAL_ContentContainer *o, struct openTIDAL_ItemsContainer item);
int openTIDAL_StructAddArtist (openTIDAL_ContentContainer *o,
                               struct openTIDAL_ArtistContainer artist);
int openTIDAL_StructAddPlaylist (openTIDAL_ContentContainer *o,
                                 struct openTIDAL_PlaylistContainer playlist);
int openTIDAL_StructAddMix (openTIDAL_ContentContainer *o, struct openTIDAL_MixContainer mix);
int openTIDAL_StructAddContributor (openTIDAL_ContentContainer *o,
                                    struct openTIDAL_ContributorContainer contributor);
int openTIDAL_StructAddCredit (openTIDAL_ContentContainer *o,
                               struct openTIDAL_CreditsContainer credit);
int openTIDAL_StructAddLink (openTIDAL_ContentContainer *o, struct openTIDAL_LinkContainer link);

#endif // STRUCT__h
