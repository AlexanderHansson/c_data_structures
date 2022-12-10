#ifndef CDS_STR_UTILS_H
#define CDS_STR_UTILS_h

#include <stddef.h>

int cds_str_find(const char *str, const char *pattern, int from, int until);

int cds_str_rfind(const char *str, const char *pattern, int from);

int cds_str_len(const char *str);

char* cds_str_substr(const char *str, size_t start, size_t end);

const char* cds_str_read(const char *str, const char *format, ...);

#endif

