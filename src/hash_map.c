#include <cds/hash_map.h>
#include <cds/list.h>
#include <cds/pair.h>
#include <stdio.h>
#include <string.h>

cds_hash_map* cds_hash_map_create(size_t vec_len) {
    cds_hash_map *m = malloc(sizeof(cds_hash_map));

    m->vec_len = vec_len;
    m->size = 0;

    m->vec = malloc(m->vec_len * sizeof(cds_list*));

    for (size_t i = 0; i < m->vec_len; i++) {
        m->vec[i] = cds_list_create(sizeof(cds_pair));
    }

    return m;
}

cds_hash_map* cds_hash_map_create_default() {
    return cds_hash_map_create(25);
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
        // destroy all key/value allocations that the pairs point to
        for (size_t j=0; j<list->size; j++) {
            cds_pair* pair = (cds_pair*) e->data;
            free(pair->key);
            free(pair->val);
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

cds_pair* find_pair_(cds_hash_map *map,
                                 void *key,
                                 size_t key_size,
                                 unsigned int (*hash)(void *key, size_t key_size),
                                 int(*equals)(void *key_a, void *key_b,
                                              size_t key_a_size, size_t key_b_size)) {
    // compute hash
    // compute vector index
    // get list from vector and find matching pair
    // return matching pair or NULL if not found
    unsigned int hash_code = hash(key, key_size);
    size_t index = hash_code % map->vec_len;
    cds_list *list = map->vec[index];
    cds_list_entry *e = list->entry;
    for (size_t i = 0; i < list->size; i++) {
        cds_pair *pair = (cds_pair*)e->data;
        if (key_size == pair->key_size && equals(key, pair->key, key_size, pair->key_size)) {
            return pair;
        }
        e = e->next;
    }
    return NULL;
}

void rebalance_(cds_hash_map *map,
                unsigned int (*hash)(void *key, size_t key_size),
                int (*equals)(void *key_a, void *key_b,
                              size_t key_a_size, size_t key_b_size)) {
    cds_hash_map *new = cds_hash_map_create(map->size*2);

    for (size_t i = 0; i < map->vec_len; i++) {
        cds_list *list = map->vec[i];
        cds_list_entry *e = list->entry;
        for (size_t j = 0; j < list->size; j++) {
            cds_pair *p = e->data;
            cds_hash_map_insert(new,
                                p->key, p->val,
                                p->key_size, p->val_size,
                                hash, equals);
        }
    }
    cds_hash_map *old = malloc(sizeof(cds_hash_map));
    old->size = map->size;
    old->vec_len = map->vec_len;
    old->vec = map->vec;

    map->size = new->size;
    map->vec_len = new->vec_len;
    map->vec = new->vec;
    free(new);
    cds_hash_map_destroy(&old);

}

void cds_hash_map_insert(cds_hash_map *map,
                    void *key,
                    void *val,
                    size_t key_size,
                    size_t val_size,
                    unsigned int (*hash)(void *key, size_t key_size),
                    int(*equals)(void *key_a, void *key_b,
                                 size_t key_a_size, size_t key_b_size)) {
    cds_pair *pair = find_pair_(map, key, key_size, hash, equals);
    if (pair) {
        if (val_size != pair->val_size) {
            free(pair->val);
            pair->val = malloc(val_size);
        }
        memcpy(pair->val, val, val_size);
    } else {
        // compute hash
        unsigned int hash_code = hash(key, key_size);
        // compute index
        size_t index = hash_code % map->vec_len;
        // create pair
        cds_pair pair;
        // allocate space for key and value
        pair.key = malloc(key_size);
        pair.val = malloc(val_size);
        // assign key and value
        memcpy(pair.key, key, key_size);
        memcpy(pair.val, val, val_size);
        pair.key_size = key_size;
        pair.val_size = val_size;
        // insert pair into list
        cds_list_insert(map->vec[index], &pair);
        map->size+=1;
        if (map->size > 0.75*map->vec_len) {
            rebalance_(map, hash, equals);
        }
    }
}

void* cds_hash_map_get(cds_hash_map *map,
                 void *key,
                 size_t key_size,
                 unsigned int (*hash)(void *key, size_t key_size),
                 int(*equals)(void *key_a, void *key_b,
                              size_t key_a_size, size_t key_b_size)) {

    cds_pair *pair = find_pair_(map, key, key_size, hash, equals);

    if (!pair) {
        printf("hash_map get returning NULL\n");
        return NULL;
    }

    return pair->val;
}

int list_equals_(void *a, void *b) {
    return a == b;
}

void cds_hash_map_remove(cds_hash_map *map,
                 void *key,
                 size_t key_size,
                 unsigned int (*hash)(void *key, size_t key_size),
                 int(*equals)(void *key_a, void *key_b,
                              size_t key_a_size, size_t key_b_size)) {

    cds_pair *pair = find_pair_(map, key, key_size, hash, equals);
    if (!pair) {
        printf("hash-map remove didn't find object\n");
        return;
    }
    free(pair->key);
    free(pair->val);
    unsigned int hash_code = hash(key, key_size);
    size_t index = hash_code % map->vec_len;
    cds_list *list = map->vec[index];
    cds_list_remove(map->vec[index], pair, list_equals_);
    map->size--;
}

cds_list* cds_hash_map_keys(cds_hash_map *map) {
    cds_list *keys = cds_list_create(sizeof(void*));
    for (size_t i = 0; i < map->vec_len; i++) {
        cds_list *list = map->vec[i];
        cds_list_entry *e = list->entry;
        for (size_t j = 0; j < list->size; j++) {
            cds_pair *p = (cds_pair*)e->data;
            cds_list_insert(keys, p->key);
            e = e->next;
        }
    }
    return keys;
}

void cds_hash_map_print(cds_hash_map *map, void(*print_key)(void*), void(*print_val)(void*)) {
    printf("HASHMAP\n");
    for (size_t i = 0; i < map->vec_len; i++) {
        cds_list *list = map->vec[i];
        cds_list_entry *e = list->entry;
        printf("vector %d: ", i);
        for (size_t j = 0; j < list->size; j++) {
            cds_pair *p = (cds_pair*)e->data;
            print_key(p->key);
            printf(":");
            print_val(p->val);
            printf(", ");
            e = e->next;
        }
        printf("\n");
    }
}
