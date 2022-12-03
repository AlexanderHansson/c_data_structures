#include <stdio.h>
#include <cds/cds.h>

void print_list(cds_list *list) {
    cds_list_entry *e = list->entry;
    printf("list(%d): ", cds_list_size(list));

    if (e) {
        printf("peek(%d) - ", *(int*)cds_list_peek(list));
        cds_list_entry *end = list->entry->prev;
        while (e != end) {
            printf("%d,", *(int*)e->data);
            e = e->next;
        }
        printf("%d", *(int*)e->data);
        printf(" - peekb(%d)", *(int*)cds_list_peek_back(list));
    }
    printf("\n\n");
}

void print_element(void* data) {
    printf("%d ", *(int*)data);
}

int compare_elements(void *a, void *b) {
    return (*(int*)a) > (*(int*)b);
}

int equals(void *a, void *b) {
    return (*(int*)a) == (*(int*)b);
}

int main(int argc, char* argv[]) {
    size_t numbers[] = {5,8,1123,7,9,4,3,12};
    cds_list *list = cds_list_create(sizeof(size_t));
    print_list(list);

    size_t len = sizeof(numbers) / sizeof(numbers[0]);
    for (int i = 0; i < len; i++) {
        printf("insert %d\n", numbers[i]);
        cds_list_insert(list, &numbers[i]);
        print_list(list);
    }

    cds_list_pop(list);
    printf("pop\n");
    print_list(list);

    cds_list_pop_back(list);
    printf("pop_back\n");
    print_list(list);

    printf("print list:\n");
    cds_list_print(list, print_element);
    printf("\n");

    int x = 3;
    printf("contains %d: %d\n\n", x, cds_list_contains(list, &x, equals));

    x = 1;
    printf("contains %d: %d\n\n", x, cds_list_contains(list, &x, equals));

    printf("max: %d\n\n", *(int*)cds_list_max(list, compare_elements));

    printf("min: %d\n\n", *(int*)cds_list_min(list, compare_elements));

    printf("print list:\n");
    cds_list_print(list, print_element);
    printf("\n");

    cds_list_remove(list, &x, equals);
    printf("remove %d:\n", x);
    cds_list_print(list, print_element);
    printf("\n");

    x = 7;
    cds_list_remove(list, &x, equals);
    printf("remove %d:\n", x);
    cds_list_print(list, print_element);
    printf("\n");

    printf("list\n");
    cds_list_print(list, print_element);
    cds_list_sort(list, compare_elements);
    printf("\nsorted\n");
    cds_list_print(list, print_element);
    printf("\n");

    printf("clear list:\n");
    cds_list_clear(list);
    print_list(list);

    printf("destroy list:\n");
    cds_list_destroy(&list);
    if (!list) {
        printf("correctly cleaned up\n");
    }

    return 0;
}
