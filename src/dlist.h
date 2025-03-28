#ifndef __LIST_H_
#define __LIST_H_

typedef struct dlist {
    int count;
    struct dlist *prev, *next;
} dlist;

#endif