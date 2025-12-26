cmake_minimum_required(VERSION 3.18)
project(vECU_Demo)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SilKit package or use local source
find_package(SilKit QUIET)
if(NOT SilKit_FOUND)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/sil-kit/CMakeLists.txt")
        message(STATUS "SilKit not found on system, using local source in sil-kit folder")
        # Disable SilKit tests/demos to speed up build
        set(SILKIT_BUILD_TESTS OFF CACHE BOOL "Build SIL Kit tests" FORCE)
        set(SILKIT_BUILD_DEMOS OFF CACHE BOOL "Build SIL Kit demos" FORCE)
        set(SILKIT_BUILD_UTILITIES OFF CACHE BOOL "Build SIL Kit utilities" FORCE)
        add_subdirectory(sil-kit)
    else()
        find_package(SilKit REQUIRED)
    endif()
endif()

# Add SpeedController executable
add_executable(SpeedControllerECU
    SpeedController_ert_rtw/SpeedController.c
    SpeedController_ert_rtw/vECU_Main.cpp
)

# Add VehicleSpeedModel executable
add_executable(VehicleSpeedModelECU
    VehicleSpeedModel_grt_rtw/VehicleSpeedModel.c
    VehicleSpeedModel_grt_rtw/VehicleSpeedModel_data.c
    VehicleSpeedModel_grt_rtw/rt_nonfinite.c
    VehicleSpeedModel_grt_rtw/vECU_PlantModel.cpp
)

# Link SilKit libraries to both executables
target_link_libraries(SpeedControllerECU PRIVATE SilKit::SilKit)
target_link_libraries(VehicleSpeedModelECU PRIVATE SilKit::SilKit)

# Include directories for both executables
target_include_directories(SpeedControllerECU PRIVATE
    SpeedController_ert_rtw
    ${SilKit_INCLUDE_DIRS}
)

target_include_directories(VehicleSpeedModelECU PRIVATE
    VehicleSpeedModel_grt_rtw
    ${SilKit_INCLUDE_DIRS}
)

# Set output directory for executables
set_target_properties(SpeedControllerECU PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

set_target_properties(VehicleSpeedModelECU PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Set properties for Visual Studio
set_target_properties(SpeedControllerECU PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

set_target_properties(VehicleSpeedModelECU PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Add Visual Studio folder grouping
set_property(TARGET SpeedControllerECU PROPERTY FOLDER "vECU")
set_property(TARGET VehicleSpeedModelECU PROPERTY FOLDER "vECU")