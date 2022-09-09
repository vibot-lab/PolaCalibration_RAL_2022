cmake_minimum_required(VERSION 3.0.2)
project("Camera basler custom GUI app")

# OpenCV Configuration
find_package(OpenCV 4 REQUIRED)

set(PROGRAM_NAME polarimetric_calibration)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
add_compile_options(-std=c++11)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -pthread -std=c++0x -Wall -Wextra")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CPP_FILES
    main.cpp
    Src/LoadSaveExperiments/LoadSaveExperiments.cpp
    Src/PixelGainCalibration/SourceOrientationEstimator.cpp
    Src/PixelGainCalibration/SuperPixelCalibration.cpp
    Src/PolarimetricImagesProcessing/PolarimetricImagesProcessing.cpp
)
set(HPP_FILES
    Inc/CameraTypes.hpp
    Inc/LoadSaveExperiments/LoadSaveExperiments.hpp
    Inc/PixelGainCalibration/SourceOrientationEstimator.hpp
    Inc/PixelGainCalibration/SuperPixelCalibration.hpp
    Inc/PolarimetricImagesProcessing/PolarimetricImagesProcessing.hpp
)

set(SOURCES
    ${CPP_FILES}
    ${HPP_FILES}
)

## We define a variable that will be available in the CPP program. This way
## we do not depend where the binary is installed.
add_definitions(-DPROJECT_PATH="${CMAKE_CURRENT_SOURCE_DIR}/")

# Includes DIR of our project
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/Inc
)

add_executable(${PROGRAM_NAME} ${SOURCES})

# Common executable link libraries
target_link_libraries(${PROGRAM_NAME}
    stdc++fs
    opencv_core
    opencv_imgcodecs
    opencv_highgui
    opencv_videoio
    opencv_imgproc
)