
#include <cds/str_utils.h>
#include <stdio.h>


int main() {
    char str[100] = "Alex is happy";
    char name[100] = {0};
    char mood[100] = {0};

    const char *p = cds_str_read(str, "%s is %s", &name, &mood);
    if (!p) {
        printf("null returned\n");
    } else {
        printf("NOT null returned: '%s'\n",p);
    }
    printf("name: '%s'\n", name);
    printf("mood: '%s'\n", mood);
    printf("num p's: %d\n", cds_str_count(str, "p"));
    cds_list* splitted = cds_str_split(str, " ");
    printf("SPLITTED, size: %d\n", splitted->size);
    while (splitted->size) {
        char *word = *(char**)cds_list_peek(splitted);
        printf("%s\n",word);
        cds_list_pop(splitted);
        free(word);
    }
    cds_list_destroy(&splitted);

    const char *repl = cds_str_replace(str, "Alex", "John");
    printf("replaced str:\n%s\n",repl);
    free((void*)repl);

    const char *sorted = cds_str_sorted(str);
    printf("sorted str:\n%s\n",sorted);
    free((void*)sorted);
    return 0;
}
