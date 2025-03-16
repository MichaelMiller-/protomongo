# protomongo

[![GCC](https://github.com/MichaelMiller-/protomongo/actions/workflows/gcc.yml/badge.svg)](https://github.com/MichaelMiller-/protomongo/actions/workflows/gcc.yml)
[![Clang](https://github.com/MichaelMiller-/protomongo/actions/workflows/clang.yml/badge.svg)](https://github.com/MichaelMiller-/protomongo/actions/workflows/clang.yml)
[![MSVC](https://github.com/MichaelMiller-/protomongo/actions/workflows/msvc.yml/badge.svg)](https://github.com/MichaelMiller-/protomongo/actions/workflows/msvc.yml)

**protomongo** is a small header-only library that bridges the gap between the generation of code from [Google Protobuf](https://protobuf.dev) and the serialization of those classes into a [MongoDB](https://www.mongodb.com) database. To accomplish this, **protomongo** uses Protobuf's reflection technology. Unlike [Microsoft.Bond](https://microsoft.github.io/bond), [Google Protobuf](https://protobuf.dev) does not generate a full compile-time schema, so it is not possible to write a zero-overhead abstraction. However, this library allows you to avoid bug-prone and tedious serialization code that has to be written over and over again for each generated class.

## API
Creates a ```bsoncxx::document::value``` from any given Protobuf class.
```cpp
template <typename T>
auto create_document(T const& obj) -> bsoncxx::document::value
```

Finds a single document in this collection that match the provided key-value pair.
```cpp
template <typename T>
auto find_document(mongocxx::collection& collection, std::string_view key, auto const& value) -> std::optional<T>
```

Finds the documents in this collection which match the provided key-value pair.
```cpp
template <typename T, template <typename...> typename ReturnType = std::vector>
auto find_documents(mongocxx::collection& collection, std::string_view key, auto const& value) -> ReturnType<T>
```

Finds all documents in this collection
```cpp
template <typename T, template <typename...> typename ReturnType = std::vector>
auto find_documents(mongocxx::collection& collection) -> ReturnType<T>
```

## Example
```protobuf
syntax = "proto3";

package example;

message User
{
  int64 id = 1;
  string name = 2;
  double cash = 3;
}
```

```cpp
#include "user.pb.h"
// example usage
class user_database
{
    mongocxx::collection collection;    
public:
    user_database() { /* setup database */ }

    auto create(example::User const& obj) -> bool
    {
        auto doc = protomongo::create_document(obj);
        return static_cast<bool>(collection.insert_one(doc.view()));
    }

    using user_id_t = decltype(example::User{}.id());

    auto find(user_id_t const& value) const -> std::optional<example::User>
    {
        return protomongo::find_document<example::User>(collection, "id", value);
    }
};
```

## Usage in CMake projects
If you have installed **protomongo** system-wide you can easily use the CMake-package-system.
```cmake
find_package(protomongo CONFIG REQUIRED)
target_link_libraries(target_name PRIVATE protomongo::protomongo)
```

## Get
There a are several methods to get the library.

### Use CPM
One of them is the setup-free CMake [CPM](https://github.com/cpm-cmake/CPM.cmake) dependency manager:
```cmake
CPMAddPackage("gh:MichaelMiller-/protomongo@1.1.0")
## ...
target_link_libraries(target_name PRIVATE protomongo::protomongo)
```

### Using FetchContent
Alternatively, the project can also be used only in a private context. Simply add the following to your CMakeLists.txt:
```cmake
include(FetchContent)
FetchContent_Declare(sec21
        GIT_REPOSITORY https://github.com/MichaelMiller-/protomongo
        # GIT_TAG ... # optional tag
        )
FetchContent_MakeAvailable(protomongo)
## If BUILD_TESTING is enabled make sure that Catch2 is available.
## find_package(Catch2 CONFIG REQUIRED)
set_target_properties(protomongo PROPERTIES BUILD_TESTING FALSE)

## Link against protomongo
target_link_libraries(target_name PRIVATE protomongo::protomongo)
```


## License
Please see LICENSE.