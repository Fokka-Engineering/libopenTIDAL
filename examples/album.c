#include <stdio.h>
#include <stdlib.h>

#include "../src/include/openTIDAL.h"

int i;
int type = 0; /* Identifier for Dynamic Array */
int main()
{
    //openTIDAL_Init(NULL);
    openTIDAL_Init("/Users/hugo/Documents/oT-config");
    openTIDAL_Verbose(3);

    printf("\nGet Album\n");
    openTIDAL resolve = openTIDAL_GetAlbum(4753719);
    if (resolve.status == 1)
    {
        for (i = 0; i < resolve.total[0]; ++i)
        {
            int artists;

            printf("Title: %s\n", resolve.albums[i].title);

            for (artists = 0; artists < resolve.albums[i].subArraySize; ++artists)
            {
                printf("ArtistName #%d: %s\n", artists, resolve.albums[i].artistName[artists]);
            }
        }
    }
    openTIDAL_StructDelete(&resolve);

    printf("\nGet Album Items\n");
    openTIDAL resolveTwo = openTIDAL_GetAlbumItems(4753719, 20, 0);
    if (resolveTwo.status == 1)
    {
        for (i = 0; i < resolveTwo.total[1]; ++i)
        {
            int artists;

            printf("Title: %s\n", resolveTwo.items[i].title);

            for (artists = 0; artists < resolveTwo.items[i].subArraySize; ++artists)
            {
                printf("ArtistName #%d: %s\n", artists, resolveTwo.items[i].artistName[artists]);
            }
        }
    }  
    openTIDAL_StructDelete(&resolveTwo); 
    
    printf("\nGet Favorite Albums\n");
    openTIDAL resolveThree = openTIDAL_GetFavoriteAlbums(10, 0, "DATE", "ASC");
    printf("Status: %d\n", resolveThree.status); 
    if (resolveThree.status == 1)
    {
        for (i = 0; i < resolveThree.total[0]; ++i)
        {
            int artists;

            printf("Title: %s\n", resolveThree.albums[i].title);

            for (artists = 0; artists < resolveThree.albums[i].subArraySize; ++artists)
            {
                printf("ArtistName #%d: %s\n", artists, resolveThree.albums[i].artistName[artists]);
            }
        }
    }
    openTIDAL_StructDelete(&resolveThree);


    printf("\nAdd Favorite Albums\n");
    int status = 0;
    status = openTIDAL_AddFavoriteAlbum(13490893);
    printf("Status: %d\n", status);


    printf("Testing\n");
    printf("\nRemove Favorite Albums\n");
    int statusTwo = openTIDAL_DeleteFavoriteAlbum(13490893);
    printf("Status: %d\n", statusTwo);
 
    printf("\nAdd Favorite Albums\n");
    status = 0;
    status = openTIDAL_AddFavoriteAlbum(13490893);
    printf("Status: %d\n", status);


    openTIDAL_Cleanup();
    return 0;
}
