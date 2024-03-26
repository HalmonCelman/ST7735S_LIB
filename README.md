# ST7735S library

## Assumptions:
- compiling also with CMake
- nothing have to be changed inside library, only one file for extern function needed
- working on all platforms
- highly customizable
- **you can freely customize this library without changing code in this repository**

## Usage(recommended way):
- Add as a submodule
- copy config file `st7735s_config.h` into parent folder

(f.e. if you have your submodule in External/ST7735S/ you should put config in External/ . *Config file in External/ST7735S/ is **unused** it exist only as template - so you should edit only config file in External/*)
- **! if you are using some special compiler flags you should set them as properties (see below) !**
- in CMakeLists.txt add as a subdirectory
- write driver file: [How to write driver file?](driver_file.md)
- create driver library
- link driver library to ST7735S library
- link ST7735S library to your project
- Enjoy!

### Compiler and Linker options
if you want to pass your compiler or linker options( *without that even if code compiles it might not work* ) you should do it as below:
```
set_property(GLOBAL PROPERTY ST7735S_LINK_OPTIONS "<linker flags here>")
set_property(GLOBAL PROPERTY ST7735S_LINK_OPTIONS "<compiler flags here>")
```
these two lines should be **before** add_subdirectory command

### When you want to config something in submodule
You know sometimes you may want to change something and you dont want from git to track these changes

Then try this:
```
git update-index --assume-unchanged <file you want to change>
```

after that git will think that this file is still unchanged, when you want to trace back this file:

```
git update-index --no-assume-unchanged <file you want to change>
```


### Example CMakeLists.txt:
```
cmake_minimum_required(VERSION 3.26.0)

# here you should define toolchain file if its needed on your platform

# here compiler and linker options (section above) 
add_subdirectory(External/ST7735S)

project(MyProject C CXX ASM)

add_executable(MyProject main.c)

add_library(Driver YourDriverFile.c)

target_link_library(ST7735S Driver)
target_link_library(MyProject ST7735S)

```

you can also see example written for stm32f302 in [Examples repo](https://github.com/HalmonCelman/Examples/tree/master/ST7735S)
