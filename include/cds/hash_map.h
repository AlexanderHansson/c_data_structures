#ifndef CDS_HASH_MAP_H
#define CDS_HASH_MAP_H

/*********INCLUDES**********/
#include <cds/list.h>
#include <cds/pair.h>

/***********TYPES***********/
struct cds_hash_map {
    size_t key_bytes; //no bytes for key
    size_t val_bytes; //no bytes for val
    size_t size;      //no (k,v) entries in map
    size_t vec_len;   //length of vector
    cds_list **vec;   //"array" of lists
};

typedef struct cds_hash_map cds_hash_map;
/***********FUNCS***********/

cds_hash_map* cds_hash_map_create(size_t key_bytes, size_t val_bytes, size_t vec_len);

cds_hash_map* cds_hash_map_create_default(size_t key_bytes, size_t val_bytes);

void cds_hash_map_destroy(cds_hash_map **map);

void cds_hash_map_insert(cds_hash_map *map,
                       void *key,
                       void *value,
                       unsigned int (*hash)(void *key),
                       int(*equals)(void *key_a, void *key_b));

void* cds_hash_map_get(cds_hash_map *map,
                       void *key,
                       unsigned int (*hash)(void *key),
                       int(*equals)(void *key_a, void *key_b));

#endif
