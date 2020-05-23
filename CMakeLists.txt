cmake_minimum_required(VERSION 3.17)

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

include(cmake/gen.cmake)
set(SOURCE ${SOURCE}
    src/cpu/insts/AllInstructions.h
    src/cpu/insts/Instruction.h
    src/cpu/insts/Illegal.cpp
    src/cpu/insts/Illegal.h
    src/cpu/insts/Util.h
    src/cpu/operands/ImmediateOperand.cpp
    src/cpu/operands/ImmediateOperand.h
    src/cpu/operands/MemoryOperand.cpp
    src/cpu/operands/MemoryOperand.h
    src/cpu/operands/ModRmOperand.cpp
    src/cpu/operands/ModRmOperand.h
    src/cpu/operands/RegisterOperand.cpp
    src/cpu/operands/RegisterOperand.h
    src/cpu/operands/RelativeOffsetOperand.cpp
    src/cpu/operands/RelativeOffsetOperand.h
    src/cpu/operands/SegmentRegisterOperand.cpp
    src/cpu/operands/SegmentRegisterOperand.h
    src/cpu/Cpu.cpp
    src/cpu/Cpu.h
    src/cpu/Cpu-inl.h
    src/cpu/Immediate.cpp
    src/cpu/Immediate.h
    src/cpu/InstructionDecoder.cpp
    src/cpu/InstructionDecoder.h
    src/cpu/InstructionFetcher.cpp
    src/cpu/InstructionFetcher.h
    src/cpu/InstructionSet.cpp
    src/cpu/InstructionSet.h
    src/cpu/ModRm.cpp
    src/cpu/ModRm.h
    src/cpu/Opcode.cpp
    src/cpu/Opcode.h
    src/cpu/Types.h
    src/peripherals/Cmos.cpp
    src/peripherals/Cmos.h
    src/peripherals/Ps2.cpp
    src/peripherals/Ps2.h
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
