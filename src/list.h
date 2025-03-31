#ifndef __LIST_H_
#define __LIST_H_

typedef struct node {
    struct node *next;
} node;

typedef struct list {
    int count;
    node *first;
} list;

typedef struct cs_node {
    struct cs_node *next;
    char *val;
} cs_node;

typedef struct cs_list {
    int count;
    cs_node *first;
} cs_list;

#endif