cmake_minimum_required(VERSION 3.26.0)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/stm32_gcc.cmake)

project(Example1 C CXX ASM)


set(STARTUP_SRC 
${CMAKE_CURRENT_SOURCE_DIR}/startup_stm32f302r8tx.s 
${CMAKE_CURRENT_SOURCE_DIR}/syscalls.c 
${CMAKE_CURRENT_SOURCE_DIR}/sysmem.c
)

#***
# normally when you are writing your program it's recommended to add ST7735S library as a submodule in git and here as a subdirectory,
# then you don't need to create library - it's created automatically
set(ST7735S_SRC
../st7735s.c
../st7735s.h 
../st7735s_config.h 
../st7735s_cl.h 
../font8x8_basic.h
)
add_library(ST7735S ${ST7735S_SRC})
#***
add_library(Startup ${STARTUP_SRC})

add_library(Driver driver.c)   # this lib you should create providing functions marked as extern in st7735s_config.h (see README)

add_executable(Example1 main.c)

target_link_options(${PROJECT_NAME} PRIVATE ${LINKER_FLAGS})
target_link_options(Startup PRIVATE ${LINKER_FLAGS})
target_link_options(ST7735S PRIVATE ${LINKER_FLAGS})
target_link_options(Driver PRIVATE ${LINKER_FLAGS})

target_compile_options(${PROJECT_NAME} PRIVATE ${COMPILE_FLAGS})
target_compile_options(Startup PRIVATE ${COMPILE_FLAGS})
target_compile_options(ST7735S PRIVATE ${COMPILE_FLAGS})
target_compile_options(Driver PRIVATE ${COMPILE_FLAGS})

target_link_libraries(${PROJECT_NAME} Startup)
target_link_libraries(${PROJECT_NAME} ST7735S)
target_compile_options(Driver PRIVATE ${COMPILE_FLAGS})

# normally there should be your subdirectory instead of ../
target_include_directories(${PROJECT_NAME} PRIVATE ../)
target_include_directories(Driver PRIVATE ../)

target_link_libraries(ST7735S Driver) # you should link your driver library to ST7735S lib