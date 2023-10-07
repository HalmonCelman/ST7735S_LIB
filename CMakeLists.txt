if(${CMAKE_VERSION} VERSION_LESS "3.26.0") 
    message(WARNING "Current CMake version is ${CMAKE_VERSION}. On version 3.26.0 everything is working properly")
endif()

set(ST7735S_SOURCES
st7735s.c
)

set(ST7735S_HEADERS 
st7735s_config.h
st7735s.h
st7735s_cl.h
font8x8_basic.h
)

get_property(LINK_OPTIONS GLOBAL PROPERTY ST7735S_LINK_OPTIONS)
get_property(COMPILE_OPTIONS GLOBAL PROPERTY ST7735S_COMPILE_OPTIONS)

add_library(ST7735S ${ST7735S_SOURCES} ${ST7735S_HEADERS})

target_link_options(ST7735S PRIVATE ${LINK_OPTIONS})
target_compile_options(ST7735S PRIVATE ${COMPILE_OPTIONS})