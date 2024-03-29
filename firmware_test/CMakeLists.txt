cmake_minimum_required(VERSION 3.14)
set(projectName firmware_test)
project(${projectName})

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)

set(DOWNLOAD_EXTRACT_TIMESTAMP true)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  DOWNLOAD_EXTRACT_TIMESTAMP true
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest )

set(SOURCES_TEST
    
    ModuleHandlerTest.cpp
    ../Firmware0_1/src/Handlers/ModuleHandler.c
    FreeRTOS_defs/FreeRTOS_FFF_MocksDefinitions.cpp
    ${SRCS}
)

# This makes the test.cpp files able to #include target libraries without errors.  
include_directories(
  ../Firmware0_1/drivers
  ../Firmware0_1/src/Headers
  fff
  FreeRTOS_defs  # Mock and FreeRTOS definitions
)

# Something to 
enable_testing()

add_executable(
  ${projectName}
  ${SOURCES_TEST}
)
target_link_libraries(
  ${projectName}
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(${projectName})