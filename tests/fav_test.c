#include "../src/include/openTIDAL.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
  init("/Users/hugo/Documents/oT-config.json");

  search_model res = get_search("Sting", 50);
  //get_favorite_playlist(100, 0, "DATE", "ASC");
}
