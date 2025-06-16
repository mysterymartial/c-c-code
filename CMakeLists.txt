cmake_minimum_required(VERSION 3.16)
project(CppProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Individual executables for each cpp file
add_executable(Thread Thread.cpp)
add_executable(bigboy bigboy.cpp)
add_executable(comOpener comOpener.cpp)
add_executable(mulithread mulithread.cpp)
add_executable(myGuy myGuy.cpp)
add_executable(threadPlay threadPlay.cpp)

# For threading support
find_package(Threads REQUIRED)
target_link_libraries(Thread Threads::Threads)
target_link_libraries(mulithread Threads::Threads)
target_link_libraries(threadPlay Threads::Threads)
