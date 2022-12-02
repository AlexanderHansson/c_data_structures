#ifndef CDS_LIST_H
#define CDS_LIST_H

/*********INCLUDES**********/
#include <stdlib.h>

/***********TYPES***********/
struct cds_list_entry {
    void *data;
    struct cds_list_entry *next;
    struct cds_list_entry *prev;
};

struct cds_list {
    struct cds_list_entry *entry;
    size_t size;
    size_t data_size;
};

typedef struct cds_list cds_list;
typedef struct cds_list_entry cds_list_entry;

/***********FUNCS***********/
cds_list* cds_list_create(size_t data_size);

void cds_list_clear(cds_list *list);

void cds_list_destroy(cds_list **list);

void cds_list_insert(cds_list *list, void *data);

size_t cds_list_size(cds_list *list);

void* cds_list_peek_back(cds_list *list);

void* cds_list_peek(cds_list *list);

void* cds_list_pop(cds_list *list);

void* cds_list_pop_back(cds_list *list);

int cds_list_contains(cds_list *list, void *data, int(*equals)(void *a, void *b));

void* cds_list_max(cds_list *list, int(*a_gt_b)(void *a, void *b));

void* cds_list_min(cds_list *list, int(*a_gt_b)(void *a, void *b));

void cds_list_remove(cds_list *list, void *data, int(*equals)(void *a, void *b));

void cds_print_list(cds_list *list, void(*print_element)(void*));

/***********TODO***********/
cds_list* cds_list_sort(cds_list *list);

#endif
