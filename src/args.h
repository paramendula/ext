#ifndef __ARG_H_
#define __ARG_H_

#include <stdlib.h>

#include "base.h"

typedef struct arg {
    struct arg *next;
    char *key, *val;
} arg;

// Simple CLI arguments parsing function
// Changes *ret and *ret_len only if (argc and argv are valid) and (OK returned)
// *ret is NULL if argc == 1
// *ret_len is 0 if argc == 1
// Returns: ERR_WARG if argc < 1
// ERR_NULLP if argv == NULL
// ERR_NEM if were unable to allocate memory for an argument
// *ret always ends with an arg_t that has key and val both set to NULL
int parse_args(int argc, const char **argv, const arg **ret, int *ret_len) {
    if(!argv) return ERR_NULLP;

    if(argc == 1) {
        if(ret) *ret = NULL;
        if(ret_len) *ret_len = 0;
        return OK;
    } else if(argc < 0) return ERR_WARG;

    if(!ret) return OK;

    arg *first = NULL;
    int len = 0;

    // (i = 1) skip executable path
    for(int i = 1; i < argc; i++) {
        const char *v = argv[i];
        
    }

    *ret = first;
    if(ret_len) *ret_len = len;

    return OK;
}

#endif