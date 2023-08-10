cmake_minimum_required(VERSION 3.25)
project(Basic_Win32_Application)

set(CMAKE_CXX_STANDARD 23)

add_executable(Basic_Win32_Application main.cpp ButtonManager.cpp ButtonManager.h
        WindowProcHandler.cpp
        WindowProcHandler.h
        PointerManager.cpp
        PointerManager.h
        RandomColorGenerator.cpp
        RandomColorGenerator.h
        Resources.rc
)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")

configure_file(${CMAKE_SOURCE_DIR}/icon.ico ${CMAKE_BINARY_DIR}/icon.ico COPYONLY)

if (WIN32)
    set_target_properties(Basic_Win32_Application PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

target_link_libraries(Basic_Win32_Application dwmapi)