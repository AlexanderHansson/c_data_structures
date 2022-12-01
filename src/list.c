#include <cds/list.h>
#include <stdio.h>

void cds_list_print_data(cds_list l) {
    printf("%d\n", l.data);
}

