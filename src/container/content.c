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

#include "../include/openTIDAL.h"

/* Initialised with openTIDAL_StructInit({pointer to container}). 
 * See structCtrl.c for more detailed documentation. */
struct openTIDAL_ContentContainer {
    openTIDAL_LoginCodeContainer code;
    openTIDAL_LoginTokenContainer token;
    
    openTIDAL_UserContainer user;
    openTIDAL_UserSubscriptionContainer subscription;

    /* Pointer to dynamic arrays. Allocated one by one only if
     * needed. The arrays can also function as content pools in
     * case of module based endpoints (home endpoint). In this case
     * a direct position to the content is given. 
     * Each array has it's own identifier to retrieve the correct capacity
     * and total items in array values. 
     * The arrays are populated with helper functions (openTIDAL_StructAdd*)
     * and grow automatically heap by doubling it's current size. This prevents
     * the expensive use of realloc.
     * The initial capacity is 10.
     * To deallocate all arrays inside the ContentContainer call
     * openTIDAL_StructDelete({pointer to container}). */
    openTIDAL_AlbumContainer *albums; /* Index (Capacity & Total): 0 */
    openTIDAL_ItemsContainer *items; /* Index (Capacity & Total): 1 */
    openTIDAL_ArtistContainer *artists; /* Index (Capacity & Total): 2 */
    openTIDAL_PlaylistContainer *playlists; /* Index (Capacity & Total): 3 */
    openTIDAL_MixContainer *mixes; /* Index (Capacity & Total): 4 */
    openTIDAL_ContributorContainer *contributors; /* Index (Capacity & Total): 5 */
    openTIDAL_CreditsContainer *credits; /* Index (Capacity & Total): 6 */
    openTIDAL_LinkContainer *links; /* Index (Capacity & Total): 7 */

    openTIDAL_StreamContainer stream;
    openTIDAL_PageHomeContainer home;

    /* Custom status of performed request. Error handling of the tidalapi and
     * primitive http response codes are parsed to provide detailed information
     * about the state of the request. */
    int status;

    /* This static array is being used to store the capacity of each array.
     * The capacity array keeps track of the allocated heap size of
     * each dynamic array.
     * The dynamic array identifiers are used to access the correct values */
    int capacity[8];
    /* This static array is being used to store the current number of items
     * added to each dynamic array.
     * The dynamic array identifiers are used to access the correct values */
    int total[8];
   
    /* Pointer to allocated cJSON Structure. After a request is performed and
     * is successful it's allocated buffer is parsed with cJSON and deallocated. 
     * This pointer is used for deallocation purposes only. */
    void *json;
    /* Pointer to allocated cJSON Structure. After a get stream request is performed
     * and is successful it's stream manifest gets decoded with base64 and pased with
     * cJSON. 
     * This pointer is used for deallocation purposes only. */
    void *jsonManifest;
};

