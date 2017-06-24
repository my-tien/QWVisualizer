project(qwvisualizer)
cmake_minimum_required(VERSION 3.1)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "RELEASE")
endif()

file(GLOB sources *.cpp)
file(GLOB headers *.h)
set(SRC_LIST ${sources} ${headers})

set(CMAKE_CXX_STANDARD 14)

find_package(Qt5 5.8 REQUIRED COMPONENTS Core DataVisualization Gui Widgets)
find_package(Eigen3 REQUIRED)

qt_wrap_cpp(${PROJECT_NAME} SRC_LIST ${headers})

add_executable(${PROJECT_NAME} ${SRC_LIST})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${EIGEN3_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::DataVisualization
    Qt5::Gui
    Qt5::Widgets
    -fuse-ld=gold
)

target_compile_options(${PROJECT_NAME} PRIVATE "-pedantic-errors")
target_compile_options(${PROJECT_NAME} PRIVATE "-Wall" "-Wextra")

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE "-fcolor-diagnostics")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(${PROJECT_NAME} PRIVATE "-fdiagnostics-color=always")
endif()

string(TOUPPER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE_UPPER_CASE)
if(CMAKE_BUILD_TYPE_UPPER_CASE MATCHES "DEBUG")
    target_compile_definitions(${PROJECT_NAME} PRIVATE "_GLIBCXX_DEBUG")
    target_compile_definitions(${PROJECT_NAME} PRIVATE "_GLIBCXX_DEBUG_PEDANTIC")
endif()
