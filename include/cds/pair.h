#ifndef CDS_PAIR_H
#define CDS_PAIR_H

/***********TYPES***********/
struct cds_pair {
    size_t key_size;
    size_t val_size;
    void *key;
    void *val;
};

typedef struct cds_pair cds_pair;

#endif
