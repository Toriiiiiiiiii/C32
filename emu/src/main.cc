#include <cstdio>
#include <pthread.h>
#include <vector>

#include "arguments.h"
#include "memory.h"

void printUsage(char **argv) {
    printf("Usage : %s [-h]\n", argv[0]);
}

int main(int argc, char **argv) {
    std::vector<ArgumentTemplate> usage = {
        ArgumentTemplate{ARG_OPTIONAL | ARG_IMPLIED, "-h", "help"}
    };

    ArgumentParser *argParser = new ArgumentParser(argc, argv, usage);

    std::vector<Argument> args;
    if(!argParser->parseArguments(args)) {
        printUsage(argv);
        goto cleanup;
    }

    if(argParser->argumentExists(args, "help")) {
        printUsage(argv);
        goto cleanup;
    }

cleanup:
    delete argParser;
    return 0;
}