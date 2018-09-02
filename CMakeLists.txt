CMAKE_MINIMUM_REQUIRED(VERSION 3.12)

PROJECT(mruby-test)
if("${MRUBY_DIR}" STREQUAL "")
    message(FATAL_ERROR "Missing MRUBY_DIR. Must point to the installed MRUBY path. ex. -DMRUBY_DIR=path/to/mruby/inst")
endif()
# Remove unwanted project types in MSVC
if(MSVC)
    list(REMOVE_ITEM CMAKE_CONFIGURATION_TYPES MinSizeRel RelWithDebInfo)
endif()

add_library(mruby STATIC IMPORTED)
if(WIN32)
    set(LIBEXT lib)
else()
    set(LIBEXT a)
endif()
set_target_properties(mruby PROPERTIES IMPORTED_LOCATION_DEBUG   ${MRUBY_DIR}/build/host-debug/lib/libmruby.${LIBEXT})
set_target_properties(mruby PROPERTIES IMPORTED_LOCATION_RELEASE ${MRUBY_DIR}/build/host/lib/libmruby.${LIBEXT})
    
set(SRC mruby-test binding.cpp function.cpp stdafx.cpp binding.h function.h stdafx.h targetver.h)

add_executable(mruby-test ${SRC})
set_property(TARGET mruby-test PROPERTY CXX_STANDARD 17)
set_property(TARGET mruby-test PROPERTY CXX_STANDARD_REQUIRED ON)
target_compile_definitions(mruby-test PRIVATE $<$<CONFIG:Debug>:_DEBUG> _CONSOLE)
target_include_directories(mruby-test PRIVATE ${MRUBY_DIR}/include)
target_link_libraries(mruby-test PRIVATE mruby)
