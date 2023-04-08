cmake_minimum_required(VERSION 3.25 FATAL_ERROR)
project(OlgaStupak)
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable(OlgaStupak Ishodniki/main.cpp)
target_link_libraries( OlgaStupak ${OpenCV_LIBS} )

#find_package (mosquitto REQUIRED COMPONENTS CXX_LIB)
include_directories (C:/Users/Olga/source/repos/OlgaStupak/Ishodniki/lib/mosquitto/include)
target_link_libraries(OlgaStupak C:/Users/Olga/source/repos/OlgaStupak/Ishodniki/lib/mosquitto/build/src/Debug/mosquitto.lib)

#include_directories(C:\Users\Olga\source\repos\OlgaStupak\Ishodniki\lib\mosquitto)
#target_link_libraries(C:\Users\Olga\source\repos\OlgaStupak\Ishodniki\lib\mosquitto)