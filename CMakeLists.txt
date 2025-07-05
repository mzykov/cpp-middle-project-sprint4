cmake_minimum_required(VERSION 3.25)
project(analyser VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Configure local githooks
execute_process(
    COMMAND git config core.hooksPath .githooks
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    RESULT_VARIABLE GIT_COMMAND_RESULT
)

find_package(GTest REQUIRED)
find_package(Boost REQUIRED COMPONENTS program_options)
find_package(range-v3 REQUIRED)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
)

set(target analyser)

add_executable(${target} main.cpp)

target_include_directories(${target}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(${target} 
    PRIVATE
        ast_extractor
        metric_accumulator
        metric
        cmd_options
        range-v3::range-v3
)

#
# Тесты
#

# Рекурсивно ищем все используемые в тестах .cpp файлы
file(GLOB TEST_SRC_FILES "${CMAKE_SOURCE_DIR}/tests/*.cpp")

add_executable(${PROJECT_NAME}_tests "${TEST_SRC_FILES}")

target_link_libraries(${PROJECT_NAME}_tests
    PRIVATE
        ast_extractor
        GTest::GTest
        GTest::Main
)

target_include_directories(${PROJECT_NAME}_tests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Включаем тестирование
enable_testing()
add_test(
    NAME analyser_tests
    COMMAND ${PROJECT_NAME}_tests
)

include(CTest)

add_subdirectory(src)
