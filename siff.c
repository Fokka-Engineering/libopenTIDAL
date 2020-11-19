#include <stdio.h>
#include <stdlib.h>
#include "src/include/openTIDAL.h"

int main()
{
  init("/Users/admin/Desktop/Wanna Die Actually/persistent.json");

  delete_playlist_item("7079f26e-174c-4012-8c3e-1f27b8189596", 0);
  int status = add_playlist_item("7079f26e-174c-4012-8c3e-1f27b8189596", 32389048, "ADD");
  printf("%d\n", status);
  playlist_model res = get_playlist("a69f746c-67cb-4a27-9036-773adceff3e2");
  printf("%d\n", res.status);
  cleanup();
}
