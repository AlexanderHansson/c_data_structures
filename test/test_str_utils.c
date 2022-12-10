
#include <cds/str_utils.h>
#include <stdio.h>


int main() {
    char str[100] = "Alex is happy ";
    char name[100] = {0};
    char mood[100] = {0};

    const char *p = cds_str_read(str, "A%s is %s ", &name, &mood);
    if (!p) {
        printf("null returned\n");
    } else {
        printf("NOT null returned: '%s'\n",p);
    }
    printf("name: %s\n", name);
    printf("mood: %s\n", mood);
    return 0;
}
