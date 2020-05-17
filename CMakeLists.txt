cmake_minimum_required(VERSION 3.16)

project(emu)

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)
find_package(fmt 6.2.1 REQUIRED)

file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
endif()

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

add_custom_command(
    COMMAND ${PROJECT_SOURCE_DIR}/src/gen.py 
    DEPENDS ${PROJECT_SOURCE_DIR}/src/gen.py
    OUTPUT ${PROJECT_SOURCE_DIR}/src/cpu/InstructionSet.cpp
    COMMENT "Generating code for InstructionSet.cpp"
)

set(SOURCE
    src/cpu/insts/AllInstructions.h
    src/cpu/insts/Cli.cpp
    src/cpu/insts/Cli.h
    src/cpu/insts/Instruction.h
    src/cpu/insts/Jmp.cpp
    src/cpu/insts/Jmp.h
    src/cpu/insts/Mov-inl.h
    src/cpu/insts/Mov.h
    src/cpu/insts/Util.h
    src/cpu/Cpu.cpp
    src/cpu/Cpu.h
    src/cpu/Immediate.cpp
    src/cpu/Immediate.h
    src/cpu/ImmediateOperand.cpp
    src/cpu/ImmediateOperand.h
    src/cpu/InstructionFetcher.cpp
    src/cpu/InstructionFetcher.h
    src/cpu/InstructionSet.cpp
    src/cpu/InstructionSet.h
    src/cpu/MemoryOperand.cpp
    src/cpu/MemoryOperand.h
    src/cpu/ModRm.cpp
    src/cpu/ModRm.h
    src/cpu/ModRmOperand.cpp
    src/cpu/ModRmOperand.h
    src/cpu/Opcode.cpp
    src/cpu/Opcode.h
    src/cpu/RegisterOperand.cpp
    src/cpu/RegisterOperand.h
    src/cpu/SegmentRegisterOperand.cpp
    src/cpu/SegmentRegisterOperand.h
    src/cpu/Types.h
    src/Bus.cpp
    src/Bus.h
    src/GdbServer.h
    src/Ram-inl.h
    src/Ram.h
    src/Registers.cpp
    src/Rom-inl.h
    src/Rom.h
    src/Types.h
)

add_executable(${PROJECT_NAME} src/main.cpp)
add_library(${PROJECT_NAME}_lib ${SOURCE})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
set_property(TARGET ${PROJECT_NAME}_lib PROPERTY CXX_STANDARD 20)
set_property(TARGET ${PROJECT_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
set_property(TARGET ${PROJECT_NAME}_lib PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(${PROJECT_NAME}_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_compile_options(${PROJECT_NAME} PRIVATE 
    -Wall -Wextra -pedantic -Werror
    -Wno-unused-function -Wno-unused-variable -Wno-unused-parameter -Wno-shift-count-overflow -Wno-gnu-anonymous-struct -Wno-nested-anon-types -Wno-unused-private-field)
target_compile_options(${PROJECT_NAME}_lib PRIVATE 
    -Wall -Wextra -pedantic -Werror
    -Wno-unused-function -Wno-unused-variable -Wno-unused-parameter -Wno-shift-count-overflow -Wno-gnu-anonymous-struct -Wno-nested-anon-types -Wno-unused-private-field)

target_link_libraries(${PROJECT_NAME} sfml-graphics ${OPENGL_LIBRARIES} fmt::fmt emu_lib)
target_link_libraries(${PROJECT_NAME}_lib fmt::fmt)

enable_testing()
add_subdirectory(tests)
