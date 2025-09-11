cmake_minimum_required(VERSION 3.16)
project(dctcp_ns3 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)

# ns-3 installs pkgconfig files per module, aggregate needed modules for TCP examples
pkg_check_modules(NS3 REQUIRED IMPORTED_TARGET \
  libns3.41-core \
  libns3.41-network \
  libns3.41-internet \
  libns3.41-point-to-point \
  libns3.41-applications)

add_executable(sim src/main.cpp)
target_link_libraries(sim PRIVATE PkgConfig::NS3)
