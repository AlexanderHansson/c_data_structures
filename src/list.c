#include <cds/list.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

cds_list* cds_list_create(void *data) {
    cds_list* list = (cds_list*) malloc(sizeof(cds_list));
    if (list == NULL) {
        errno = ENOMEM;
        return NULL;
    }
    memset(list, 0, sizeof(cds_list));
    list->data = data;
    list->next = list;
    list->prev = list;
    return list;
}

void cds_list_destroy(cds_list **list) {
    cds_list *end = (*list)->prev;
    cds_list *p = *list;

    while (p != end) {
        cds_list *next = p->next;
        free(p);
        p = next;
    }
    free(end);

    *list = NULL;
}

void cds_list_insert(cds_list *list, void *data) {
    cds_list *entry = cds_list_create(data);
    entry->prev = list->prev;
    entry->prev->next = entry;

    list->prev = entry;
    entry->next = list;
}

size_t cds_list_size(cds_list *list) {
    cds_list *p = list->next;
    size_t n = 1;
    while (p != list) {
        n++;
        p = p->next;
    }
    return n;
}

