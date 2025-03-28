#ifndef __STR_H_
#define __STR_H_

#include <stdlib.h>
#include <string.h>

#include "base.h"

// simple string handling matters

// doesn't distinguish between ASCII and UTF-8
// and may potentially break UTF-8 sequences

// unsafe functions versions utilize 'strlen' which may leak memory

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

inline static str_t *str_empty_alloc(size_t cap) {
    str_t *s = (str_t *)malloc(sizeof(*s));
    return str_empty_into(s, cap) ? s : NULL;
}

int str_from_ch_into(str_t *buff, char ch) {
    if(!buff) return ERR_NULLP;

    char *raw = (char *)malloc(2);

    if(!raw) return ERR_NEM;

    raw[0] = ch;
    raw[1] = 0;

    *buff = (str_t) {
        .len = 1,
        .cap = 1,
        .raw = raw,
    };

    return OK;
}

inline static str_t *str_from_ch_alloc(char ch) {
    str_t *s = (str_t *)malloc(sizeof(*s));
    return str_from_ch_into(s, ch) ? s : NULL;
}

int str_from_raw_own_into(str_t *buff, char *raw, size_t len, size_t cap) {
    if(!buff) return ERR_NULLP;

    if(raw) {
        if(cap == 0 || (len > cap)) return ERR_WARG;
    } else if(cap != 0 || len != 0) return ERR_WARG;

    raw[len] = 0;

    *buff = (str_t) {
        .len = len,
        .cap = cap,
        .raw = raw,
    };

    return OK;
}

inline static str_t *str_from_raw_own_alloc(char *raw, size_t len, size_t cap) {
    str_t *s = (str_t *)malloc(sizeof(*s));
    return str_from_raw_own_into(s, raw, len, cap) ? s : NULL;
}

inline static int str_from_raw_own_into_unsafe(str_t *buff, char *raw) {
    size_t len = strlen(raw);
    return str_from_raw_own_into(buff, raw, len, len);
}

inline static str_t *str_from_raw_own_alloc_unsafe(char *raw) {
    str_t *s = (str_t *)malloc(sizeof(*s));
    if(!s) return NULL;
    size_t l = strlen(raw);
    return str_from_raw_own_into(s, raw, l, l) ? s : NULL;
}

int str_from_raw_into(str_t *buff, char *raw, size_t len) {
    if(len == 0) return str_empty_into(buff, 0);
    if(!buff) return ERR_NULLP;
    if(!raw && len != 0) return ERR_WARG;

    char *raw_copy = (char *)malloc(len + 1);
    if(!raw_copy) return ERR_NEM;

    memcpy(raw_copy, raw, len);
    raw_copy[len] = 0;

    *buff = (str_t) {
        .len = len,
        .cap = len,
        .raw = raw_copy,
    };

    return OK;
}

inline static str_t *str_from_raw_alloc(char *raw, size_t len) {
    str_t *s = (str_t *)malloc(sizeof(*s));
    return str_from_raw_into(s, raw, len) ? s : NULL;
}

inline static int str_from_raw_into_unsafe(str_t *buff, char *raw) {
    return str_from_raw_into(buff, raw, strlen(raw));
}

inline static str_t *str_from_raw_alloc_unsafe(char *raw) {
    str_t *s = (str_t *)malloc(sizeof(*s));
    return str_from_raw_into(s, raw, strlen(raw)) ? s : NULL;
}

inline static int _str_size_check(str_t *s, size_t amount) {
    size_t nlen = s->len + amount;
    size_t cap = s->cap;
    size_t ncap = s->cap;
    
    while(nlen > ncap) ncap *= 2;

    if(cap != ncap) {
        char *nraw = (char *)realloc(s->raw, ncap + 1);
        if(!nraw) return ERR_NEM;

        s->cap = ncap;
        s->raw = nraw;
    }

    return OK;
}

int str_insert_char(str_t *s, size_t idx, char ch) {
    if(!s) return ERR_NULLP;
    if(idx > s->len) return ERR_WARG;
    if(_str_size_check(s, 1)) return ERR_NEM;

    char *raw = s->raw;
    size_t to_move = s->len - idx;

    if(to_move)
        memmove(raw + idx + 1, raw + idx, to_move);

    raw[idx] = ch;
    raw[++s->len] = 0;

    return OK;
}

int str_insert_raw(str_t *s, size_t idx, char *raw, size_t len) {
    if(!s) return ERR_NULLP;
    if(idx > s->len) return ERR_WARG;
    if(len == 0) return OK;
    if(_str_size_check(s, len)) return ERR_NEM;

    char *sraw = s->raw;
    size_t to_move = s->len - idx;

    if(to_move)
        memmove(sraw + idx + len, sraw + idx, to_move);

    memcpy(sraw + idx, raw, len);

    s->len += len;
    raw[s->len] = 0;

    return OK;
}

int str_insert_raw_unsafe(str_t *s, size_t idx, char *raw) {
    return str_insert_raw(s, idx, raw, strlen(raw));
}

int str_insert(str_t *s1, size_t idx, str_t *s2) {
    return str_insert_raw(s1, idx, s2->raw, s2->len);
}

#endif