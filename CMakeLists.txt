cmake_minimum_required(VERSION 3.15)
project(ExpressionEvaluationSystem VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include directories
include_directories(include)

# Modular Application Target
add_executable(ExpressionEvaluationSystem_modular
    src/main.cpp
    src/validation.cpp
    src/conversion.cpp
    src/evaluation.cpp
    src/history.cpp
)

# Consolidated Single-File Target (ideal for direct college submission)
add_executable(ExpressionEvaluationSystem
    ExpressionEvaluationSystem.cpp
)

# Automated Test Suite Target
add_executable(test_runner
    tests/test_runner.cpp
    src/validation.cpp
    src/conversion.cpp
    src/evaluation.cpp
    src/history.cpp
)

if (MSVC)
    target_compile_options(ExpressionEvaluationSystem PRIVATE /W4)
    target_compile_options(ExpressionEvaluationSystem_modular PRIVATE /W4)
    target_compile_options(test_runner PRIVATE /W4)
else()
    target_compile_options(ExpressionEvaluationSystem PRIVATE -Wall -Wextra -O2)
    target_compile_options(ExpressionEvaluationSystem_modular PRIVATE -Wall -Wextra -O2)
    target_compile_options(test_runner PRIVATE -Wall -Wextra -O2)
endif()
