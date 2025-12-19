#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define RTB_GLOB_IMPL
#include "../glob.h"

typedef struct {
    const char *pattern;
    const char *text;
    const bool result;
} Expect;

typedef struct _List {
    const Expect *expect;
    struct _List *next;
} List;

void error(const char *msg) {
    fprintf(stderr, "error: %s\n", msg);
    exit(1);
}

List *list_push(List *list, Expect *exp) {
    List *node = malloc(sizeof(List));
    if (!node) error("failed to malloc List node");
    node->expect = exp;
    node->next = list;
    return node;
}

List *list_reverse(List *list) {
    List *prev = NULL;
    while (list != NULL) {
        List *next = list->next;
        list->next = prev;
        prev = list;
        list = next;
    }
    return prev;
}

// TODO: add test cases
const Expect expects[] =  {
    { "12", "123", false },
    { "123", "123", true },
};

int main(void) {
    List *fail_list = NULL;
    for (size_t i = 0; i < sizeof(expects)/sizeof(*expects); ++i) {
        Expect exp = expects[i];
        printf("glob(\"%s\", \"%s\") == %s",
               exp.pattern, exp.text, exp.result ? "true" : "false");
        if (glob(exp.pattern, exp.text) == exp.result) {
            printf(" [PASS]\n");
        } else {
            printf(" [FAIL]\n");
            fail_list = list_push(fail_list, &exp);
        }
    }
}
