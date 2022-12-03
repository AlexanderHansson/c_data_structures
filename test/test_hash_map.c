#include <cds/hash_map.h>
#include <stdio.h>

unsigned int hash(void *key) {
    return *(int*)key;
}

int equals(void *key_a, void *key_b) {
    return *(int*)key_a == *(int*)key_b;
}

void test_insert_get() {
    printf("creating map\n");
    int x = 10;
    char c = 'W';
    cds_hash_map *m = cds_hash_map_create(sizeof(int), sizeof(char), 10);

    cds_hash_map_insert(m, &x, &c, hash, equals);

    printf("size: %d\n", m->size);
    printf("inserted 10:w\n");
    printf("size: %d\n", m->size);

    char y = *(char*)cds_hash_map_get(m, &x, hash, equals);
    printf("get x: %c\n", y);

    printf("destroying map\n");
    cds_hash_map_destroy(&m);
}

int main() {
    test_insert_get();
}
