#include <stdio.h>
#include <stdlib.h>

#include "../src/include/openTIDAL.h"

openTIDAL_SessionContainer session;

int i;
int type = 0; /* Identifier for Dynamic Array */
int main()
{
    //openTIDAL_Init(NULL);
    
    openTIDAL_Verbose(3);
    openTIDAL_SessionInit(&session, NULL);
    //openTIDAL_SessionInit(&session, "/Users/hugo/Documents/oT-config");


    printf("\nGet Album\n");
    openTIDAL_ContentContainer resolve = openTIDAL_GetAlbum(&session, 4753719);
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
    openTIDAL_ContentContainer resolveTwo = openTIDAL_GetAlbumItems(&session, 4753719, 20, 0);
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
    openTIDAL_ContentContainer resolveThree = openTIDAL_GetFavoriteAlbums(&session, 10, 0, "DATE", "ASC");
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

    openTIDAL_SessionCleanup(&session);
    return 0;
}
