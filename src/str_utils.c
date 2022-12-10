#include <cds/str_utils.h>
#include <cds/list.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int cds_str_len(const char *str) {
    int len = 0;
    while (*(str+len) != '\0') {
        len++;
    }
    return len;
}

int cds_str_find(const char *str, const char *pattern, int from, int until) {
    int index = 0;
    int pattern_len = 0;
    int strlen = 0;

    // find length of pattern
    while (*(pattern+pattern_len) != '\0') {
        pattern_len++;
    }

    // find length of str
    strlen = cds_str_len(str);

    if (*pattern == '\0') {
        //strlen
        return strlen;
    }

    if (until == -1 || (until > strlen - pattern_len)) {
        until = strlen - pattern_len;
    }
    /*
    printf("trying to find '%s' in '%s'\n", pattern, str);
    printf("from: %d, until: %d\n", from, until);
    printf("strlen %d, patternlen: %d\n", strlen, pattern_len);
    */
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

    // find length of pattern
    while (*(pattern+pattern_len) != '\0') {
        pattern_len++;
    }

    // find length of str
    strlen = cds_str_len(str);

    if (until == -1) {
        until = 0;
    }

    if (from == -1 || ((from + pattern_len) >= strlen)) {
        from = strlen - pattern_len;
    }
    /*
    printf("trying to rfind '%s' in '%s'\n", pattern, str);
    printf("backwards from: %d, until: %d\n", from, until);
    printf("strlen %d, patternlen: %d\n", strlen, pattern_len);
    */
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
            printf("UNSUPPORTED SPECIFIER: %c\n", specifier);
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

    printf("specifiers size:%d\n", fmt_specifiers->size);

    int prefix_idx = 0;       // index of prefix start in fmt
    int spec_idx = 0;         // index of specifier in fmt
    int end_idx = 0;          // index of next specifier in fmt
    int str_prefix_pos = 0;   // position of prefix in str
    int str_postfix_pos = 0;  // position of postfix in str
    int latest_find_end = 0;  // position of end of latest find

    while (fmt_specifiers->size) {
        prefix_idx = spec_idx+2;
        spec_idx = *(int*)cds_list_peek(fmt_specifiers);
        cds_list_pop(fmt_specifiers);
        printf("specifiers size:%d\n", fmt_specifiers->size);

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
                //free(prefix);
                //free(postfix);
                //did not find any match
                //cds_list_destroy(&fmt_specifiers);
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
