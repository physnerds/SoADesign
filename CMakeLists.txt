cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(soa CXX C)
set(CMAKE_CXX_STANDARD 17)

find_package(TBB REQUIRED)
find_package(MPI REQUIRED)
find_package(ROOT REQUIRED COMPONENTS)
find_package(nlohmann_json)
find_package(HDF5 REQUIRED)

message(STATUS "C++ compiler is :${CMAKE_CXX_COMPILER}")

set(INCLUDE_DIR /opt/homebrew/include)
set(ONEAPI_DIR /opt/homebrew/include/oneapi)
set(TBB_LIB /opt/homebrew/lib/libtbb.so)
#set(HDF5_LIB /opt/homebrew/lib/libhdf5.so)
include_directories (SYSTEM
         ${EXTRA_INCLUDES}
         ${MPI_INCLUDE_DIRS}
         ${MPI_CXX_INCLUDE_DIRS}
             ${INCLUDE_DIR}
             ${ONEAPI_DIR}
         ${ROOT_INCLUDE_DIRS}
             ${EXTRA_INCLUDE_DIR}
                
         )

set (libraries ${libraries}
             ${ROOT_LIBRARIES}
         ${MPI_CXX_LIBRARIES}
         ${HDF5_LIB}
           ${TBB_LIB}
         ROOT::Core
         ROOT::RIO
         ROOT::Tree
         ROOT::Hist
         )
         
add_executable(test_soa Test_SoA.cc)
target_link_libraries(test_soa PRIVATE ${libraries})

