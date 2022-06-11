# tsf - A type safe printf-compatible C++ library

- Type safe
- Small (234 lines in .h file, 465 lines in .cpp file)
- Compatible with printf formatting rules
- Optional interface to provide own buffer
- Public domain license

## Example

```cpp
tsf::fmt("%v %v", "abc", 123)        // -->  "abc 123"     <== Use %v as a generic value type
tsf::fmt("%s %d", "abc", 123)        // -->  "abc 123"     <== Auto fixup if your type is wrong
tsf::fmt("%v", std::string("abc"))   // -->  "abc"         <== std::string
tsf::fmt("%v", std::wstring("abc"))  // -->  "abc"         <== std::wstring
tsf::fmt("%.3f", 25.5)               // -->  "25.500"      <== Use format strings as usual
tsf::print("%v", "Hello world")      // -->  "Hello world" <== Print to stdout
tsf::print(stderr, "err %v", 5)      // -->  "err 5"       <== Print to stderr (or any other FILE*)
```

## Implementation

We use snprintf as a backend, so all of the regular formatting
commands that you expect from the printf family of functions work.
This makes the code much smaller than other implementations.

We do however implement some of the common operations ourselves,
such as emitting integers or plain strings, because most snprintf
implementations are actually very slow, and we can gain a lot of
speed by doing these common operations ourselves.

## Known unsupported features

- Positional arguments
- `%*s` (integer width parameter) -- wouldn't be hard to add. Presently ignored.

## Usage

If you specify invalid formatting types (eg `%f` instead of `%d`), then tsf rewrites
your formatting symbol with one that is compatible with the actual type passed in.
For example, if you say `%d`, and you pass in a `const char*`, then the `%d` is
rewritten into `%s`.

Generally, you just use `%v`, which stands for "value".

tsf is very light on templates - it uses them only for type safe variadic argument lists,
so impact on compile times is negligible.

To use tsf, just add `tsf.cpp` to your project, and `#include "tsf.h"`.

## API

```
fmt           returns std::string.
fmt_buf       is useful if you want to provide your own buffer to avoid memory allocations.
print         prints to stdout
print(FILE*)  prints to any FILE*
```

By providing a cast operator to `fmtarg`, you can get an arbitrary type
supported as an argument, provided it fits into one of the molds of the
printf family of arguments.

We also support two custom types: `%Q` and `%q`. In order to use these, you need to provide your own
implementation that wraps one of the lower level functions, and provides a 'context' object with
custom functions defined for `Escape_Q` and `Escape_q`. These were originally added in order to provide
quoting and escaping for SQL identifiers and SQL string literals.

## Test

```
clang -O2 test.cpp tsf.cpp -std=c++11 -lstdc++ -lm && ./a.out
gcc -O2 test.cpp tsf.cpp -std=c++11 -lstdc++ -lm && ./a.out
```
