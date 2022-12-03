#include <cds/list.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

cds_list* cds_list_create(size_t data_size) {
    cds_list *list = (cds_list*) malloc(sizeof(cds_list));
    if (list == NULL) {
        errno = ENOMEM;
        return NULL;
    }
    memset(list, 0, sizeof(cds_list));
    list->data_size = data_size;
    return list;
}

//private
void cds_list_entry_destroy(cds_list_entry **e, size_t bytes) {
    free((*e)->data);
    free(*e);
    *e = NULL;
}

//private
cds_list_entry* cds_list_entry_create(void *data, size_t bytes) {

    cds_list_entry *e = (cds_list_entry*) malloc(sizeof(cds_list_entry));

    if (e == NULL) {
        return NULL;
    }
    memset(e, 0, sizeof(cds_list_entry));

    e->next = e;
    e->prev = e;


    e->data = malloc(bytes);
    if (e->data == NULL) {
        cds_list_entry_destroy(&e, bytes);
        return e;
    }
    memcpy(e->data, data, bytes);

    return e;
}



void cds_list_clear(cds_list *list) {
    if (!list->entry) {
        return;
    }
    cds_list_entry *head = list->entry;
    cds_list_entry *end = head->prev;
    cds_list_entry *p = head;

    while (head != end) {
        p = p->next;
        cds_list_entry_destroy(&head, list->data_size);
        head = p;
    }

    cds_list_entry_destroy(&head, list->data_size);

    list->entry = NULL;
    list->size = 0;
}

void cds_list_destroy(cds_list **list) {
    cds_list_clear(*list);
    free(*list);
    *list = NULL;
}

void cds_list_insert(cds_list *list, void *data) {
    cds_list_entry *head = list->entry;
    cds_list_entry *e = cds_list_entry_create(data, list->data_size);

    if (!e) {
        printf("FAILED [cds_list] failed inserting elements (no memory)\n");
        return;
    }

    if (!list->entry) {
        list->entry = e;
    } else {
        e->prev = head->prev;
        e->prev->next = e;

        head->prev = e;
        e->next = head;
    }
    list->size++;
}

size_t cds_list_size(cds_list *list) {
    return list->size;
}

void* cds_list_peek(cds_list *list) {
    return list->entry->data;
}

void* cds_list_peek_back(cds_list *list) {
    return list->entry->prev->data;
}

void cds_list_pop(cds_list *list) {
    list->entry = list->entry->next;
    cds_list_pop_back(list);
}

void cds_list_pop_back(cds_list *list) {
    cds_list_entry *head = list->entry;

    if (!head) {
        printf("ERROR [cds_list] attempting to pop empty list\n");
    }

    cds_list_entry *back = head->prev;

    // overwrite head->prev
    head->prev = head->prev->prev;
    head->prev->next = head;

    if (list->size == 1) {
        list->entry = NULL;
        list->size = 0;
    }

    // cleanup back
    back->next = back;
    back->prev = back;
    cds_list_entry_destroy(&back, list->data_size);
    list->size--;
}

void cds_list_print(cds_list *list, void(*print_element)(void*)) {
    if (!list) {
        return;
    }

    cds_list_entry *e = list->entry;

    printf("list(%d): ", list->size);

    if (e) {
        cds_list_entry *end = list->entry->prev;
        while (e != end) {
            print_element(e->data);
            e = e->next;
        }
        print_element(e->data);
    }
    printf("\n");
}

int cds_list_contains(cds_list *list, void *data, int(*equals)(void *a, void *b)) {
    if (!list || !list->entry) {
        return 0;
    }

    cds_list_entry *e = list->entry;

    if (e) {
        cds_list_entry *end = e->prev;
        while (e != end) {
            if (equals(e->data, data)) {
                return 1;
            }
            e = e->next;
        }
        if (equals(e->data, data)) {
            return 1;
        }
    }

    return 0;
}

void* cds_list_max(cds_list *list, int(*a_gt_b)(void *a, void *b)) {
    if (!list || !list->entry) {
        return NULL;
    }

    cds_list_entry *e = list->entry;
    cds_list_entry *end = e->prev;
    void* max = e->data;

    while(e != end) {
        if (a_gt_b(e->data, max)) {
            max = e->data;
        }
        e = e ->next;
    }
    if (a_gt_b(e->data, max)) {
        max = e->data;
    }
    return max;
}

void* cds_list_min(cds_list *list, int(*a_gt_b)(void *a, void *b)) {
    if (!list || !list->entry) {
        return NULL;
    }

    cds_list_entry *e = list->entry;
    cds_list_entry *end = e->prev;
    void* min = e->data;

    while(e != end) {
        if (a_gt_b(min, e->data)) {
            min = e->data;
        }
        e = e ->next;
    }
    if (a_gt_b(min, e->data)) {
        min = e->data;
    }
    return min;
}

void cds_list_remove(cds_list *list, void *data, int(*equals)(void *a, void *b)) {
    if (!list || !list->entry) {
        return;
    }

    cds_list_entry *e = list->entry;
    cds_list_entry *end = e->prev;

    while (e != end) {
        if (equals(data, e->data)) {
            e->prev->next = e->next;
            e->next->prev = e->prev;

            e->next = e;
            e->prev = e;
            cds_list_entry_destroy(&e, list->data_size);
            list->size--;
            return;
        }
        e = e->next;
    }

    if (equals(data, e->data)) {
        e->prev->next = e->next;
        e->next->prev = e->prev;

        e->next = e;
        e->prev = e;
        cds_list_entry_destroy(&e, list->data_size);
        list->size--;
        return;
   }
}

// A silly simple n^2 sort
// TODO: improve with mergesort?
void cds_list_sort(cds_list *list, int(*a_gt_b)(void *a, void *b)) {
    cds_list *temp = cds_list_create(list->data_size);
    temp->entry = list->entry;

    temp->size = list->size;
    list->entry = NULL;
    list->size = 0;
    cds_list_entry *e = temp->entry;
    while (temp->size) {
        cds_list_entry *end = e->prev;
        cds_list_entry *min = e;

        while (e != end) {
            if (a_gt_b(min->data, e->data)) {
                min = e;
            }
            e = e->next;
        }

        if (a_gt_b(min->data, e->data)) {
            min = e;
            e = e->next;
        }

        cds_list_insert(list, min->data);

        min->prev->next = min->next;
        min->next->prev = min->prev;
        temp->size--;
        min->prev = min;
        min->next = min;

        cds_list_entry_destroy(&min, temp->data_size);
    }
    temp->entry = NULL;
    cds_list_destroy(&temp);
}

