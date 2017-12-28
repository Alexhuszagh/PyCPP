# SFINAE

**Table of Contents**

- [Introduction](#introduction)
- [Operation Detection](#operator-detection)
  - [Has Bit And](#has-bit-and)
  - [Has Bit And Assign](#has-bit-and-assign)
  - [Has Bit Or](#has-bit-or)
  - [Has Bit Or Assign](#has-bit-or-assign)
  - [Has Bit Xor](#has-bit-xor)
  - [Has Bit Xor Assign](#has-bit-xor-assign)
  - [Has Divides](#has-divides)
  - [Has Divides Assign](#has-divides-assign)
  - [Has Equal To](#has-equal-to)
  - [Has Greater](#has-greater)
  - [Has Greater Equal](#has-greater-equal)
// TODO: here

- [Member Variable, Function, and Type Detection](#member-variable-function-and-type-detection)
- [Type Identification](#type-identification)
- [Fail Safe Implementations](#fail-safe-implementations)
  - [Back](#back)
  - [Front](#front)
  - [Emplace Back](#emplace-back)
  - [Emplace Front](#emplace-front)

## Introduction

Substitution failure is not an error (SFINAE) enables the selection of proper function overloads by rejecting templates with substitution failures. PyCPP uses SFINAE extensively in its container design, and provides type traits for the detection of supported operators, member variable, function, and type detection, type identification, as well as fail-safe member function implementations.

## Operation Detection

### Has Bit And

Detect if two types have a bitwise and (`&`) operator.

```cpp
#include <pycpp/sfinae/has_bit_and.h>

PYCPP_USING_NAMESPACE

struct x {};

int main()
{
    static_assert(has_bit_and<int, int>::value, "");
    static_assert(!has_bit_and<x, int>::value, "");
    return 0;
}
```

### Has Bit And Assign

Detect if two types have a bitwise and assignment (`&=`) operator.

```cpp
#include <pycpp/sfinae/has_bit_and_assign.h>

PYCPP_USING_NAMESPACE

struct x {};

int main()
{
    static_assert(has_bit_and_assign<int, int>::value, "");
    static_assert(!has_bit_and_assign<x, int>::value, "");
    return 0;
}
```

### Has Bit Or

Detect if two types have a bitwise or (`|`) operator.

```cpp
#include <pycpp/sfinae/has_bit_or.h>

PYCPP_USING_NAMESPACE

struct x {};

int main()
{
    static_assert(has_bit_or<int, int>::value, "");
    static_assert(!has_bit_or<x, int>::value, "");
    return 0;
}
```

### Has Bit Or Assign

Detect if two types have a bitwise or assignment (`|=`) operator.

```cpp
#include <pycpp/sfinae/has_bit_or_assign.h>

PYCPP_USING_NAMESPACE

struct x {};

int main()
{
    static_assert(has_bit_or_assign<int, int>::value, "");
    static_assert(!has_bit_or_assign<x, int>::value, "");
    return 0;
}
```

### Has Bit Xor

Detect if two types have a bitwise xor (`^`) operator.

```cpp
#include <pycpp/sfinae/has_bit_xor.h>

PYCPP_USING_NAMESPACE

struct x {};

int main()
{
    static_assert(has_bit_xor<int, int>::value, "");
    static_assert(!has_bit_xor<x, int>::value, "");
    return 0;
}
```

### Has Bit Xor Assign

Detect if two types have a bitwise xor assignment (`^=`) operator.

```cpp
#include <pycpp/sfinae/has_bit_xor_assign.h>

PYCPP_USING_NAMESPACE

struct x {};

int main()
{
    static_assert(has_bit_xor_assign<int, int>::value, "");
    static_assert(!has_bit_xor_assign<x, int>::value, "");
    return 0;
}
```

### Has Divides

Detect if two types have a bitwise xor (`/`) operator.

```cpp
#include <pycpp/sfinae/has_divides.h>

PYCPP_USING_NAMESPACE

struct x {};

int main()
{
    static_assert(has_divides<int, int>::value, "");
    static_assert(!has_divides<x, int>::value, "");
    return 0;
}
```

### Has Divides Assign

Detect if two types have a bitwise xor assignment (`/=`) operator.

```cpp
#include <pycpp/sfinae/has_divides_assign.h>

PYCPP_USING_NAMESPACE

struct x {};

int main()
{
    static_assert(has_divides_assign<int, int>::value, "");
    static_assert(!has_divides_assign<x, int>::value, "");
    return 0;
}
```

### Has Equal To

Detect if two types have an equal to (`==`) operator.

```cpp
#include <pycpp/sfinae/has_equal_to.h>

PYCPP_USING_NAMESPACE

struct x {};

int main()
{
    static_assert(has_equal_to<int, int>::value, "");
    static_assert(!has_equal_to<x, int>::value, "");
    return 0;
}
```

### Has Greater

Detect if two types have a greater than (`>`) operator.

```cpp
#include <pycpp/sfinae/has_greater.h>

PYCPP_USING_NAMESPACE

struct x {};

int main()
{
    static_assert(has_greater<int, int>::value, "");
    static_assert(!has_greater<x, int>::value, "");
    return 0;
}
```

### Has Greater Equal

Detect if two types have a greater than or equal to (`>=`) operator.

```cpp
#include <pycpp/sfinae/has_greater_equal.h>

PYCPP_USING_NAMESPACE

struct x {};

int main()
{
    static_assert(has_greater_equal<int, int>::value, "");
    static_assert(!has_greater_equal<x, int>::value, "");
    return 0;
}
```

// TODO:

## Member Variable, Function, and Type Detection

// TODO:

## Type Identification

// TODO:

## Fail Safe Implementations

### Back

Access the last item in a container.

```cpp
#include <pycpp/sfinae/back.h>
#include <set>
#include <vector>

PYCPP_USING_NAMESPACE

int main()
{
    std::set<int> s = {1, 2, 3};
    std::vector<int> v = {1, 2, 3};
    back()(s);      // 3
    back()(v);      // 3
    return 0;
}
```

### Front

Access the first item in a container.

```cpp
#include <pycpp/sfinae/front.h>
#include <set>
#include <vector>

PYCPP_USING_NAMESPACE

int main()
{
    std::set<int> s = {1, 2, 3};
    std::vector<int> v = {1, 2, 3};
    front()(s);      // 1
    front()(v);      // 1
    return 0;
}
```

### Emplace Back

Construct item in-place at end of container.

```cpp
#include <pycpp/sfinae/emplace_back.h>
#include <set>
#include <vector>

PYCPP_USING_NAMESPACE

int main()
{
    std::set<int> s = {1, 2, 3};
    std::vector<int> v = {1, 2, 3};
    emplace_back()(s, 4);
    emplace_back()(v, 4);
    return 0;
}
```

### Emplace Front

Construct item in-place at start of container.

```cpp
#include <pycpp/sfinae/emplace_front.h>
#include <set>
#include <vector>

PYCPP_USING_NAMESPACE

int main()
{
    std::set<int> s = {1, 2, 3};
    std::vector<int> v = {1, 2, 3};
    emplace_front()(s, 0);
    emplace_front()(v, 0);
    return 0;
}
```

// TODO:
