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
#include "../src/include/openTIDAL.h"
#include <unistd.h>

void login_polling()
{
  login_code_model ress = login_create_code();
  if (ress.status == 1)
  {
    printf("DeviceCode: %s\n", ress.deviceCode);
    printf("UserCode: %s\n", ress.userCode);
    size_t i;
    while (time(NULL) <= ress.expires_in)
    {
      sleep(2);
      login_token_model res = login_create_token(ress.deviceCode);
      if (res.status == 2)
      {
        printf("%s\n", "Authorization Pending...");
      }
      else if (res.status == 1)
      {
        printf("%s\n", "Authorization Successfull!");
        printf("AccessToken: %s\n", res.access_token);
        printf("RefreshToken: %s\n", res.refresh_token);
        printf("Timestamp: %zu\n", res.expires_in);
	printf("Username: %s\n", res.username);
	printf("CountryCode: %s\n", res.countryCode);
	printf("UserId: %zu\n", res.userId);
	break;
      }
      else
      {
        printf("%s\n", "Error! .. Retry\n");
        break;
      }
    }
  }
  else
  {
    fprintf(stderr, "DeviceCode Request failed.\n");
  }
}

int main(void)
{
  init("/Users/admin/Documents/Git/openTIDAL/bin/persistent.json");
  login_polling();
  cleanup();
}
