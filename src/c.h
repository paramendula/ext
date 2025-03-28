#ifndef __C_H_
#define __C_H_

#include <stdio.h>
#include <stdlib.h>

#include "base.h"

// Input
// C source code -> c_parse_* ->
// C tokens -> c_analyze_* ->
// C AST

// Output
// C AST -> c_ast_to_toks_* ->
// C tokens -> c_toks_to_str_* ->
// C source code

#define STATUS_OK 0
#define STATUS_ERR -1
#define STATUS_EOF -2

typedef enum {
    ctWrong = 0,
    ctPrep,
    ctId,
    ctStr,
    ctChar,
    ctInt,
    ctReal,
    ctParO,
    ctParC,
    ctSParO,
    ctSParC,
    ctCParO,
    ctCParC,
    ctLess,
    ctGreat,
    ctExclam,
    ctAmper,
    ctAster,
    ctQuest,
    ctMinus,
    ctPlus,
    ctEq,
    ctSlash,
    ctComma,
    ctDot,
    ctSemi, // opt
    ctColon,
    ctWSpace, // opt
    ctBSlash, // opt
} eC_tok;

typedef struct c_tok {
    struct c_tok *prev, *next;
    eC_tok t;
} c_tok;

typedef struct c_code {
    int tok_count;
    c_tok *tok_first, tok_last;
    // tok errors

    // AST
    // ast errors
} c_code;

typedef struct c_opts {
    char read_error_not_fatal;
    char save_wspace;
    char save_semi;
    char save_bslash;
} c_opts;

typedef struct c_vtable {
    char (*read_char)(void*);
    int (*read)(void*, char *buff, int amount);

    char (*peek_char)(void*);
    int (*peek)(void*, char *buff, int amount);

    int (*get_status)(void*);
    int (*get_error)(void*, char **const buff);
} c_vtable;

const c_opts C_OPTS_DEFAULT = (c_opts) {
    .read_error_not_fatal = 1,
    .save_wspace = 0,
    .save_semi = 0,
    .save_bslash = 0,
};

int c_parse_into(c_code *buff, c_opts *opts, c_vtable *vt, void *data) {
    if(!buff) return ERR_NULLP;
    if(!vt) return ERR_NULLP;
    if(!data) return ERR_NULLP;
    if(!opts) opts = &C_OPTS_DEFAULT;

    while (1) {
        int status = vt->get_status(data);

        if(status == STATUS_ERR) {
            if(opts->read_error_not_fatal) {

            } else {

            }
        } else if(status == STATUS_EOF) {

        }
    }

    return OK;
}

#endif