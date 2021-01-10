/*
  Copyright (c) 2009-2020 Dave Gamble, Hugo Melder and contributors

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

/* OTJson manipulation
 */

#ifndef OTJSON__h
#define OTJSON__h

#if !defined(__WINDOWS__)                                                                          \
    && (defined(WIN32) || defined(WIN64) || defined(_MSC_VER) || defined(_WIN32))
#define __WINDOWS__
#endif

#ifdef __WINDOWS__
#define CJSON_CDECL __cdecl
#else /* !__WINDOWS__ */
#define CJSON_CDECL
#endif

#include <stddef.h>

/* struct OTJsonContainer Types: */
#define OTJsonInvalid (0)
#define OTJsonFalse (1 << 0)
#define OTJsonTrue (1 << 1)
#define OTJsonNULL (1 << 2)
#define OTJsonNumber (1 << 3)
#define OTJsonString (1 << 4)
#define OTJsonArray (1 << 5)
#define OTJsonObject (1 << 6)
#define OTJsonRaw (1 << 7) /* raw json */

#define OTJsonIsReference 256
#define OTJsonStringIsConst 512

typedef struct OTJsonHooks
{
    /* malloc/free are CDECL on Windows regardless of the default calling convention of the
     * compiler, so ensure the hooks allow passing those functions directly. */
    void *(CJSON_CDECL *malloc_fn) (size_t sz);
    void (CJSON_CDECL *free_fn) (void *ptr);
} OTJsonHooks;

/* Limits how deeply nested arrays/objects can be before struct OTJsonContainer rejects to parse
 * them. This is to prevent stack overflows. */
#ifndef CJSON_NESTING_LIMIT
#define CJSON_NESTING_LIMIT 1000
#endif

/* Supply malloc, realloc and free functions to struct OTJsonContainer */
void OTJsonInitHooks (OTJsonHooks *hooks);

/* Memory Management: the caller is always responsible to free the results from all variants of
 * OTJsonParse (with OTJsonDelete) and OTJsonPrint (with stdlib free, OTJsonHooks.free_fn, or
 * OTJsonfree as appropriate). The exception is OTJsonPrintPreallocated, where the caller has
 * full responsibility of the buffer. */
/* Supply a block of JSON, and this returns a struct OTJsonContainer object you can interrogate.
 */
struct OTJsonContainer *OTJsonParse (const char *value);
/* ParseWithOpts allows you to require (and check) that the JSON is null terminated, and to
 * retrieve the pointer to the final byte parsed. */
/* If you supply a ptr in return_parse_end and parsing fails, then return_parse_end will contain
 * a pointer to the error so will match OTJsonGetErrorPtr(). */
struct OTJsonContainer *OTJsonParseWithOpts (const char *value, const char **return_parse_end,
                                             int require_null_terminated);
struct OTJsonContainer *OTJsonParseWithLengthOpts (const char *value, size_t buffer_length,
                                                   const char **return_parse_end,
                                                   int require_null_terminated);

/* Render a struct OTJsonContainer entity to text for transfer/storage. */
char *OTJsonPrint (const struct OTJsonContainer *item);
/* Render a struct OTJsonContainer entity to text for transfer/storage without any formatting.
 */
char *OTJsonPrintUnformatted (const struct OTJsonContainer *item);
/* Render a struct OTJsonContainer entity to text using a buffered strategy. prebuffer is a
 * guess at the final size. guessing well reduces reallocation. fmt=0 gives unformatted, =1
 * gives formatted */
char *OTJsonPrintBuffered (const struct OTJsonContainer *item, int prebuffer, int fmt);
/* Render a struct OTJsonContainer entity to text using a buffer already allocated in memory
 * with given length. Returns 1 on success and 0 on failure. */
/* NOTE: struct OTJsonContainer is not always 100% accurate in estimating how much memory it
 * will use, so to be safe allocate 5 bytes more than you actually need */
int OTJsonPrintPreallocated (struct OTJsonContainer *item, char *buffer, const int length,
                             const int format);
/* Delete a struct OTJsonContainer entity and all subentities. */
void OTJsonDelete (struct OTJsonContainer *item);

/* These calls create a struct OTJsonContainer item of the appropriate type. */
struct OTJsonContainer *OTJsonCreateNull (void);
struct OTJsonContainer *OTJsonCreateTrue (void);
struct OTJsonContainer *OTJsonCreateFalse (void);
struct OTJsonContainer *OTJsonCreateBool (int boolean);
struct OTJsonContainer *OTJsonCreateNumber (double num);
struct OTJsonContainer *OTJsonCreateString (const char *string);
/* raw json */
struct OTJsonContainer *OTJsonCreateRaw (const char *raw);
struct OTJsonContainer *OTJsonCreateArray (void);
struct OTJsonContainer *OTJsonCreateObject (void);

/* Create a string where valuestring references a string so
 * it will not be freed by OTJsonDelete */
struct OTJsonContainer *OTJsonCreateStringReference (const char *string);
/* Create an object/array that only references it's elements so
 * they will not be freed by OTJsonDelete */
struct OTJsonContainer *OTJsonCreateObjectReference (const struct OTJsonContainer *child);
struct OTJsonContainer *OTJsonCreateArrayReference (const struct OTJsonContainer *child);

/* These utilities create an Array of count items.
 * The parameter count cannot be greater than the number of elements in the number array,
 * otherwise array access will be out of bounds.*/
struct OTJsonContainer *OTJsonCreateIntArray (const int *numbers, int count);
struct OTJsonContainer *OTJsonCreateFloatArray (const float *numbers, int count);
struct OTJsonContainer *OTJsonCreateDoubleArray (const double *numbers, int count);
struct OTJsonContainer *OTJsonCreateStringArray (const char *const *strings, int count);

/* Append item to the specified array/object. */
int OTJsonAddItemToArray (struct OTJsonContainer *array, struct OTJsonContainer *item);
int OTJsonAddItemToObject (struct OTJsonContainer *object, const char *string,
                           struct OTJsonContainer *item);
/* Use this when string is definitely const (i.e. a literal, or as good as), and will definitely
 * survive the struct OTJsonContainer object. WARNING: When this function was used, make sure to
 * always check
 * that (item->type & OTJsonStringIsConst) is zero before writing to `item->string` */
int OTJsonAddItemToObjectCS (struct OTJsonContainer *object, const char *string,
                             struct OTJsonContainer *item);
/* Append reference to item to the specified array/object. Use this when you want to add an
 * existing struct OTJsonContainer to a new struct OTJsonContainer, but don't want to corrupt
 * your existing struct OTJsonContainer. */
int OTJsonAddItemReferenceToArray (struct OTJsonContainer *array, struct OTJsonContainer *item);
int OTJsonAddItemReferenceToObject (struct OTJsonContainer *object, const char *string,
                                    struct OTJsonContainer *item);

/* Remove/Detach items from Arrays/Objects. */
struct OTJsonContainer *OTJsonDetachItemViaPointer (struct OTJsonContainer *parent,
                                                    struct OTJsonContainer *const item);
struct OTJsonContainer *OTJsonDetachItemFromArray (struct OTJsonContainer *array, int which);
void OTJsonDeleteItemFromArray (struct OTJsonContainer *array, int which);
struct OTJsonContainer *OTJsonDetachItemFromObject (struct OTJsonContainer *object,
                                                    const char *string);
void OTJsonDeleteItemFromObject (struct OTJsonContainer *object, const char *string);

/* Update array items. */
int OTJsonInsertItemInArray (
    struct OTJsonContainer *array, int which,
    struct OTJsonContainer *newitem); /* Shifts pre-existing items to the right. */
int OTJsonReplaceItemViaPointer (struct OTJsonContainer *const parent,
                                 struct OTJsonContainer *const item,
                                 struct OTJsonContainer *replacement);
int OTJsonReplaceItemInArray (struct OTJsonContainer *array, int which,
                              struct OTJsonContainer *newitem);
int OTJsonReplaceItemInObject (struct OTJsonContainer *object, const char *string,
                               struct OTJsonContainer *newitem);

/* Duplicate a struct OTJsonContainer item */
struct OTJsonContainer *OTJsonDuplicate (const struct OTJsonContainer *item, int recurse);
/* Duplicate will create a new, identical struct OTJsonContainer item to the one you pass, in
 * new memory that will need to be released. With recurse!=0, it will duplicate any children
 * connected to the item. The item->next and ->prev pointers are always zero on return from
 * Duplicate. */
/* Recursively compare two struct OTJsonContainer items for equality. If either a or b is NULL
 * or invalid, they will be considered unequal. case_sensitive determines if object keys are
 * treated case sensitive (1) or case insensitive (0)
 */
int OTJsonCompare (const struct OTJsonContainer *const a, const struct OTJsonContainer *const b,
                   const int case_sensitive);

/* Minify a strings, remove blank characters(such as ' ', '\t', '\r', '\n') from strings.
 * The input pointer json cannot point to a read-only address area, such as a string constant,
 * but should point to a readable and writable adress area. */
void OTJsonMinify (char *json);

/* Helper functions for creating and adding items to an object at the same time.
 * They return the added item or NULL on failure. */
struct OTJsonContainer *OTJsonAddNullToObject (struct OTJsonContainer *const object,
                                               const char *const name);
struct OTJsonContainer *OTJsonAddTrueToObject (struct OTJsonContainer *const object,
                                               const char *const name);
struct OTJsonContainer *OTJsonAddFalseToObject (struct OTJsonContainer *const object,
                                                const char *const name);
struct OTJsonContainer *OTJsonAddBoolToObject (struct OTJsonContainer *const object,
                                               const char *const name, const int boolean);
struct OTJsonContainer *OTJsonAddNumberToObject (struct OTJsonContainer *const object,
                                                 const char *const name, const double number);
struct OTJsonContainer *OTJsonAddStringToObject (struct OTJsonContainer *const object,
                                                 const char *const name, const char *const string);
struct OTJsonContainer *OTJsonAddRawToObject (struct OTJsonContainer *const object,
                                              const char *const name, const char *const raw);
struct OTJsonContainer *OTJsonAddObjectToObject (struct OTJsonContainer *const object,
                                                 const char *const name);
struct OTJsonContainer *OTJsonAddArrayToObject (struct OTJsonContainer *const object,
                                                const char *const name);

/* When assigning an integer value, it needs to be propagated to valuedouble too. */
#define OTJsonSetIntValue(object, number)                                                          \
    ((object) ? (object)->valueint = (object)->valuedouble = (number) : (number))
/* helper for the OTJsonSetNumberValue macro */
double OTJsonSetNumberHelper (struct OTJsonContainer *object, double number);
#define OTJsonSetNumberValue(object, number)                                                       \
    ((object != NULL) ? OTJsonSetNumberHelper (object, (double)number) : (number))
/* Change the valuestring of a OTJsonString object, only takes effect when type of object is
 * OTJsonString */
char *OTJsonSetValuestring (struct OTJsonContainer *object, const char *valuestring);

/* malloc/free objects using the malloc/free functions that have been set with OTJsonInitHooks
 */
void *OTJsonmalloc (size_t size);
void OTJsonfree (void *object);

#endif
