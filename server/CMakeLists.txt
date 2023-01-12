cmake_minimum_required(VERSION 3.20)
project(server)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost 1.81.0 REQUIRED)

set(HEADER_FILES
        src/TCPServer.hpp
        src/TCPConnection.hpp
        )

set(SOURCE_FILES
        src/main.cpp
        src/TCPServer.cpp
        src/TCPConnection.cpp
        )

add_executable(${PROJECT_NAME} ${HEARDER_FILES} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME}
        PRIVATE
            ${Boost_INCLUDE_DIRS}
        )
target_link_libraries(${PROJECT_NAME}
        PRIVATE
            ${Boost_LIBRARIES}
)
