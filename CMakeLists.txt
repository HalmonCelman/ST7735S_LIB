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

add_library(ST7735 ${ST7735S_SOURCES} ${ST7735S_HEADERS})