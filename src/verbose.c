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

#include "include/openTIDAL.h"

/* Loglevel:
 *   0 = Disabled
 *   1 = Error
 *   2 = Debug
 *   3 = Trace*/

int globalLoglevel = 0;

void openTIDAL_Verbose(int loglevel)
{
  globalLoglevel = loglevel;
}

void openTIDAL_ParseVerbose(const char *prefix, const char *string, int loglevel)
{
  if (loglevel <= globalLoglevel)
  {
    switch (loglevel)
    {
      case 1:
        fprintf(stderr, "[%s] %s.\n", prefix, string);
        break;

      default:
        fprintf(stdout, "[%s] %s.\n", prefix, string);
        break;
    }
  }

}

int openTIDAL_GetLogLevel()
{
  return globalLoglevel;
}
