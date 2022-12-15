#include <cds/str_utils.h>
#include <cds/list.h>
#include <cds/array_functions.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int cds_str_len(const char *str) {
    int len = 0;
    if (!str) {
        return len;
    }
    while (*(str+len) != '\0') {
        len++;
    }
    return len;
}

int cds_str_find(const char *str, const char *pattern, int from, int until) {
    int index = 0;
    int pattern_len = 0;
    int strlen = 0;

    if (!str) {
        return -1;
    }

    // find length of str
    strlen = cds_str_len(str);

    if ((!pattern) || (*pattern == '\0')) {
        //strlen
        return strlen;
    }

    // find length of pattern
    while (*(pattern+pattern_len) != '\0') {
        pattern_len++;
    }

    if (until == -1 || (until > strlen - pattern_len)) {
        until = strlen - pattern_len;
    }

    for (index = from; index <= until; index += 1) {
        if (*(str+index) != *pattern) {
            continue;
        }

        int found = 1;
        for (size_t i = 1; i < pattern_len; i++) {
            if (*(str+index+i) != *(pattern+i)) {
                found = 0;
                break;
            }
        }

        if (found) {
            return index;
        }
    }
    return -1;
}

int cds_str_rfind(const char *str, const char *pattern, int from) {
    int index = 0;
    int pattern_len = 0;
    int strlen = 0;
    int until = 0;

    if (!str) {
        return -1;
    }

    // find length of str
    strlen = cds_str_len(str);

    if (!pattern || !str) {
        return 0;
    }

    // find length of pattern
    while (*(pattern+pattern_len) != '\0') {
        pattern_len++;
    }

    if (until == -1) {
        until = 0;
    }

    if (from == -1 || ((from + pattern_len) >= strlen)) {
        from = strlen - pattern_len;
    }

    for (index = from; index >= until; index -= 1) {
        if (*(str+index) != *pattern) {
            continue;
        }

        int found = 1;
        for (size_t i = 1; i < pattern_len; i++) {
            if (*(str+index+i) != *(pattern+i)) {
                found = 0;
                break;
            }
        }

        if (found) {
            return index;
        }
    }
    return -1;
}

char* cds_str_substr(const char *str, size_t start, size_t end) {
    if (end<=start || !str)
        return NULL;
    size_t n_bytes = end-start;

    char *result = malloc(n_bytes+1);
    memcpy(result, (str+start), n_bytes);
    result[n_bytes] = '\0';
    return result;
}

/*
 * Sets va_args param, much like sscanf
 */
void set_param_(va_list args, const char* str, char specifier) {
    if (!str) {
        return;
    }
    switch (specifier) {
        case 's':
            char *ret = va_arg(args, char*);
            sscanf(str, "%s", ret);
            break;
        case 'd':
            int *i = va_arg(args, int*);
            sscanf(str, "%d", i);
            break;
        default:

    }

}

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
const char* cds_str_read(const char *str, const char *fmt, ...) {
    if (!str || !fmt) {
        return str;
    }
    va_list args;
    va_start(args, fmt);

    int str_len = cds_str_len(str);
    int fmt_len = cds_str_len(fmt);

    cds_list *fmt_specifiers = cds_list_create(sizeof(size_t));

    //find all fmt specifiers
    for (size_t i = 0; i < fmt_len; i++) {
        if (*(fmt+i) == '%') {
            cds_list_insert(fmt_specifiers, &i);
        }
    }

    cds_list_entry *e = fmt_specifiers->entry;
    for (size_t i = 0; i < fmt_specifiers->size; i++) {

        e = e->next;
    }

    int prefix_idx = 0;       // index of prefix start in fmt
    int spec_idx = -2;         // index of specifier in fmt
    int end_idx = 0;          // index of next specifier in fmt
    int str_prefix_pos = 0;   // position of prefix in str
    int str_postfix_pos = 0;  // position of postfix in str
    int latest_find_end = 0;  // position of end of latest find

    while (fmt_specifiers->size) {
        prefix_idx = spec_idx+2;
        spec_idx = *(int*)cds_list_peek(fmt_specifiers);
        cds_list_pop(fmt_specifiers);


        if (fmt_specifiers->size) {
            end_idx = *(int*)cds_list_peek(fmt_specifiers);
        } else {
            end_idx = fmt_len;
        }

        char *prefix = cds_str_substr(fmt, prefix_idx, spec_idx);
        char *postfix = cds_str_substr(fmt, spec_idx+2, end_idx);

        // now we need to find pre/postfix in actual string
        // then we take the "string" between them

        str_postfix_pos = cds_str_find(str, postfix, 0, -1);
        // found potential postfix
        // now search backwards for prefix
        // if prefix is not found, search next postfix
        str_prefix_pos = cds_str_rfind(str, prefix, str_postfix_pos - cds_str_len(prefix));

        while(str_prefix_pos == -1) {
            str_postfix_pos = cds_str_find(str, postfix, str_postfix_pos + 1, -1);
            if (str_postfix_pos == -1) {
                char *fmt_str = cds_str_substr(fmt, prefix_idx, end_idx);
                printf("DID NOT FIND ANY MATCH FOR '%s'\n", fmt_str);
                free(fmt_str);
                if (prefix) {
                    free(prefix);
                }
                if (postfix) {
                    free(postfix);
                }
                //did not find any match
                cds_list_destroy(&fmt_specifiers);
                va_end(args);
                return str + latest_find_end;
            }
            str_prefix_pos = cds_str_rfind(str, prefix, str_postfix_pos - cds_str_len(prefix));
        }

        char *result = cds_str_substr(str, str_prefix_pos + cds_str_len(prefix), str_postfix_pos);

        latest_find_end = str_postfix_pos;
        set_param_(args, result, *(fmt+spec_idx+1));
        free(prefix);
        free(postfix);
        free(result);
    }
    va_end(args);
    cds_list_destroy(&fmt_specifiers);

    //check how much of the string we parsed
    if (str_postfix_pos == str_len) {
        return NULL;
    }
    return (str + latest_find_end);
}

size_t cds_str_count(const char *str, const char *format) {
    if (!str || !format) {
        return 0;
    }
    int i = -1;
    i = cds_str_find(str, format, i+1, -1);
    size_t total = 0;
    while (i  >= 0) {
        total++;
        i = cds_str_find(str, format, i+1, -1);
    }
    return total;
}

cds_list* cds_str_split(const char *str, const char *format) {
    cds_list *result = cds_list_create(sizeof(char*));
    size_t len = cds_str_len(str);
    size_t fmt_len = cds_str_len(format);

    size_t start = 0;
    size_t end = -1;
    end = cds_str_find(str, format, start, len);
    while (end != -1) {
        char *substr = cds_str_substr(str, start, end);
        cds_list_insert(result, &substr);
        start = end + fmt_len;
        end = cds_str_find(str, format, start, len);
    }
    char *substr = cds_str_substr(str, start, len);
    cds_list_insert(result, &substr);
    return result;
}

const char* cds_str_replace(const char *str, const char *search, const char *replace) {
    size_t strlen = cds_str_len(str);
    size_t searchlen = cds_str_len(search);
    size_t replacelen = cds_str_len(replace);
    size_t n_words = cds_str_count(str, search);

    size_t new_len = strlen + (n_words * (replacelen - searchlen));
    char *newstr = malloc(new_len+1);

    size_t start = 0;
    size_t end = -1;
    char *p = newstr;
    end = cds_str_find(str, search, start, strlen);
    while (end != -1) {
        size_t n_bytes = end-start;
        memcpy(p, &str[start], n_bytes);
        p += n_bytes;
        memcpy(p, replace, replacelen);
        p += replacelen;
        start = end + searchlen;
        end = cds_str_find(str, search, start, strlen);
    }
    memcpy(p, &str[start], strlen-start);
    newstr[new_len] = '\0';
    return newstr;
}

int compare_chars_(uint8_t *a, uint8_t *b) {
    return *(char*)a > *(char*)b;
}

const char* cds_str_sorted(const char *str) {
    size_t strlen = cds_str_len(str);
    if (strlen<=0) {
        return NULL;
    }
    char *sorted = malloc(strlen+1);
    memcpy(sorted, str, strlen);
    sorted[strlen] = '\0';
    cds_af_sort((uint8_t*)sorted, strlen, sizeof(char), compare_chars_);
    return sorted;
}


