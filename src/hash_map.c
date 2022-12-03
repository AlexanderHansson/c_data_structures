#include <cds/hash_map.h>

cds_hash_map* cds_hash_map_create(size_t key_bytes, size_t val_bytes, size_t vec_len) {
    cds_hash_map *m = malloc(sizeof(cds_hash_map));

    m->key_bytes = key_bytes;
    m->val_bytes = val_bytes;
    m->vec_len = vec_len;

    m->vec = malloc(m->vec_len * sizeof(cds_list*));

    for (size_t i = 0; i < m->vec_len; i++) {
        m->vec[i] = cds_list_create(sizeof(cds_pair));
    }

    return m;
}

cds_hash_map* cds_hash_map_create_default(size_t key_bytes, size_t val_bytes) {
    return cds_hash_map_create(key_bytes, val_bytes, 25);
}
