# C++17 Minimalistic Json parser.

The code is just a header under 200 lines of code. There is no particular
reason why I wrote it. It was just a weekend fun project to practice finite
state machines.

## Mini Json parser supports:
- *strings*
- *arrays* (no nested arrays supported)
- *objects*

```json
{
    "k1": "",
    "a1": [],
    "o1": {}
}
```

## Simple usage of the parser:

```c++
#include <mjson/mjson.hpp>

void foo() {
    const auto cfg = R"(
        {
            "key" : "value"
        }
    )";

    mjson::json js(cfg);

    if (!js.is_valid()) {
        std::cout << "FAILURE: Format error!" << std::endl;
        return;
    }

    if (!js.has("key")) {
        std::cout << "No 'key' field found" << std::endl;
        return;
    }

    const auto& key = js["key"];
    std::cout << "key: " << key << std::endl;
}
```

## Files:
- The header is [here](/include/mjson/mjson.hpp)
- The hello sample application is [here](/apps/hello_mjson/src/hello_mjson.cpp)
- Catch2 unit tests are [here](/test/mjson_test/src/mjson_test.cpp)

## Build
The code has been built and tested on Windows and Linux using MS Visual Studio
2019 and gcc 9.3.0 on Ubuntu 18.4

### Microsoft Visual Studio
To create Microsoft Visual Studio solution and project files, and build the
solution from PowerShell or CMD:
```sh
mkdir -p build/msvs
cd build/msvs
cmake ../..
cmake --build .
```
Microsoft Visual Studio solution file will be */build/msvs/mjson.sln*

### Linux
To build on Linux using make:
```sh
mkdir -p build/x86_x64_linux
cd build/x86_x64_linux
cmake ../..
make
```
