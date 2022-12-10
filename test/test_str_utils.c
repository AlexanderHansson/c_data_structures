
#include <cds/str_utils.h>
#include <stdio.h>


int main() {
    char str[100] = " billy had a little kid ";
    char s[100];
    char q[100];

    int len = cds_str_len(str);
    int index = cds_str_find(str, "had ", 0, -1);
    printf("%d\n", index);
    printf("len: %d\n", len);
    return 0;
}
