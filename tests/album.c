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
    openTIDAL_AlbumModel album = resolve.albums[i];

    printf("Title: %s\n", album.title);

    for (artists = 0; artists < album.subArraySize; ++artists)
    {
      printf("ArtistName #%d: %s\n", artists, album.artistName[artists]);
    }
  }

  openTIDAL_StructDelete(&resolve);
  openTIDAL_Cleanup();
  return 0;
}
