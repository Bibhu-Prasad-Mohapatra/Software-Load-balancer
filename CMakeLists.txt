cmake_minimum_required(VERSION 3.10)
project(SoftwareLoadBalancer VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -pthread")

include_directories(include)

# Backend Simulator
add_executable(backend_server backend/backend_server.cpp)
target_link_libraries(backend_server pthread)

# Client Simulator
add_executable(client_simulator client/client_simulator.cpp)
target_link_libraries(client_simulator pthread)

# Load Balancer Core
set(LB_SOURCES 
    src/main.cpp 
    src/routing/router.cpp
    src/health/health_manager.cpp
)
add_executable(load_balancer ${LB_SOURCES})
target_link_libraries(load_balancer pthread)
