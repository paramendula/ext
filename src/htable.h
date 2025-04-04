#ifndef __HTABLE_H_
#define __HTABLE_H_

#include <stdlib.h>
#include <string.h>

#include "base.h"
#include "dlist.h"

// htable; len / cap
// if reaches it, then 
const double HT_RATIO = 0.75;

// multipler of htable's cap
// when HT_RATIO is exceeded
const size_t HT_GROWTH = 2;

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

// doesn't check for ht != NULL
// doesn't check for htb != NULL
// doesn't check for htb->key and htb->hash
// doesn't check for ht->len == ht->cap (infinte loop possible)
// doesn't refit
void _ht_insert_raw(htable *ht, ht_bucket *htb) {
    size_t idx = htb->hash % ht->cap;
    size_t lim = ht->cap;
    ht_bucket **buckets = ht->buckets;

    // search for an empty spot
    while (1) {
        if(buckets[idx]) {
            idx++;
            if(idx == lim) idx = 0;
            continue;
        }
        break;
    }

    buckets[idx] = htb;

    dl_append((dlist *)&ht->len, (dnode *)htb);
}

// doesn't check for ht != NULL
// doesn't check for ht->cap > new_cap
// dangerous
int _ht_refit(htable *ht, size_t new_cap) {
    ht_bucket **buckets = (ht_bucket **)calloc(new_cap, sizeof(*buckets));

    if(!buckets) return ERR_NEM;

    for(ht_bucket *htb = ht->first; htb != NULL; htb = htb->next) {
        size_t idx = htb->hash % new_cap;

        while(1) {
            if(buckets[idx]) {
                idx++;
                if(idx == new_cap) idx = 0;
                continue;
            }
            break;
        }

        buckets[idx] = htb;
    }

    free(ht->buckets);
    ht->buckets = buckets;
    ht->cap = new_cap;

    return OK;
}

// assumes that
// htb->key != NULL
// htb->hash is calculated from htb->key
// and overrides htb->prev, htb->next
// DOESN'T CHECK IF *htb's KEY IS ALREADY TAKEN
int ht_insert(htable *ht, ht_bucket *htb) {
    if(!ht) return ERR_NULLP;
    if(!htb) return ERR_NULLP;

    if(ht->cap == 0 || ((((double) (ht->len + 1)) / ht->cap) > HT_RATIO)) {
        // if ht->cap == 0, grow to 4
        int result = _ht_refit(ht, (ht->cap) ? (ht->cap * HT_GROWTH) : 4);
        if(result) return result;
    }

    _ht_insert_raw(ht, htb);

    return OK;
}

// If htb->key == NULL, *key is copied.
// If so, then it's unsafe, because of strlen(key);
// Automatically calculates the hash of htb->key,
// then calls ht_insert
int ht_insert_auto(htable *ht, ht_bucket *htb, const char *key) {
    if(!ht) return ERR_NULLP;
    if(!htb) return ERR_NULLP;

    char *ckey = NULL;

    if(!htb->key) {
        if(!key) return ERR_WARG;

        size_t key_len = strlen(key); // unsafe

        if(key_len == 0) return ERR_WARG;

        ckey = (char *)malloc(key_len + 1);

        if(!ckey) return ERR_NEM;

        strcpy(ckey, key);

        htb->key = ckey;
    }

    htb->hash = ht_hash_func(htb->key);

    int result = ht_insert(ht, htb);

    if(result) { // if ERR
        if(ckey) { // if the key was copied
            // revert the changes
            htb->key = NULL;
            free(ckey);
        }
    }

    return result;
}

// no error propagation
ht_bucket* ht_get(htable *ht, char *key) {
    if(!ht) return NULL;
    if(!key) return NULL;

    size_t start = ht_hash_func(key) % ht->cap;
    size_t idx = start;
    size_t lim = ht->cap;

    ht_bucket **buckets = ht->buckets;

    ht_bucket *htb;

    while (1) {
        htb = buckets[idx];

        idx++;

        if(idx == start) {
            // not found
            return NULL;
        } else if(idx == lim) {
            idx = 0;
        }

        if(!htb) {
            continue;
        }

        if(!strcmp(htb->key, key)) break;
    }

    return htb;
}

// for O(1) we can add current index to ht_bucket
// doesn't free *htb
int ht_remove(htable *ht, ht_bucket *htb) {
    if(!ht) return ERR_NULLP;
    if(!htb) return ERR_NULLP;

    size_t start = htb->hash % ht->cap;
    size_t idx = start;
    size_t lim = ht->cap;

    ht_bucket **buckets = ht->buckets;

    char found = 0;

    while (1) {
        ht_bucket *attempt = buckets[idx];

        if(!attempt) {
            goto skip;
        }

        if(attempt == htb) {
            found = 1;
            break;
        };

        skip:
        idx++;

        if(idx == start) {
            break;
        } else if(idx == lim) {
            idx = 0;
        }
    }

    if(!found) return ERR_OTHER; // strange

    buckets[idx] = NULL;
    dl_remove((dlist *)&ht->len, (dnode *)htb);

    return OK;
}

#endif