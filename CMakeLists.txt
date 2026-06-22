cmake_minimum_required(VERSION 3.21)

project(UDRMS LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

add_library(udrms_backend
    backend/src/Person.cpp
    backend/src/Student.cpp
    backend/src/Room.cpp
    backend/src/Restaurant.cpp
    backend/src/Dormitory.cpp
    backend/src/University.cpp
    backend/src/JsonStorage.cpp
)

target_include_directories(udrms_backend PUBLIC backend/include)
target_link_libraries(udrms_backend PUBLIC Qt6::Core)
target_compile_options(udrms_backend PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

add_library(dormora_app_support
    backend/src/AppState.cpp
)

target_include_directories(dormora_app_support PUBLIC backend/include)
target_link_libraries(dormora_app_support PUBLIC udrms_backend Qt6::Core)
target_compile_options(dormora_app_support PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

add_executable(udrms_demo backend/src/main.cpp)
target_link_libraries(udrms_demo PRIVATE udrms_backend)
target_compile_options(udrms_demo PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

add_executable(udrms_tests backend/tests/backend_tests.cpp)
target_link_libraries(udrms_tests PRIVATE udrms_backend)
target_compile_options(udrms_tests PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

add_executable(udrms_gui WIN32 frontend/src/gui_main.cpp frontend/resources/dormora_icons.qrc)
target_link_libraries(udrms_gui PRIVATE dormora_app_support Qt6::Widgets)
target_compile_options(udrms_gui PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

enable_testing()
add_test(NAME backend_tests COMMAND udrms_tests)
add_test(NAME gui_smoke_test COMMAND udrms_gui --smoke-test)
add_test(NAME gui_persistence_test COMMAND udrms_gui --persistence-self-test)
add_test(NAME gui_seed_data_test COMMAND udrms_gui --seed-data-self-test)
add_test(NAME gui_dropdown_test COMMAND udrms_gui --dropdown-self-test)
add_test(NAME gui_loading_screen_test COMMAND udrms_gui --loading-screen-self-test)
add_test(NAME gui_message_box_style_test COMMAND udrms_gui --message-box-style-self-test)
add_test(NAME gui_credential_routing_test COMMAND udrms_gui --credential-routing-self-test)
add_test(NAME gui_admin_permissions_test COMMAND udrms_gui --admin-permissions-self-test)
add_test(NAME gui_admin_account_creator_test COMMAND udrms_gui --admin-account-creator-self-test)
add_test(NAME gui_student_account_workflows_test COMMAND udrms_gui --student-account-workflows-self-test)
add_test(NAME gui_scoped_student_editing_test COMMAND udrms_gui --scoped-student-editing-self-test)
add_test(NAME gui_ui_hardening_test COMMAND udrms_gui --ui-hardening-self-test)
add_test(NAME gui_restaurant_access_test COMMAND udrms_gui --restaurant-access-self-test)
add_test(NAME gui_restaurant_meal_counter_test COMMAND udrms_gui --restaurant-meal-counter-self-test)
add_test(NAME gui_list_filters_test COMMAND udrms_gui --list-filters-self-test)
add_test(NAME gui_portfolio_security_roles_test COMMAND udrms_gui --portfolio-security-roles-self-test)
set_tests_properties(gui_persistence_test PROPERTIES TIMEOUT 5)
set_tests_properties(gui_seed_data_test PROPERTIES TIMEOUT 5)
set_tests_properties(gui_dropdown_test PROPERTIES TIMEOUT 5)
set_tests_properties(gui_loading_screen_test PROPERTIES TIMEOUT 5)
set_tests_properties(gui_message_box_style_test PROPERTIES TIMEOUT 5)
set_tests_properties(gui_credential_routing_test PROPERTIES TIMEOUT 5)
set_tests_properties(gui_admin_permissions_test PROPERTIES TIMEOUT 5)
set_tests_properties(gui_admin_account_creator_test PROPERTIES TIMEOUT 5)
set_tests_properties(gui_student_account_workflows_test PROPERTIES TIMEOUT 5)
set_tests_properties(gui_scoped_student_editing_test PROPERTIES TIMEOUT 5)
set_tests_properties(gui_ui_hardening_test PROPERTIES TIMEOUT 5)
set_tests_properties(gui_restaurant_access_test PROPERTIES TIMEOUT 5)
set_tests_properties(gui_restaurant_meal_counter_test PROPERTIES TIMEOUT 5)
set_tests_properties(gui_list_filters_test PROPERTIES TIMEOUT 5)
set_tests_properties(gui_portfolio_security_roles_test PROPERTIES TIMEOUT 5)
