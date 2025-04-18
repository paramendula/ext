#ifndef __C_H_
#define __C_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "base.h"
#include "str.h"
#include "dlist.h"
#include "list.h"
#include "htable.h"

// Input
// C source code -> c_parse_* ->
// C tokens -> c_analyze_* ->
// C AST

// Output
// C AST -> c_deanalyze_* ->
// C tokens -> c_deparse_* ->
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
    ctPerc,
    ctCaret,
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
    ctDoubleEq,
    ctDoublePlus,
    ctDoubleMinus,
    ctPtrMem, // ->
    ctCommSingle, // opt
    ctCommMulti, // opt
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
    size_t count;
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
    // caPreWarning,
    caPreDefine,
    caPreUndef,
    caPreInclude,
    // caPreDefined, // defined <id>
    // caPreId, // for # and ##
    caPreCall,
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
    size_t count;
    c_ast_node *first, *last;
} c_ast_list;

// PREPROCESSOR
// TODO: separate PP from C AST

// caPreIf: c_ast* (expression)
// caPreElif: c_ast* (expression)
// caPreIfdef: char* (id)
// caPreIfndef: char* (id)
// caPreElse: NULL
// caPreEndif: NULL
// caPreUndef: char* (id)
// caPreId: char* (special id with '#' or '##')
// caPreError: char* 
// caPrePragma: char*
// caPreCall: c_tok_list*

typedef struct c_pre_define_param {
    struct c_pre_define_param *next;
    char is_varg;
    char *id;
} c_pre_define_param;

typedef struct c_pre_define {
    char *id;
    c_pre_define_param *first; // != NULL if a func
    char *rep; // raw replacement list
} c_pre_define;

typedef enum eC_pre_include {
    cpiWrong = 0,
    cpiHString,
    cpiQString,
    cpiPPTokens,
} eC_pre_include;

typedef struct c_pre_include {
    eC_pre_include t;
    union {
        char *str;
        c_ast *pp;
    };
} c_pre_include;

typedef struct c_pre_line {
    int lineno;
    c_ast *filename;
} c_pre_line;

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
    size_t count;
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
    clabWrong = 0,
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

typedef enum eCdec_type {
    cdtWrong = 0,
    cdtId,
    cdtStruct,
    cdtEnum,
    cdtUnion,
} eCdec_type;

// int (*(*func)(double))[3] = NULL;
// { id: func
// parts: ptr -> function (double) :returns: -> ptr -> array (3)
// bits: 0 (for struct members only) }
// func is a pointer to a function that accepts one double argument
// and returns a pointer to an array of 3 integers

// also used for function parameters in function signature declaration/definiton
struct cdef_struct_mem;

typedef enum eCdec_decr_part {
    cdpWrong = 0,
    cdpPtr,
    cdpArray,
    cdpFunction,
} eCdec_decr_part;

typedef struct cdec_decr_part {
    struct cdec_decr_part *next;
    eCdec_decr_part t;
    union {
        struct cdef_struct_mem *args; // for func
        struct {
            cs_list *qfs;
            c_ast *len;
        } arr;
    }; // 2 * sizeof(size_t)
} cdec_decr_part;

typedef struct cdec_decr {
    char *id;
    size_t count;
    cdec_decr_part *first;
    int bits; // for struct
} cdec_decr;

typedef struct cdec_type {
    eCdec_type t;
    void *data;
    cs_list qfs;
} cdec_type;

// decr != NULL, except for function declaration
typedef struct cdec_struct_mem {
    struct cdec_struct_mem *next;
    cdec_type type;
    cdec_decr *decr;
} cdec_struct_mem;

typedef struct cdec_struct {
    char *id;
    size_t count;
    struct cdec_struct_mem *first;
} cdec_struct;

typedef struct cdec_union {
    char *id;
    size_t count;
    struct cdec_struct_mem *first;
} cdec_union;

typedef struct cdec_enum_mem {
    struct cdec_enum_mem *next;
    char *id;
    c_ast *val; // = <const> or = <const expr>
} cdec_enum_mem;

typedef struct cdec_enum {
    char *id, *type;
    struct cdec_enum_mem *first;
} cdec_enum;

typedef struct cdec_pair {
    cdec_decr decr;
    c_ast init;
} cdec_pair;

// pseudo schema:
// <is_typedef>?typedef <cdec_type> <cdec_pair='<cdec_decr>=<c_ast>'>+;

typedef struct c_declaration {
    char is_typedef;
    cdec_type type;
} c_declaration;

// always check for e != caWrong beforehand
#define CAST_IS_PRE(e) ((e) <= caPreLine)
#define CAST_IS_EXPR(e) (((e) > caPreLine) && ((e) <= caCall))
#define CAST_IS_STAT(e) ((e) > caCall)

typedef struct c_tok_parse {
    c_tok_list l;
    // errors list
} c_tok_parse;

typedef struct c_tok_opts {
    char read_error_not_fatal;
    char save_wspace;
    char save_semi;
    char save_bslash;
    char save_comm;
} c_tok_opts;

// 1 <= amount <= 256
typedef struct c_vtable {
    char (*read_char)(void*);
    int (*read)(void*, char *buff, int amount);

    char (*peek_char)(void*);
    int (*peek)(void*, char *buff, int amount);

    int (*seek)(void*, int amount, int flag);
    int (*tell)(void*);

    int (*get_status)(void*);
    int (*get_error)(void*, const char **buff);
} c_vtable;

const c_tok_opts C_TOK_OPTS_DEFAULT = (c_tok_opts) {
    .read_error_not_fatal = 1,
    .save_wspace = 0,
    .save_semi = 0,
    .save_bslash = 0,
    .save_comm = 0,
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

// Simple tokens
// ( ) [ ] { }
// ? , . : ;

// Complex Tokens
// prep: #...$
// id: <_, alnum>+; can't start with a digit
// str: "..."
// char: '...'
// int: <num>+ || 0<X,x><hexnum>+
// real: <int>.<int><<E,e><int>>? || 0<X,x><hexint>.<hexint><<P,p><hexint>>?

// Semi-complex tokens
// <: <, <<, <<=
// >: >, >>, >>=
// !: !, !=
// &: &, &=, &&
// *: *, *=
// -: -, -=, --, ->
// +: +, +=, ++
// =: =, ==
// /: /, /=, //, /*
// |: |, |=, ||
// \ : \\n
// %: %, %=
// ^: ^, ^=

// Manual:
// Optionals
// Complex Tokens
// // /* (Comments, passed from HTable)



// C source code -> C tokens
int c_parse_into(c_tok_parse *buff, const c_tok_opts *opts, c_vtable *vt, void *data) {
    if(!buff) return ERR_NULLP;
    if(!vt) return ERR_NULLP;
    if(!data) return ERR_NULLP;
    if(!opts) opts = &C_TOK_OPTS_DEFAULT;

    char ch = vt->read_char(data);

    eC_tok cur_t = ctWrong;

    while (1) {
        int status = vt->get_status(data);

        if(status == STATUS_ERR) {
            if(opts->read_error_not_fatal) {
                // TODO
            } else {
                // TODO
            }
        }

        if(isspace(ch)) {
            if(opts->save_wspace) {
                // TODO
            }
        }
        


        if(status == STATUS_EOF) {
            // TODO
        } else ch = vt->read_char(data);
    }

    return OK;
}

#endif