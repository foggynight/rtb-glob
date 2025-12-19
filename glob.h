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

// TODO: Rewrite using switch instead of ifs.
// TODO: Add character escapes.
// TODO: Add sets and ranges.
bool glob(const char *pattern, const char *text) {
    for (;;) {
        if (*pattern == '\0' && *text == '\0') {
            return true;

        } else if (*pattern == '?') {
            if (*text == '\0') return false;
            ++pattern; ++text;

        } else if (*pattern == '*') {
            const char *next; // next non-wildcard character in pattern
            for (next = pattern; *next != '\0'; ++next)
                if (*next != '?' && *next != '*')
                    break;
            while (*text != '\0' && *text != *next)
                ++text;
            pattern = next;

        } else if (*pattern++ != *text++) {
            return false;
        }
    }
}

#endif // RTB_GLOB_IMPL
