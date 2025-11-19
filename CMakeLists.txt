cmake_minimum_required(VERSION 3.12)
project(OptionsPricer CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

include_directories(include)

set(SRC_FILES
    src/math/math.cpp
    src/models/black_sholes.cpp
    main.cpp
)

message(STATUS "Looking for source files...")
foreach(src_file ${SRC_FILES})
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${src_file})
        message(STATUS "Found: ${src_file}")
    else()
        message(WARNING "Missing: ${src_file}")
    endif()
endforeach()

add_executable(${PROJECT_NAME} ${SRC_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)