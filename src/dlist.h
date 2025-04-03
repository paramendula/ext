#ifndef __DLIST_H_
#define __DLIST_H_

#include <stdlib.h>

#include "base.h"

typedef struct dnode {
    struct dnode *prev, *next;
} dnode;

typedef struct dlist {
    size_t count;
    dnode *first, *last;
} dlist;

// {0}'d dlist is a proper empty structure, no init funcs needed

typedef void(*dnode_free_func)(void*);

void dn_free(dnode *dn) {
    if(!dn) return;
    free(dn);
}

void dl_clean(dlist *dl, dnode_free_func ff) {
    if(!dl) return;
    if(!ff) ff = (dnode_free_func)dn_free;

    dnode *temp;

    for(dnode *cur = dl->first; cur != NULL; cur = temp) {
        temp = cur->next;
        ff(cur);
    }
}

void dl_free(dlist *dl, dnode_free_func ff) {
    dl_clean(dl, ff);
    free(dl);
}

int dl_append(dlist *dl, dnode *dn) {
    if(!dl) return ERR_NULLP;
    if(!dn) return ERR_NULLP;
    
    if(!dl->last) {
        dl->first = dl->last = dn;
    } else {
        dn->prev = dl->last;
        dl->last->next = dn;
        dl->last = dn;

        dn->next = NULL;
    }

    dl->count++;

    return OK;
}

int dl_prepend(dlist *dl, dnode *dn) {
    if(!dl) return ERR_NULLP;
    if(!dn) return ERR_NULLP;
    
    if(!dl->first) {
        dl->first = dl->last = dn;
    } else {
        dn->next = dl->first;
        dl->first->prev = dn;
        dl->first = dn;
        
        dn->prev = NULL;
    }

    dl->count++;

    return OK;
}

// puts dn after *after*
int dl_insert_after(dlist *dl, dnode *dn, dnode *after) {
    if(!dl) return ERR_NULLP;
    if(!dn) return ERR_NULLP;
    if(!after) return ERR_NULLP;

    dn->next = after->next;
    after->next = dn;
    dn->prev = after;

    dl->count++;

    return OK;
}

// puts dn before *before*
int dl_insert_before(dlist *dl, dnode *dn, dnode *before) {
    if(!dl) return ERR_NULLP;
    if(!dn) return ERR_NULLP;
    if(!before) return ERR_NULLP;

    dn->next = before;
    dn->prev = before->prev;
    before->prev = dn;

    dl->count++;

    return OK;
}

// dn is not free'd, manual management needed
int dl_remove(dlist *dl, dnode *dn) {
    if(!dl) return ERR_NULLP;
    if(!dn) return ERR_NULLP;

    dl->count--;

    if(dn->prev) {
        dn->prev->next = dn->next;
    } else {
        dl->first = dn->next;
    }

    if(dn->next) {
        dn->next->prev = dn->prev;
    } else {
        dl->last = dn->prev;
    }

    return OK;
}

#endif