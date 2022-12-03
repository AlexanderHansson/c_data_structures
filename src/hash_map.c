#include <cds/hash_map.h>
#include <cds/list.h>
#include <cds/pair.h>
#include <stdio.h>
#include <string.h>

cds_hash_map* cds_hash_map_create(size_t key_bytes, size_t val_bytes, size_t vec_len) {
    cds_hash_map *m = malloc(sizeof(cds_hash_map));

    m->key_bytes = key_bytes;
    m->val_bytes = val_bytes;
    m->vec_len = vec_len;
    m->size = 0;

    m->vec = malloc(m->vec_len * sizeof(cds_list*));

    for (size_t i = 0; i < m->vec_len; i++) {
        m->vec[i] = cds_list_create(sizeof(cds_pair));
    }

    return m;
}

cds_hash_map* cds_hash_map_create_default(size_t key_bytes, size_t val_bytes) {
    return cds_hash_map_create(key_bytes, val_bytes, 25);
}


void cds_hash_map_destroy(cds_hash_map **map) {
    // for each list destroy the data the pairs are using?
    // destroy the list
    // destroy the maps vector
    // destroy the hash_map
    cds_hash_map *m = *map;
    // loop over all lists
    for (size_t i = 0; i < m->vec_len; i++) {
        cds_list *list = m->vec[i];
        cds_list_entry *e = list->entry;
        printf("clearing list with size: %d\n", list->size);
        // destroy all key/value allocations that the pairs point to
        for (size_t j=0; j<list->size; j++) {
            cds_pair* pair = (cds_pair*) e->data;
            free(pair->key);
            free(pair->value);
            e = e->next;
        }
        // all k/v's are destroyed, now destroy the list
        cds_list_destroy(&list);
    }
    // destroy the vector
    free((*map)->vec);
    // destroy the map struct
    free(*map);
    *map = NULL;
}

cds_pair* cds_hash_map_find_pair(cds_hash_map *map,
                                 void *key,
                                 unsigned int (*hash)(void *key),
                                 int(*equals)(void *key_a, void *key_b)) {
    // compute hash
    // compute vector index
    // get list from vector and find matching pair
    // return matching pair or NULL if not found
    unsigned int hash_code = hash(key);
    size_t index = hash_code % map->vec_len;
    cds_list *list = map->vec[index];
    cds_list_entry *e = list->entry;
    for (size_t i = 0; i < list->size; i++) {
        cds_pair *pair = (cds_pair*)e->data;
        if (equals(key, pair->key)) {
            return pair;
        }
    }
    return NULL;
}

void cds_hash_map_insert(cds_hash_map *map,
                    void *key,
                    void *value,
                    unsigned int (*hash)(void *key),
                    int(*equals)(void *key_a, void *key_b)) {
    cds_pair *pair = cds_hash_map_find_pair(map, key, hash, equals);
    if (pair) {
        memcpy(pair->value, value, map->val_bytes);
    } else {
        // compute hash
        unsigned int hash_code = hash(key);
        // compute index
        size_t index = hash_code % map->vec_len;
        // create pair
        cds_pair pair;
        // allocate space for key and value
        pair.key = malloc(map->key_bytes);
        pair.value = malloc(map->val_bytes);
        // assign key and value
        memcpy(pair.key, key, map->key_bytes);
        memcpy(pair.value, value, map->val_bytes);
        // insert pair into list
        cds_list_insert(map->vec[index], &pair);
        map->size+=1;
    }
}

void* cds_hash_map_get(cds_hash_map *map,
                 void *key,
                 unsigned int (*hash)(void *key),
                 int(*equals)(void *key_a, void *key_b)) {
    cds_pair *pair = cds_hash_map_find_pair(map, key, hash, equals);
    if (!pair) {
        return NULL;
    }
    return pair->value;
}
