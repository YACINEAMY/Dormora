cmake_minimum_required(VERSION 3.21)

project(UDRMS LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

add_library(udrms_backend
    src/Person.cpp
    src/Student.cpp
    src/Room.cpp
    src/Restaurant.cpp
    src/Dormitory.cpp
    src/University.cpp
    src/JsonStorage.cpp
)

target_include_directories(udrms_backend PUBLIC include)
target_link_libraries(udrms_backend PUBLIC Qt6::Core)
target_compile_options(udrms_backend PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

add_executable(udrms_demo src/main.cpp)
target_link_libraries(udrms_demo PRIVATE udrms_backend)
target_compile_options(udrms_demo PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

add_executable(udrms_tests tests/backend_tests.cpp)
target_link_libraries(udrms_tests PRIVATE udrms_backend)
target_compile_options(udrms_tests PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

add_executable(udrms_gui WIN32 src/gui_main.cpp)
target_link_libraries(udrms_gui PRIVATE udrms_backend Qt6::Widgets)
target_compile_options(udrms_gui PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

enable_testing()
add_test(NAME backend_tests COMMAND udrms_tests)
add_test(NAME gui_smoke_test COMMAND udrms_gui --smoke-test)
add_test(NAME gui_persistence_test COMMAND udrms_gui --persistence-self-test)
set_tests_properties(gui_persistence_test PROPERTIES TIMEOUT 5)
