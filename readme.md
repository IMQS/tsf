# tsf - A type safe printf-compatible C++ library

* Type safe
* Small (234 lines in .h file, 465 lines in .cpp file)
* Compatible with printf formatting rules
* Optional interface to provide own buffer
* Public domain license

## Example

```cpp
tsf::fmt("Hello %v %v", "world", 123); // "Hello world 123"  - Use %v for any value
tsf::fmt("%.1f", 5.12);                // "5.1"              - Familiar printf rules work
```

## Usage

tsf is much smaller than other comparable C++ type-safe formatting libraries,
because it is really just a sanitizer in front of `snprintf`.

If you specify invalid formatting types (eg `%f` instead of `%d`), then tsf rewrites
your formatting symbol with one that is compatible with the actual type passed in.
For example, if you say `%d`, and you pass in a `const char*`, then the `%d` is
rewritten into `%s`.

Generally, you just use `%v`, which stands for "value".

tsf is very light on templates - it uses them only for type safe variadic argument lists,
so impact on compile times is negligible.

To use tsf, just add `tsf.cpp` to your project, and `#include "tsf.h"`.

## API

```cpp
std::string fmt(const char* fs, ...);                                // Format to string
StrLenPair  fmt_buf(char* buf, size_t buf_len, const char* fs, ...); // Try to use buffer. Fallback to heap.
size_t      printfmt(FILE* file, const char* fs, ...);               // Write to file
size_t      printfmt(const char* fs, ...);                           // Write to stdout
```

## Test

```
clang -O2 test.cpp tsf.cpp -std=c++11 -lstdc++ -lm && ./a.out
gcc -O2 test.cpp tsf.cpp -std=c++11 -lstdc++ -lm && ./a.out
```
