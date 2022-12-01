#ifndef CDS_LIST_H
#define CDS_LIST_H

struct cds_list {
    int data;
};
typedef struct cds_list cds_list;

void cds_list_print_data(cds_list);

#endif
