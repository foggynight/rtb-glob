#include <stdio.h>

#define RTB_GLOB_IMPL
#include "glob.h"

int main(int argc, char **argv) {
    printf("%d\n", glob("1**3*", "123"));
    return 0;
}
