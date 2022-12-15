#ifndef CDS_STR_UTILS_H
#define CDS_STR_UTILS_h

/*********INCLUDES**********/
#include <stddef.h>
#include <cds/list.h>

/***********TYPES***********/
//-
/***********FUNCS***********/

/*
 * Finds @pattern in @str from index @from until index @until
 */
int cds_str_find(const char *str, const char *pattern, int from, int until);

/*
 * Reverse-finds @pattern in @str from index @from
 */
int cds_str_rfind(const char *str, const char *pattern, int from);

/*
 * Returns length of @str
 * expects NULL-termination
 */
int cds_str_len(const char *str);

/*
* cds_str_substr("hello my name is", 0, 5)
* will allocate a new (null-terminated) string called "hello"
*/
char* cds_str_substr(const char *str, size_t start, size_t end);

/*
*
* cds_str_read("hello my name is Alex !", "%s my name is %s ", &greeting, &name)
* will read "hello" into greeting
* and "Alex" into name
*
* @str:     String input  e.g. "hello my name is johan".
* @fmt:     Format string e.g. "name is %s".
* @...:     Variable length arguments
*           You can pass any char*'s or int*'s to this function
*           to properly match the format string.
*
* @return:  pointer to @str after last parsed argument.
*/
const char* cds_str_read(const char *str, const char *format, ...);

size_t cds_str_count(const char *str, const char *format);

cds_list* cds_str_split(const char *str, const char *format);

const char* cds_str_replace(const char *str, const char *search, const char *replace);

const char* cds_str_sorted(const char *str);



#endif

