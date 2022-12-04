#include <cds/array_functions.h>
#include <stddef.h>

void* cds_af_max(uint8_t *array, size_t n_elements, size_t element_size_bytes, int(*a_gt_b)(uint8_t *a, uint8_t *b)) {
    uint8_t *p = array;
    uint8_t *el = array;
    for (size_t i = 0; i < n_elements; i++) {
        if (a_gt_b(p, el)) {
            el = p;
        }
        p = p + element_size_bytes;
    }
    return el;
}

void* cds_af_min(uint8_t *array, size_t n_elements, size_t element_size_bytes, int(*a_gt_b)(uint8_t *a, uint8_t *b)) {
    uint8_t *p = array;
    uint8_t *el = array;
    for (size_t i = 0; i < n_elements; i++) {
        if (a_gt_b(el, p)) {
            el = p;
        }
        p = p + element_size_bytes;
    }
    return el;
}

void* cds_af_sort(uint8_t *array, size_t n_elements, size_t element_size_bytes, int(*a_gt_b)(uint8_t *a, uint8_t *b)) {
    //sort in place?
    // add remove, append, sublist?
    //TODO
    uint8_t temp[n_elements*element_size_bytes];
}
