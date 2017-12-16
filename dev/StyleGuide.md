# Style Guide

PyCPP is a relatively large project, and therefore should try to maintain a consistent style internally.

**Table of Contents**

- [General Recommendations](#general-recommendations)
- [Template Metaprogramming](#template-metaprogramming)
- [Types](#types)

## General Recommendations

- Prefer `using x = y;` to `typedef y x;`, since the more important factor (the alias) is on the left size of the expression.

## Template Metaprogramming

- Always alias `using deduced_type_t = typename deduced_type::type` for `struct deduced_type { using type = /**/; };`

## Types

- Paths should use a type derived from `basic_string_view` or `basic_string`.
- Codec and casemapping, which should support multiple codepoints, should use `basic_string_view`.
- All other string types should use `string_wrapper` with UTF-8 contents.
