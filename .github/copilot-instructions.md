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
- Do not use `using namespace std;` or any other namespace in header files.
- Use `override` keyword for overridden virtual functions.
- Always initialize member variables in the constructor initializer list.
- Prefer `emplace_back` over `push_back` for better performance.
- Check comments for TODOs and FIXMEs, they are important for the codebase.
- Check comments for context, they are important for understanding the codebase.
# Rules
- must ask if lacking information
- must ask if not sure about the implementation
- if the prompt is not clear, ask for clarification
- if the prompt is question, answer it
- if the prompt is a request, fulfill it
- answer in the context of the codebase and related libraries used
- do not suggest code that has been deleted
- answer honestly, brutal facts, no sugar coating and do not fabricate information
- do not suggest code that is not related to the context

