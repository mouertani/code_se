cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Client/bin)

add_executable(SocketClient Client/src/parse_json.c)
target_include_directories(SocketClient PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Client/inc)
target_link_libraries(SocketClient PRIVATE pthread)
target_link_libraries(SocketClient PRIVATE json-c)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Server/bin)
add_executable(SocketServer Server/src/SendResult.c)
target_include_directories(SocketServer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Server/inc)
target_link_libraries(SocketServer PRIVATE pthread)
