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

List *list_push(List *list, const Expect *exp) {
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

const Expect expects[] =  {
    // simple patterns
    { "", "", true },
    { "", "1", false },
    { "1", "", false },
    { "12", "123", false },
    { "123", "123", true },

    // '?' patterns
    { "?", "", false },
    { "?", "?", true },
    { "?", "1", true },
    { "?", "12", false },
    { "??", "12", true },
    { "1?", "12", true },
    { "?2", "12", true },
    { "1?2", "123", false },
    { "123", "123", true },
    { "1?3", "123", true },
    { "1??4", "1234", true },
    { "12?", "123", true },
    { "12?", "1234", false },
    { "12??", "1234", true },
    { "?2?", "123", true },

    // '*' patterns
    { "*", "", true },
    { "***", "", true },
    { "*", "*", true },
    { "***", "*", true },
    { "*", "1", true },
    { "*", "12", true },
    { "*", "123", true },
    { "*1", "1", true },
    { "**1", "1", true },
    { "1*", "1", true },
    { "1*3", "13", true },
    { "1*3", "123", true },
    { "1*4", "1234", true },
    { "1*3", "1234", false },
    { "1*3", "1234", false },
    { "1**3", "1234", false },
    { "1**3", "123", true },
    { "12*", "12", true },
    { "12*", "123", true },
    { "12*", "1234", true },
    { "12**", "12", true },
    { "*12*", "12", true },
    { "*12*", "123", true },
    { "*1*2*", "123", true },
    { "*1*2*", "123", true },
    { "*1*2*3", "123", true },
    { "*1*2*", "13", false },
    { "*123", "12123", true },
    { "*12", "12123", false },
    { "*1234", "12123", false },
    { "*12*", "12123", true },
    { "*123*", "12123", true },
    { "123*", "12123", false },
    { "**123*", "12123", true },
    { "**1*2*3**", "12123", true },
    { "*1*2*3*", "132", false },
    { "*1*2*3*", "321", false },

    // wildcard combinations
    { "?*", "", false },
    { "*?", "", false },
    { "?*?", "", false },
    { "*?*", "", false },
    { "?*", "1", true },
    { "*?", "1", true },
    { "?*", "123", true },
    { "*?", "123", true },
    { "?*?", "1", false },
    { "?*?", "12", true },
    { "?*?", "123", true },
    { "*?*", "123", true },
    { "*???*", "123", true },
    { "*????*", "123", false },

    // sets and ranges
    { "[]", "", true },
    { "?[]", "", false },
    { "[]?", "", false },
    { "?[]?", "", false },
    { "*[]", "", true },
    { "[]*", "", true },
    { "*[]*", "", true },
    { "[1]", "1", true },
    { "[1]", "2", false },
    { "[12]", "1", true },
    { "[12]", "2", true },
    { "[1][2]", "1", false },
    { "[1][2]", "2", false },
    { "[1][2]", "12", true },
    { "[12][12]", "12", true },
    { "[][1]", "1", true },
    { "[1][]", "1", true },
    { "[][1][]", "1", true },
    { "[-]", "", false },
    { "[-]", "-", true },
    { "[-]", "1", false },
    { "[1-]", "-", true },
    { "[1-]", "0", false },
    { "[1-]", "1", true },
    { "[1-]", "2", false },
    { "[-1]", "-", true },
    { "[-1]", "0", false },
    { "[-1]", "1", true },
    { "[-1]", "2", false },
    { "[1-2]", "-", false },
    { "[1-2]", "0", false },
    { "[1-2]", "1", true },
    { "[1-2]", "2", true },
    { "[1-2]", "3", false },
    { "[1-3]", "0", false },
    { "[1-3]", "1", true },
    { "[1-3]", "2", true },
    { "[1-3]", "3", true },
    { "[1-3]", "4", false },
    { "[-1-2]", "-", true },
    { "[-1-2]", "-1", false },
    { "[-1-2]", "0", false },
    { "[-1-2]", "1", true },
    { "[-1-2]", "2", true },
    { "[-1-2]", "3", false },
    { "[1-2-]", "-", true },
    { "[1-2-]", "0", false },
    { "[1-2-]", "1", true },
    { "[1-2-]", "2", true },
    { "[1-2-]", "3", false },
    { "[a1-2]", "a", true },
    { "[a1-2]", "b", false },
    { "[1-2a]", "a", true },
    { "[1-2a]", "b", false },

    // escaped characters
    { "\\", "\\", true },
    { "\f", "\f",  true },
    { "\n", "\n",  true },
    { "\r", "\r",  true },
    { "\t", "\t",  true },
    { "\v", "\v",  true },
};

int main(void) {
    List *fail_list = NULL;
    for (size_t i = 0; i < sizeof(expects) / sizeof(*expects); ++i) {
        const Expect *exp = expects + i;
        if (glob(exp->pattern, exp->text) == exp->result) {
            printf("[PASS] ");
        } else {
            printf("[FAIL] ");
            fail_list = list_push(fail_list, exp);
        }
        printf("glob(\"%s\", \"%s\") == %s\n",
               exp->pattern, exp->text, exp->result ? "true" : "false");
    }

    if (fail_list == NULL) {
        printf("SUCCESS!\n");
    } else {
        fail_list = list_reverse(fail_list);
        printf("FAILs:\n");
        while (fail_list != NULL) {
            const Expect *exp = fail_list->expect;
            printf("  glob(\"%s\", \"%s\") == %s\n",
                   exp->pattern, exp->text, exp->result ? "true" : "false");
            fail_list = fail_list->next;
        }
    }
}
