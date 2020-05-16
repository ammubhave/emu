include(FetchContent)

FetchContent_Declare(
    catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.0.0-preview2
)
FetchContent_MakeAvailable(catch2)

set(SOURCE
    EmuTest.cpp
)

add_executable(EmuTest ${SOURCE})

set_property(TARGET EmuTest PROPERTY CXX_STANDARD 20)
target_link_libraries(EmuTest PRIVATE emu_lib Catch2 Catch2WithMain)
add_test(NAME EmuTestTest COMMAND EmuTest)
