#ifndef CDS_PAIR_H
#define CDS_PAIR_H

/***********TYPES***********/
struct cds_pair {
    void *key;
    void *value;
};

typedef struct cds_pair cds_pair;

#endif
