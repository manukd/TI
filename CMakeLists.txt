# cmake_minimum_required(VERSION <specify CMake version here>)
project(_TI_TraitementImage)

set(CMAKE_CXX_STANDARD 17)

set(OpenCV_DIR "C:\\opencv\\build\\install")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(main main.cpp)

# add libs you need
set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs)
target_link_libraries(main ${OpenCV_LIBS})