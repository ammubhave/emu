include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.10.0
)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

set(SOURCE
    insts/Cli.cpp
    EmuTest.cpp
    EmuTest.h
)

add_executable(EmuTest ${SOURCE})
set_property(TARGET EmuTest PROPERTY CXX_STANDARD 20)
target_include_directories(EmuTest PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(EmuTest PRIVATE emu_lib gtest gtest_main)

gtest_add_tests(TARGET      EmuTest
                TEST_SUFFIX .noArgs
                TEST_LIST   noArgsTests
)
