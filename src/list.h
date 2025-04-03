#ifndef __LIST_H_
#define __LIST_H_

#include <stdlib.h>

typedef struct node {
    struct node *next;
} node;

typedef struct list {
    size_t count;
    node *first;
} list;

typedef struct cs_node {
    struct cs_node *next;
    char *val;
} cs_node;

typedef struct cs_list {
    size_t count;
    cs_node *first;
} cs_list;

#endif