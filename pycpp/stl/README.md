# STL

Custom wrappers for Standard Template Library (STL) types for PyCPP's custom allocator model. To use a type, for example, a vector, you would traditionally use the following:

```cpp
#include <vector>

using namespace std;

int main()
{
    vector<int> v;
    // do stuff with vector

    return 0;
}
```

In PyCPP, to allow a custom allocator model, by default all types using a [polymorphic allocator](/pycpp/allocator/polymorphic.h). We therefore have created custom aliases of STL template types to allow the default allocator. To use types compatible with PyCPP's high-level API, you would simply do:

```cpp
#include <pycpp/stl/vector.h>

PYCPP_USING_NAMESPACE

int main()
{
    vector<int> v;
    // do stuff with vector

    return 0;
}
```

// TODO: add custom types for all STL types
