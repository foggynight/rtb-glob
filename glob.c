#include <stdio.h>

#define RTB_GLOB_IMPL
#include "glob.h"

int main(void) {
    //printf("%d\n", glob("ab[cd]", "abc"));
    printf("%d\n", glob("LALARA", "LALARA"));
    printf("%d\n", glob("*LARA", "LALARA"));
    printf("%d\n", glob("LALARA*", "LALARA"));
    printf("%d\n", glob("*LARA*", "LALARA"));
    printf("%d\n", glob("**LARA*", "LALARA"));
    printf("%d\n", glob("*LARA**", "LALARA"));
    printf("%d\n", glob("*L****A*L*?", "LARAL"));
    printf("%d\n", glob("*L****A*L*?", "LARALC"));
    printf("%d\n", glob("*L****A*L[*]?", "LARAL*C"));
    return 0;
}
