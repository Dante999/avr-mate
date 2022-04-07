cmake_minimum_required(VERSION 3.10)

project(handheld_emulator)

set(AVR_MATE_CORE_DIR ../../core)

add_library(
	${PROJECT_NAME}_core
	${AVR_MATE_CORE_DIR}/src/protocol.c
	${AVR_MATE_CORE_DIR}/src/graphx.c
	${AVR_MATE_CORE_DIR}/src/handheld.c
)
target_include_directories(${PROJECT_NAME}_core PUBLIC ${AVR_MATE_CORE_DIR}/include)


add_executable(
        ${PROJECT_NAME}
        main.cpp #
	handheld_emulator.cpp
)


target_link_libraries(
        ${PROJECT_NAME}
        PUBLIC project_options #
               project_warnings #
	       ${PROJECT_NAME}_core #
               CONAN_PKG::imgui-sfml #
	       CONAN_PKG::spdlog
)
