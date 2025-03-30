#include <stdio.h>
#include <stdlib.h>

#include "base.h"
#include "str.h"
// #include "args.h"

int main(int argc, char **argv) {
    // simple file-to-file translator currently
    if(argc != 3) {
        puts("Usage: ext <input file> <output file>");
        return OK;
    }

    return OK;
}