#include <stdio.h>
#include <cds/cds.h>


int main(int argc, char* argv[]) {
    cds_list l;
    l.data = 10;
    cds_list_print_data(l);
    return 0;
}
