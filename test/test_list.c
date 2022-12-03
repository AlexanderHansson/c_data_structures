#include <stdio.h>
#include <cds/cds.h>

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
    cds_list_print(list, print_element);

    size_t len = sizeof(numbers) / sizeof(numbers[0]);
    for (int i = 0; i < len; i++) {
        printf("insert %d\n", numbers[i]);
        cds_list_insert(list, &numbers[i]);
        cds_list_print(list, print_element);
    }

    cds_list_pop(list);
    printf("\npop\n");
    cds_list_print(list, print_element);

    cds_list_pop_back(list);
    printf("\npop_back\n");
    cds_list_print(list, print_element);

    printf("\nprint list:\n");
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
    cds_list_print(list, print_element);

    printf("destroy list:\n");
    cds_list_destroy(&list);
    if (!list) {
        printf("correctly cleaned up\n");
    }

    return 0;
}
