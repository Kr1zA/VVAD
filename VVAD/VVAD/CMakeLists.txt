#for more information see https://github.com/davisking/dlib/blob/master/examples/CMakeLists.txt

cmake_minimum_required(VERSION 2.8.12)

project(examples)

add_subdirectory(../dlib dlib_build)

if (DLIB_NO_GUI_SUPPORT)
    message("No GUI support, so we won't build the testVVAD example.")
else ()
    find_package(OpenCV QUIET)
    if (OpenCV_FOUND)
        include_directories(${OpenCV_INCLUDE_DIRS})
        if (DLIB_NO_GUI_SUPPORT)
            message("No GUI support, so we won't build the ${name} example.")
        else ()
            add_library(VVAD SHARED VVAD.h VVAD.cpp)
            target_link_libraries(VVAD dlib::dlib ${OpenCV_LIBS})
            add_executable(testVVAD testVVAD.cpp)
            target_link_libraries(testVVAD VVAD dlib::dlib ${OpenCV_LIBS})
        endif ()
    else ()
        message("OpenCV not found, so we won't build the testVVAD example.")
    endif ()
endif ()

add_executable(train_shape_predictor_ex train_shape_predictor_ex.cpp)
target_link_libraries(train_shape_predictor_ex dlib::dlib)

set(CMAKE_CXX_FLAGS '-std=c++17')

