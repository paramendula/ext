#ifndef __LIST_H_
#define __LIST_H_

typedef struct item {
    struct item *prev, *next;
} item;

typedef struct dlist {
    int count;
    item *first, *last;
} dlist;

#endif