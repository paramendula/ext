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
    eC_tok t;
    char *raw; // id, str, char, int, real, wspace
} c_tok;

typedef struct c_tok_node {
    struct c_tok_node *prev, *next;
    union {
        struct {
            eC_tok t;
            char * raw;
        };
        c_tok tok;
    };
} c_tok_node;

typedef struct c_tok_list {
    int count;
    c_tok_node *first, *last;
} c_tok_list;

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
    caBasic, // token holder for basic expressions (all simple literals + identifiers)
    caInfix, // + - * / % >> << & | ^ && || = == != += -= *= /= %= >>= <<= &= |= ^= . ->
             // > < <= >=
    caPrefix, // - + * ++ -- ! ~ &
    caPostfix, // ++ --
    caSub, // a[n]
    caComma, // a, b, c
    caTypeCast, // (<id>)a
    caSizeof, // sizeof a
    caCond, // a ? b : c
    caInit, // { ... }
    caCall, // <id>(...)
    // statement
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

typedef struct c_ast {
    eC_ast t;
    void *data;
} c_ast;

typedef struct c_ast_node {
    struct c_ast_node *prev, *next;

    union {
        struct {
            eC_ast t;
            void *data;
        };
        c_ast ast;
    };
} c_ast_node;

typedef struct c_ast_list {
    int count;
    c_ast_node *first, *last;
} c_ast_list;

// PREPROCESSOR


// EXPRESSIONS

// caBasic: c_tok
// caSizeof: c_ast

typedef struct c_infix {
    eC_tok op;
    struct c_ast left, right;
} c_infix;

typedef struct c_prefix {
    eC_tok op;
    struct c_ast right;
} c_prefix;

typedef struct c_postfix {
    eC_tok op;
    struct c_ast left;
} c_postfix;

typedef struct c_sub {
    struct c_ast arr, idx;
} c_sub;

typedef struct c_typecast {
    char *id;
    struct c_ast right;
} c_typecast;

typedef struct c_cond {
    struct c_ast cond, b1, b2;
} c_cond;

typedef struct cini_memspec {
    struct cini_memspec *next;
    char is_id; // is .<id>? if not, it's [<expr>]
    union {
        char *id;
        c_ast expr;
    };
} cini_memspec;

typedef struct cini_member {
    struct cini_member *next;
    struct cini_memspec *spec;
    c_ast val;
} cini_member;

typedef struct c_init {
    int count;
    cini_member *first;
} c_init;

typedef struct c_call {
    char *id;
    struct c_ast *args;
} c_call;

// STATEMENTS

// caBlock: c_ast_list
// caBreak: NULL
// caContinue: NULL
// caReturn: c_ast
// caGoto: char*

typedef enum eC_label {
    clabWrong,
    clabTarget,
    clabCase,
    clabDefault,
} eC_label;

typedef struct c_label {
    eC_label t;
    union {
        char *id; // clabTarget
        c_tok cc; // clabCase
    };
} c_label;

typedef struct c_if {
    c_ast cond;
    struct c_if *e; // else
} c_if;

typedef struct c_switch {
    c_ast cond;
    c_ast_list block;
} c_switch;

typedef struct c_while {
    c_ast cond;
    c_ast_list block;
} c_while;

typedef struct c_for {
    c_ast init, cond, step;
    c_ast_list block;
} c_for;

typedef struct c_do {
    c_ast cond;
    c_ast_list block;
} c_do;

typedef enum eC_declaration {
    cdecWrong,
    cdecTypedef,
    cdecOther,
} eC_declaration;

typedef enum eCdec_type {
    cdtWrong,
    cdtId,
    cdtStruct,
    cdtEnum,
    cdtUnion,
} eCdec_type;

typedef struct cdec_decr {
    int pass;
} cdec_decr;

typedef struct cdec_type {
    eCdec_type t;
    union {

    };
} cdec_type;

typedef struct cdec_typedef {
    cdec_type type;
    char *id;
} cdec_typedef;

typedef struct c_declaration {
    eC_declaration t;
    union {
        cdec_typedef tdef;
    };
} c_declaration;

// always check for e != caWrong beforehand
#define CAST_IS_PRE(e) ((e) <= caPreLine)
#define CAST_IS_EXPR(e) (((e) > caPreLine) && ((e) <= caCall))
#define CAST_IS_STAT(e) ((e) > caCall)

typedef struct c_tok_parse {
    c_tok_list l;
} c_tok_parse;

// c is a ptr
#define C_TOKLIST(c) ((dlist*)&((c)->tok_count))
#define C_ASTLIST(c) ((dlist*)&((c)->ast_count))

typedef struct c_tok_opts {
    char read_error_not_fatal;
    char save_wspace;
    char save_semi;
    char save_bslash;
} c_tok_opts;

typedef struct c_vtable {
    char (*read_char)(void*);
    int (*read)(void*, char *buff, int amount);

    char (*peek_char)(void*);
    int (*peek)(void*, char *buff, int amount);

    int (*get_status)(void*);
    int (*get_error)(void*, char **const buff);
} c_vtable;

const c_tok_opts C_TOK_OPTS_DEFAULT = (c_tok_opts) {
    .read_error_not_fatal = 1,
    .save_wspace = 0,
    .save_semi = 0,
    .save_bslash = 0,
};

void c_tok_clean(c_tok *t) {
    if(!t) return;
    // TODO
}

inline static void c_tok_free(c_tok *t) {
    c_tok_clean(t);
    free(t);
}

void c_ast_clean(c_ast *a) {
    if(!a) return;
    // TODO
}

inline static void c_ast_free(c_ast *a) {
    c_ast_clean(a);
    free(a);
}

void c_tok_parse_clean(c_tok_parse *c) {
    if(!c) return;
    // TODO 
}

inline static void c_tok_parse_free(c_tok_parse *c) {
    c_code_clean(c);
    free(c);
}

// C source code -> C tokens
int c_parse_into(c_tok_parse *buff, const c_tok_opts *opts, c_vtable *vt, void *data) {
    if(!buff) return ERR_NULLP;
    if(!vt) return ERR_NULLP;
    if(!data) return ERR_NULLP;
    if(!opts) opts = &C_TOK_OPTS_DEFAULT;

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