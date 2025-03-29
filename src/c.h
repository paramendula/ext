#ifndef __C_H_
#define __C_H_

#include <stdio.h>
#include <stdlib.h>

#include "base.h"
#include "str.h"
#include "dlist.h"

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
    ctBar,
    ctComma,
    ctDot,
    ctSemi, // opt
    ctColon,
    ctWSpace, // opt
    ctBSlash, // opt
    // Compound
    ctShiftRight,
    ctShiftLeft,
    ctAnd,
    ctOr,
    ctEqLong,
    ctEqNot,
    ctEqPlus,
    ctEqMinus,
    ctEqMul,
    ctEqDiv,
    ctEqMod,
    ctEqShiftRight,
    ctEqShiftLeft,
    ctEqXor,
    ctEqOr,
    ctEqAnd,
    ctEqLess,
    ctEqGreat,
    ctDoublePlus,
    ctDoubleMinus,
    ctPtrMem, // ->
} eC_tok;

typedef struct c_tok {
    struct c_tok *prev, *next;
    eC_tok t;
    char *raw; // id, str, char, int, real, wspace
} c_tok;

typedef enum eC_ast {
    caWrong = 0,
    caPreIf,
    caPreIfdef,
    caPreIfndef,
    caPreElse,
    caPreElif,
    caPreEndif,
    caPreError,
    caPrePragma,
    caPreDefine,
    caPreUndef,
    caPreInclude,
    caPreLine,
    // expression
    caBasic, // token holder for basic expressions (all literals + identifiers)
    caInfix, // + - * / % >> << & | ^ && || = == != += -= *= /= %= >>= <<= &= |= ^= . ->
             // > < <= >=
    caPrefix, // - + * ++ -- ! ~ &
    caPostfix, // ++ --
    caSub, // a[n]
    caComma, // a, b, c
    caTypeCast, // (type)a
    caSizeof, // sizeof a
    caCond, // a ? b : c
    caCall, // <id>(...)
    // statement
    caTypedef,
    caLabel,
    caBlock,
    caIf,
    caSwitch,
    caWhile,
    caFor,
    caDo,
    caBreak,
    caContinue,
    caReturn,
    caGoto,
    caDeclaration,
} eC_ast;

// always check for e != caWrong beforehand
#define CAST_IS_PRE(e) ((e) <= caPreLine)
#define CAST_IS_EXPR(e) (((e) > caPreLine) && ((e) <= caCall))
#define CAST_IS_STAT(e) ((e) > caCall)

typedef struct c_ast {
    struct c_ast *prev, *next;
    eC_ast t;
} c_ast;

typedef struct c_code {
    int tok_count;
    c_tok *tok_first, tok_last;
    // TODO: tok errors

    int ast_count;
    c_ast *ast_first, ast_last;
    // TODO: ast errors
} c_code;

// c is a ptr
#define C_TOKLIST(c) ((dlist*)&((c)->tok_count))
#define C_ASTLIST(c) ((dlist*)&((c)->ast_count))

typedef struct c_opts {
    // tok
    char read_error_not_fatal;
    char save_wspace;
    char save_semi;
    char save_bslash;
    // ast
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

void c_tok_clean(c_tok *t) {
    if(!t) return;
    // TODO
}

inline static c_tok_free(c_tok *t) {
    c_tok_clean(t);
    free(t);
}

void c_ast_clean(c_ast *a) {
    if(!a) return;
    // TODO
}

void c_ast_free(c_ast *a) {
    c_ast_clean(a);
    free(a);
}

void c_code_clean(c_code *c) {
    if(!c) return;

    // TODO (use dlist generic)
}

inline static void c_code_free(c_code *c) {
    c_code_clean(c);
    free(c);
}

// C source code -> C tokens
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