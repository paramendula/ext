#ifndef __DLIST_H_
#define __DLIST_H_

typedef struct dnode {
    struct dnode *prev, *next;
} dnode;

typedef struct dlist {
    int count;
    dnode *first, *last;
} dlist;

#endif