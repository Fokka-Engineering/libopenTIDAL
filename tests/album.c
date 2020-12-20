#include <stdio.h>
#include <stdlib.h>

#include "../src/include/openTIDAL.h"

int i;
int type = 0; /* Identifier for Dynamic Array */
int main()
{
  openTIDAL_InitDemo();

  openTIDAL resolve = openTIDAL_GetAlbum(4753719);
  for (i = 0; i < resolve.total[0]; ++i)
  {
    int artists;

    printf("Title: %s\n", resolve.albums[i].title);

    for (artists = 0; artists < resolve.albums[i].subArraySize; ++artists)
    {
      printf("ArtistName #%d: %s\n", artists, resolve.albums[i].artistName[artists]);
    }
  }
  
  openTIDAL_StructDelete(&resolve);
  openTIDAL_Cleanup();
  return 0;
}
