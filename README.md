# ST7735S library

## Assumptions:
- compiling also with CMake
- nothing have to be changed inside library, only one file for extern function needed
- working on all platforms
- highly customizable

## Usage(recommended way):
- Add as a submodule
- in CMakeLists.txt add as a subdirectory
- write driver file: [How to write driver file?](driver_file.md)
- create driver library
- link driver library to ST7735S library
- link ST7735S library to your project
- Enjoy!

### Example CMakeLists.txt:
```
cmake_minimum_required(VERSION 3.26.0)

# here you should define toolchain file if its needed on your platform

add_subdirectory(External/ST7735S)

project(MyProject C CXX ASM)

add_executable(MyProject main.c)

add_library(Driver YourDriverFile.c)

target_link_library(ST7735S Driver)
target_link_library(MyProject ST7735S)

```

you can also see example written for stm32f302 in [Examples repo](https://github.com/HalmonCelman/ST7735S_LIB_Examples)