#include <cds/array_functions.h>
#include <stdio.h>
#include <string.h>

void print_array(int *arr, size_t arrlen) {
    printf("[");
    for (size_t i = 0; i < arrlen; i++) {
        printf("%d,",arr[i]);
    }
    printf("]\n");
}

int compare(uint8_t *a, uint8_t *b) {
    int x;
    int y;
    size_t n_bytes = sizeof(int);
    memcpy(&x, a, n_bytes);
    memcpy(&y, b, n_bytes);
    return x > y;
}

int main() {
    int arr[10] = {5,6,8,4,3,6,123,5,6,2341};
    int arrlen = sizeof(arr)/sizeof(arr[0]);
    print_array(arr, arrlen);
    void *ret = cds_af_min((uint8_t*)arr, arrlen, sizeof(int), compare);

    if (ret) {
        int min = *(int*)ret;
        printf("min: %d\n", min);
    }

    ret = cds_af_max((uint8_t*)arr, arrlen, sizeof(int), compare);

    if (ret) {
        int max = *(int*)ret;
        printf("max: %d\n", max);
    }

    printf("sorted\n");
    cds_af_sort((uint8_t*)arr, arrlen, sizeof(int), compare);
    print_array(arr, arrlen);
}

