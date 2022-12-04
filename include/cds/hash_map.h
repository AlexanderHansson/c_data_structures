#ifndef CDS_HASH_MAP_H
#define CDS_HASH_MAP_H

/*********INCLUDES**********/
#include <cds/list.h>
#include <cds/pair.h>

/***********TYPES***********/
struct cds_hash_map {
    size_t size;      //no (k,v) entries in map
    size_t vec_len;   //length of vector
    cds_list **vec;   //"array" of lists
};

typedef struct cds_hash_map cds_hash_map;
/***********FUNCS***********/

cds_hash_map* cds_hash_map_create(size_t vec_len);

cds_hash_map* cds_hash_map_create_default();

void cds_hash_map_destroy(cds_hash_map **map);

void cds_hash_map_insert(cds_hash_map *map,
                       void *key,
                       void *val,
                       size_t key_size,
                       size_t val_size,
                       unsigned int (*hash)(void *key, size_t key_size),
                       int(*equals)(void *key_a, void *key_b,
                                    size_t key_a_size, size_t key_b_size));

void* cds_hash_map_get(cds_hash_map *map,
                       void *key,
                       size_t key_size,
                       unsigned int (*hash)(void *key, size_t key_size),
                       int(*equals)(void *key_a, void *key_b,
                                    size_t key_a_size, size_t key_b_size));

void cds_hash_map_remove(cds_hash_map *map,
                       void *key,
                       size_t key_size,
                       unsigned int (*hash)(void *key, size_t key_size),
                       int(*equals)(void *key_a, void *key_b,
                                    size_t key_a_size, size_t key_b_size));

cds_list* cds_hash_map_keys(cds_hash_map *map);

void cds_hash_map_print(cds_hash_map *map, void(*print_key)(void*), void(*print_val)(void*));

//TODO: rebalance on insert?
#endif
