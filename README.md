# CppNameForward

Name forwarding in C++ can be very boilerplate heavy requiring a definition for each method you want to wrap.

This library aims to provide a much easier method to name forward items to the standard library.

# Usage:
Each class should have a method that returns a reference to the object to forward to, we call these identity methods.

To allow forwarding multiple objects from a single class there are multiple identity methods:
- `ToForwardTo()` for objects that are not containers or tuple
- `ContainerToForwardTo()` for containers, tuples and generators.

```C++
#include <Name/Forward.hpp>

struct VectorLike : name_forward::EmplaceBack<VectorLike>, name_forward::Back<VectorLike> {
    std::vector<int>& ContainerToForwardTo() { return to_name_forward; }
private:
    std::vector<int> to_name_forward;
};
```
That object v must have all the methods that are forwarded, in this example `emplace_back` and `back`.

Note that `back` is not defined on a `const VectorLike` in this example as `ContainerToForwardTo` does not have a const overload.

The following will always provide a `back` method. 
```C++
struct ConstRange : name_forward::Iterators<ConstRange>, name_forward::Back<ConstRange> {
    const std::deque<int>& ContainerToForwardTo() const { return to_name_forward; }
private:
    std::deque<int> to_name_forward;
};
```

A helper CRTP classes have been provided for those who do not wish to write both overloads:
```C++
#include <Name/Forward/Id.hpp>
struct UsesNamedVar : name_forward::identity::PreferVar_toForwardTo_Container<UsesNamedVar> {
    // Identifer must be accessible to `name_forward::identity::ClassName` (public or friend)
    std::array<int, 5> toForwardTo; // Identifer is important
};
```
All classes are constexpr friendly:
```C++
static_assert(UsesNamedVar{}.size() == 5, "failed");
```

# Building
If using CMake the user can:
```Bash
cmake -B dir -S .
cmake --build dir
cmake --install dir
```

Though this is a header only library so copying the required headers into an include directory is perfectly reasonable.

For building tests, mutation tests, sanitizers please see [CONTRIBTING.md](CONTRIBUTING.md).


