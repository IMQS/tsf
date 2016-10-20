# tsf - A type safe printf-compatible C++ library

* Type safe
* Small (178 lines in .h file, 322 lines in .cpp file)
* Compatible with printf formatting rules
* Optional interface to achieve zero allocations during operation

```cpp
tsf::fmt("Hello %v %v", "world", 123); // "Hello world 123"  - Use %v for any value
tsf::fmt("%.1f", 5.12);                // "5.1"              - Familiar printf rules work
```

tsf is much smaller than other comparable C++ type-safe formatting libraries, because
it just acts as a sanitizer in front of `snprintf`.

If you specify invalid formatting types (eg `%f` instead of `%d`), then tsf forces
your formatting symbol to at least be compatible with the actual type passed in.
For example, if you say `%d`, and you pass in a `const char*`, then the `%d` is
rewritten into `%s`.

Generally, you just use `%v`.

tsf is very light on templates - it uses them only for variadic argument lists, so
impact on compile times is negligible.

To use tsf, just add `tsf.cpp` to your project, and `#include "tsf.h"`.

API

```cpp
std::string fmt(const char* fs, ...)                                                   // Format to string
CharLenPair fmt_static_buf(char* staticbuf, size_t staticbuf_len, const char* fs, ...) // Try to use static buffer. Fallback to heap.
size_t      fmt_write(FILE* file, const char* fs, ...)                                 // Write to file
size_t      fmt_print(const char* fs, ...)                                             // Write to stdout
```
