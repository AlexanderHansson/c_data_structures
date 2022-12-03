#include <cds/hash_map.h>
#include <stdio.h>

unsigned int hash(void *key, size_t key_size) {
    return *(int*)key;
}

int equals(void *key_a, void *key_b, size_t key_a_size, size_t key_b_size) {
    int a = (*(int*)key_a);
    int b = (*(int*)key_b);
    int res = ((*(int*)key_a) == (*(int*)key_b));
    printf("equal check: %d ==%d: %d\n", a, b, res);
    return res;
}

void test_insert_get() {
    printf("creating map\n");
    int x = 10;
    char c = 'W';
    cds_hash_map *m = cds_hash_map_create(10);

    cds_hash_map_insert(m, &x, &c, sizeof(x), sizeof(c), hash, equals);

    printf("size: %d\n", m->size);
    printf("inserted 10:w\n");
    printf("size: %d\n", m->size);

    char y = *(char*)cds_hash_map_get(m, &x, sizeof(x), hash, equals);
    printf("get x: %c\n", y);

    printf("destroying map\n");
    cds_hash_map_destroy(&m);
}
void test_insert_get_str() {
    printf("creating map\n");

    int x = 14;
    char *str = "hello";
    char *str2 = "there";
    char *str3 = "if you please";

    printf("%s, %d\n", str, sizeof(str));

    cds_hash_map *m = cds_hash_map_create(10);
    printf("size: %d\n", m->size);

    cds_hash_map_insert(m, &x, &str, sizeof(x), sizeof(str), hash, equals);
    x=3;
    cds_hash_map_insert(m, &x, &str2, sizeof(x), sizeof(str2), hash, equals);
    x=7;
    cds_hash_map_insert(m, &x, &str3, sizeof(x), sizeof(str3), hash, equals);

    printf("size: %d\n", m->size);

    x=14;
    printf("attempting to get\n");
    char *res = *(char**)cds_hash_map_get(m, &x, sizeof(x), hash, equals);
    printf("get %d: %s\n", x, res);

    printf("remove %d - size before:%d\n", x, m->size);
    x = 3;
    cds_hash_map_remove(m, &x, sizeof(x), hash, equals);
    printf("remove %d - size after:%d\n", x, m->size);

    printf("destroying map\n");
    cds_hash_map_destroy(&m);
}

int main() {
    test_insert_get();
    test_insert_get_str();
}
