cmake_minimum_required(VERSION 3.15)

project(avr_mate_emulator CXX)

# Link this 'library' to set the c++ standard / compile-time options requested
add_library(
        project_options INTERFACE
)
target_compile_features(project_options INTERFACE cxx_std_17)

# standard compiler warnings
add_library(
        project_warnings INTERFACE
)
include(cmake/compiler-warnings.cmake)
set_project_warnings(project_warnings)
include(cmake/standard-project-settings.cmake)

# ------------------------------------------------------------------------------
# setup conan
set(CONAN_EXTRA_REQUIRES ${CONAN_EXTRA_REQUIRES} 
                         gtest/1.10.0
                         imgui-sfml/2.1@bincrafters/stable
                         spdlog/1.8.2
)

if(NOT CONAN_INITIALIZED)
        set(CONAN_INITIALIZED TRUE)
        include(cmake/conan.cmake)
        run_conan()
endif()

add_subdirectory(src)
