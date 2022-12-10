#include <cds/parse_utils.h>
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
    for (index = from; index <= until; index += 1) {
        if (*(str+index) != *pattern) {
            continue;
        }

        int found = 1;
        for (size_t i = 0; i < pattern_len; i++) {
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



/*
 *
 * cds_parse_utils_read("dsf[asd]-!?=asd", "[%s]-", &str)
 * will find string prefixed by [ and postfixed by ]-
 *
 */
/*
char* cds_parse_read(const char *str, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    const char *s = str;

    int spec = cds_parse_find(fmt, "%");
    const char fmt_c = fmt[spec+1];

    int postfix_end = cds_parse_find(fmt+spec+2, "%");

    if (postfix_end == -1) {
        postfix_end = cds_parse_strlen(fmt) -1;
    }

    char prefix[spec+1];
    char postfix[postfix_end-spec-2+1];
    memcpy(postfix, fmt+spec+2, postfix_end-spec-2); 
    memcpy(prefix, fmt, spec);
    prefix[spec] = '\0';
    postfix[postfix_end-spec-2] = '\0';
    printf("prefix: '%s'\n", prefix);
    printf("postfix: '%s'\n", postfix);


    int post = 0;
    int pre = 0;
    find_first_match(str, prefix, postfix, &pre, &post);

    if (pre < 0) {
        return NULL;
    }

    int len = post - pre - cds_parse_strlen(prefix);

    //memcpy ret into arg
    char *ret = va_arg(args, char*);
    memcpy(ret, &s[pre + cds_parse_strlen(prefix)], len);
    ret[len] = '\0';
    str += post;
    va_end(args);
    return str;
}
*/
