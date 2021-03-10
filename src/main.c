#include "9cc.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "%s: invalid number of arguments\n", argv[0]);
        return EXIT_FAILURE;
    }

    Token* tok     = tokenize(argv[1]);
    Function* prog = parse(tok);
    codegen(prog);

    return EXIT_SUCCESS;
}