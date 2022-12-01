#ifndef CDS_LIST_H
#define CDS_LIST_H

/*
 * INCLUDES
 */
#include <stdlib.h>

struct cds_list {
    void *data;
    struct cds_list *next;
    struct cds_list *prev;
};

/*
 * TYPES
 */
typedef struct cds_list cds_list;

/*
 * FUNCTIONS
 */
cds_list* cds_list_create(void* data);

void cds_list_destroy(cds_list **list);

void cds_list_insert(cds_list *list, void *data);

size_t cds_list_size(cds_list *list);

//TODO
void* cds_list_pop(cds_list *list);
void* cds_list_pop_back(cds_list *list);
//TODO
cds_list* cds_list_sort(cds_list *list);

#endif
