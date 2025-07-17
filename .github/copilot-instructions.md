# Toolchain
- Emscripten
# Tech
- SDL2, C++20
# Coding convention
- member variable should start with m_
- variable is camelCase
- Function, class, struct name is PascalCase
- Function name must be verb for bool function it must be a yes no question (eg IsLoaded, IsThis, IsThat)
- variable must describe there usage (eg: count, iter)
- remember to include pch.h
- when using ifdef must add a comment with the macro name used in #else and #endif
- when checking for nullptr explicitly use `if (ptr == nullptr)` or `if (ptr != nullptr)` instead of `if (ptr)` or `if (!ptr)`
- use `nullptr` instead of `NULL`