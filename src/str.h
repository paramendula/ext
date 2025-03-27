#ifndef __STR_H_
#define __STR_H_

#include <stdlib.h>
#include <string.h>

#include "base.h"

// simple string handling matters

// doesn't distinguish between ASCII and UTF-8
// and may potentially break UTF-8 sequences

// if cap == 0, raw == NULL
// len <= cap
// raw's byte size = cap + 1 (for null byte)
typedef struct str {
    size_t len, cap;
    char *raw;
} str_t;

void str_clean(str_t *s) {
    if(!s) return;
    if(s->raw) free(s->raw);
}

void str_free(str_t *s) {
    str_clean(s);
    free(s);
}

int str_empty_into(str_t *buff, size_t cap) {
    if(!buff) return ERR_NULLP;

    char *raw = NULL;

    if(cap != 0) {
        raw = (char *)malloc(cap + 1);

        if(!raw) return ERR_NEM;
    }

    *buff = (str_t) {
        .len = 0,
        .cap = cap,
        .raw = raw,
    };

    return OK;
}

// returns NULL on any error
str_t *str_empty_alloc(size_t cap) {
    str_t *s = (str_t *)malloc(sizeof(*s));

    if(!s || !str_empty_into(s, cap)) return NULL;

    return s;
}

int str_from_ch_into(str_t *buff, char ch) {

    return OK;
}

str_t *str_from_ch_alloc(char ch) {

}

int str_from_raw_own_into(str_t *buff, char *raw, size_t len, size_t cap) {

    return OK;
}

str_t *str_from_raw_own_alloc(char *raw, size_t len, size_t cap) {

}

int str_from_raw_own_into_unsafe(str_t *buff, char *raw) {
    size_t len = strlen(raw);
    return str_from_raw_own_into(buff, raw, len, len);
}

str_t *str_from_raw_own_alloc_unsafe(char *raw) {

}

int str_from_raw_into(str_t *buff, char *raw, size_t len) {
    // copy raw and call str_from_raw_own_into
}

str_t *str_from_raw_alloc(char *raw, size_t len) {

}

int str_from_raw_into_unsafe(str_t *buff, char *raw) {
    // get raw's len (unsafe) and call str_from_raw_into
}

str_t *str_from_raw_alloc_unsafe(char *raw) {
    
}

int str_insert_char(str_t *s, size_t idx, char ch) {

    return OK;
}

int str_insert_raw(str *s, size_t idx, char *raw, size_t len) {

}

int str_insert_raw_unsafe(str *s, size_t idx, char *raw) {
    return str_insert_raw(s, idx, raw, strlen(raw));
}

int str_insert(str_t *s1, size_t idx, str_t *s2) {
    return str_insert_raw(s1, idx, s2->raw, s2->len);
}

#endif