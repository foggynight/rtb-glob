# rtb-glob

Find strings matching a pattern, similar to regular expressions.

This is a single header file library for C. It is primarily composed of 2 files:
- `glob.h`: Single header file library.
- `glob.c`: Command-line program which... TODO

To include `glob` in your C program:
- `#define RTB_GLOB_IMPL` before `#include "glob.h"` in a single file.
- Just `#include "glob.h"` everywhere else.
See `glob.c` for an example.


## Pattern Quantifiers

This implementation of glob follows the pattern quantifiers standardized by
POSIX.2. The following descriptions were taken from Wikipedia with slight
modifications.

### Wildcards
- `?` (not in brackets) matches any character exactly once.
- `*` (not in brackets) matches a string of zero or more characters.

### Sets/Ranges
- [...], where the first character within the brackets is not '!', matches any
  single character among the characters specified in the brackets. If the first
  character within brackets is '!', then the [!...] matches any single character
  that is not among the characters specified in the brackets.
- The characters in the brackets may be a list ([abc]) or a range ([a-c]).
- For the sake of simplicity, character classes (like [[:space:]], where the
  inner brackets are part of the classname) are not supported.

Source of above definitions (with slight modifications):
<https://en.wikipedia.org/wiki/Glob_(programming)>


## License

MIT License

Copyright (C) 2025 Robert Coffey

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
