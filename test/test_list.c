#include <stdio.h>
#include <cds/cds.h>


int main(int argc, char* argv[]) {
    int x = 10;
    int y = 25;

    cds_list *l = cds_list_create(&x);

    printf("%d\n", *(int*)l->data);
    printf("%d\n", cds_list_size(l));

    cds_list_insert(l, &y);
    printf("%d\n", cds_list_size(l));

    cds_list_destroy(&l);

    if (!l) {
        printf("list properly freed\n");
    }
    return 0;
}
