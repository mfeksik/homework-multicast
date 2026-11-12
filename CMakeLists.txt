cmake_minimum_required(VERSION 3.10)
project(multicast)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wextra -Wpedantic -fsanitize=address,undefined)
add_link_options(-fsanitize=address,undefined)

add_executable(multicast)
target_sources(multicast PRIVATE
  main.cpp
  config.cpp
  config.hpp
  client.hpp
  udp.hpp
  udp.cpp
  udp_client.hpp
  udp_client.cpp
  serial.hpp
  serial.cpp
  serial_client.hpp
  serial_client.cpp
)

add_executable(multicast_test
        tests/test_multicast.cpp
        client.hpp
        udp.hpp
        udp.cpp
        udp_client.hpp
        udp_client.cpp
        serial.hpp
        serial.cpp
        serial_client.hpp
        serial_client.cpp
)
target_include_directories(multicast_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()
add_test(NAME multicast_test COMMAND multicast_test)
