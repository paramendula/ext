#ifndef __HTABLE_H_
#define __HTABLE_H_

#include <stdlib.h>

#include "base.h"
#include "dlist.h"

// djb2
size_t ht_hash_func(const char *key) {
    size_t hash = 5381;

    char ch;
    while (ch = *key++) {
        hash = ((hash << 5) + hash) + ch; // hash * 33 + c
    }

    return hash;
}

typedef struct ht_bucket {
    struct ht_bucket *prev, *next;
    size_t hash;
    char *key;
} ht_bucket;

typedef struct htable {
    size_t cap;
    ht_bucket **buckets;
    union {
        struct {
            size_t len;
            ht_bucket *first, *last;
        };
        dlist dl;
    };
} htable;

int ht_empty_into(htable *buff, size_t cap) {
    if(!buff) return ERR_NULLP;
    
    ht_bucket **buckets = NULL;

    if(cap > 0) {
        buckets = (ht_bucket **)calloc(cap, sizeof(*buckets));

        if(!buckets) return ERR_NEM;
    }

    *buff = (htable) {
        .len = 0,
        .first = NULL,
        .last = NULL,
        .cap = cap,
        .buckets = buckets,
    };

    return OK;
}

inline static int ht_empty_alloc(size_t cap) {
    htable *ht = (htable *)malloc(sizeof(*ht));
    return ht_empty_into(ht, cap);
}

typedef void(*htb_free_func)(void*);

// default htable free function
void htb_free(ht_bucket *htb) {
    if(!htb) return;
    free(htb->key);
    free(htb);
}

void ht_clean(htable *ht, htb_free_func ff) {
    if(!ht) return;
    if(!ff) ff = (htb_free_func)htb_free;

    dlist_clean(&ht->dl, ff);

    free(ht->buckets);
}

void ht_free(htable *ht, htb_free_func ff) {
    ht_clean(ht, ff);
    free(ht);
}

#endif