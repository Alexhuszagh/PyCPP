# Style Guide

PyCPP is a relatively large project, and therefore should try to maintain a consistent style internally.

**Table of Contents**

- [General Recommendations](#general-recommendations)
- [Headers](#headers)
- [Template Metaprogramming](#template-metaprogramming)
- [Types](#types)

## General Recommendations

- Prefer `using x = y;` to `typedef y x;`, since the more important factor (the alias) is on the left size of the expression.

## Headers

Headers should declare a brief description, a detailed description, and a synopsis of the public API, such as that found in [allocator/crt.h](/pycpp/allocator/crt.h). Functions and methods not comparable to those found in the standard template library should be annotated in the class declaration: the synopsis acts as a brief overview of the class structure. Implementation details should not be included in the synopsis.

## Template Metaprogramming

- Always alias `using deduced_type_t = typename deduced_type::type` for `struct deduced_type { using type = /**/; };`

## Types

- Paths should use a type derived from `basic_string_view` or `basic_string`.
- Codec and casemapping, which should support multiple codepoints, should use `basic_string_view`.
- All other string types should use `string_wrapper` with UTF-8 contents.
