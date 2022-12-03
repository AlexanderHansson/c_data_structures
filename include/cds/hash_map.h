#ifndef CDS_HASH_MAP_H
#define CDS_HASH_MAP_H

/*********INCLUDES**********/
#include <cds/list.h>
#include <cds/pair.h>

/***********TYPES***********/
struct cds_hash_map {
    size_t key_bytes;
    size_t val_bytes;
    size_t vec_len;
    cds_list **vec;
};

typedef struct cds_hash_map cds_hash_map;

/***********FUNCS***********/

cds_hash_map* cds_hash_map_create(size_t key_bytes, size_t val_bytes, size_t vec_len);

cds_hash_map* cds_hash_map_create_default(size_t key_bytes, size_t val_bytes);


#endif
