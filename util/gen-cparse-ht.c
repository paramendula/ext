#include <stdio.h>
#include "../src/c.h"

struct {
    const char *key;
    eC_tok t;
    eC_tok conts[4];
} items[] = {
    {
        "(",
        ctParO,
        { 0 },
    },
    {
        ")",
        ctParC,
        { 0 },
    },
    {
        "[",
        ctSParO,
        { 0 },
    },
    {
        "]",
        ctSParC,
        { 0 },
    },
    {
        "{",
        ctCParO,
        { 0 },
    },
    {
        "}",
        ctCParC,
        { 0 },
    },
    {
        "?",
        ctQuest,
        { 0 },
    },
    {
        ",",
        ctComma,
        { 0 },
    },
    {
        ".",
        ctDot,
        { 0 },
    },
    {
        ":",
        ctColon,
        { 0 },
    },
    {
        "<",
        ctLess,
        { ctShiftLeft, 0 },
    },
    {
        "<<",
        ctShiftLeft,
        { ctEqShiftLeft, 0 },
    },
    {
        "<<=",
        ctEqShiftLeft,
        { 0 },
    },
    {
        ">",
        ctGreat,
        { ctShiftLeft, 0 },
    },
    {
        ">>",
        ctShiftRight,
        { ctEqShiftRight, 0 },
    },
    {
        ">>=",
        ctEqShiftRight,
        { 0 },
    },
    {
        "!",
        ctExclam,
        { ctEqNot, 0 },
    },
    {
        "!=",
        ctEqNot,
        { 0 },
    },
    {
        "&",
        ctAmper,
        { ctEqAnd, ctAnd, 0 },
    },
    {
        "&=",
        ctEqAnd,
        { 0 },
    },
    {
        "&&",
        ctAnd,
        { 0 },
    },
    {
        "*",
        ctAster,
        { ctEqMul, 0 },
    },
    {
        "*=",
        ctEqMul,
        { 0 },
    },
    {
        "-",
        ctMinus,
        { ctEqMinus, ctDoubleMinus, ctPtrMem, 0 },
    },
    {
        "-=",
        ctEqMinus,
        { 0 },
    },
    {
        "--",
        ctDoubleMinus,
        { 0 },
    },
    {
        "->",
        ctPtrMem,
        { 0 },
    },
    {
        "+",
        ctPlus,
        { ctEqPlus, ctDoublePlus, 0 },
    },
    {
        "+=",
        ctEqPlus,
        { 0 },
    },
    {
        "++",
        ctDoublePlus,
        { 0 },
    },
    {
        "=",
        ctEq,
        { ctDoubleEq, 0 },
    },
    {
        "==",
        ctDoubleEq,
        { 0 },
    },
    {
        "/",
        ctSlash,
        { ctEqDiv, ctCommSingle, ctCommMulti, 0, },
    },
    {
        "//",
        ctCommSingle,
        { 0 },
    },
    {
        "/*",
        ctCommMulti,
        { 0 },
    },
    {
        "|",
        ctBar,
        { ctEqOr, ctOr, 0 },
    },
    {
        "|=",
        ctEqOr,
        { 0 },
    },
    {
        "||",
        ctOr,
        { 0 },
    },
    {
        "%",
        ctPerc,
        { ctEqMod, 0 },
    },
    {
        "%=",
        ctEqMod,
        { 0 },
    },
    {
        "^",
        ctCaret,
        { ctEqXor, 0 },
    },
    {
        "^=",
        ctEqXor,
        { 0 },
    },
};

size_t items_count = sizeof(items) / sizeof(*items);

typedef struct rawc {
    struct rawc *prev, *next;
    size_t hash;
    char *key;
} rawc;

int main() {

    for(size_t i = 0; i < items_count; i++) {

    }

    return 0;
}