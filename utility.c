#include "utility.h"
#include <stdio.h>
#include <stdlib.h>

void error_and_exit(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}
