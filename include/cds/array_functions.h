#ifndef CDS_ARRAY_FUNCTIONS_H
#define CDS_ARRAY_FUNCTIONS_H

/*********INCLUDES**********/
#include <stddef.h>
/***********TYPES***********/
typedef unsigned char uint8_t;
/***********FUNCS***********/

void* cds_af_sort(uint8_t *array, size_t n_elements, size_t element_size_bytes, int(*a_gt_b)(uint8_t *a, uint8_t *b));

void* cds_af_max(uint8_t *array, size_t n_elements, size_t element_size_bytes, int(*a_gt_b)(uint8_t *a, uint8_t *b));
void* cds_af_min(uint8_t *array, size_t n_elements, size_t element_size_bytes, int(*a_gt_b)(uint8_t *a, uint8_t *b));

#endif
