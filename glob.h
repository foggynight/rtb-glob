// glob.h - Find strings matching a pattern.
//
// Copyright (C) 2025 Robert Coffey
// Released under the MIT license.
//
// Version: 0.1.0

#include <stdbool.h>

bool glob(const char *pattern, const char *text);

#ifdef RTB_GLOB_IMPL
#undef RTB_GLOB_IMPL

static void error_msg(const char *msg) {
    fprintf(stderr, "glob: %s\n", msg);
}

#define CHAR_COUNT 256
static bool charset[CHAR_COUNT];

static void charset_reset(void) {
    for (size_t i = 0; i < CHAR_COUNT; ++i)
        charset[i] = false;
}

static void charset_add(char c) {
    charset[(size_t)c] = true;
}

static bool charset_contains(char c) {
    return charset[(size_t)c];
}

bool glob(const char *pattern, const char *text) {
    while (*pattern != '\0' && *text != '\0') {
        switch (*pattern) {
        case '?':
            ++pattern;
            ++text;
            break;

        case '*':
            while (*pattern == '*')
                ++pattern;
            if (*pattern == '\0')
                return true;
            for (const char *t = text; *t; ++t)
                if (glob(pattern, t))
                    return true;
            return false;

        case '[': {
            const char *next = pattern + 1;

            // Empty character set, skip.
            if (*next == ']') {
                pattern = next + 1;
                continue;
            }

            // Populate character set with valid characters.
            charset_reset();
            char prev;
            while (*next && *next != ']') {
                if (*next != '-' || next[-1] == '[' || next[1] == ']') {
                    charset_add(*next);
                } else {
                    char range = next[1] - prev;
                    for (char i = 1; i < range; ++i)
                        charset_add(prev + i);
                }
                prev = *next;
                ++next;
            }
            if (!*next || *next != ']') {
                error_msg("missing closing bracket");
                return false;
            }
            pattern = next + 1;

            // Check next character of text.
            if (!charset_contains(*text))
                return false;
            ++text;
        } break;

        default:
            if (*pattern != *text)
                return false;
            ++pattern;
            ++text;
            break;
        }
    }

    if (*pattern == '\0')
        return *text == '\0';
    if (*text == '\0') {
        while (*pattern == '*' || (*pattern == '[' && *(pattern + 1) == ']')) {
            if (*pattern == '*') pattern += 1;
            else if (*pattern == '[' && *(pattern + 1) == ']') pattern += 2;
        }
        return *pattern == '\0';
    }
    return false;
}

#endif // RTB_GLOB_IMPL
