cmake_minimum_required(VERSION 3.10)
project(SpeedControllerSILKit)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SIL Kit package
find_package(SilKit REQUIRED)

# Add executable for the SpeedController virtual ECU
add_executable(virtual_ecu
    vECU_Main.cpp
    SpeedController.c
    # Note: ert_main.c is not included as we have our own main in vECU_Main.cpp
)

# Link SIL Kit libraries
target_link_libraries(virtual_ecu
    SilKit::SilKit
)

# Include directories
target_include_directories(virtual_ecu PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Set output directory for the executable
set_target_properties(virtual_ecu PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Optional: Set compiler-specific flags
if(MSVC)
    target_compile_options(virtual_ecu PRIVATE /W4)
else()
    target_compile_options(virtual_ecu PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Install target
install(TARGETS virtual_ecu
    RUNTIME DESTINATION bin
)