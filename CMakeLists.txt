cmake_minimum_required(VERSION 3.20)

project(soa CXX C)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#find_package(MPI REQUIRED)
find_package(ROOT REQUIRED COMPONENTS RIO Net OPTIONAL_COMPONENTS)
#find_package(nlohmann_json)
find_package(HDF5 REQUIRED)
#find_package(Boost REQUIRED)

#set(CMAKE_CXX_FLAGS ${ROOT_CXX_FLAGS})
#set( GCC_Flags_For_CXX "-Wall -fPIC -pthread -Wformat=0 --std=c++0x" )
#set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_Flags_For_CXX}" )
#set( CMAKE_CXX_FLAGS_DEBUG "-ggdb" )

message(STATUS "C++ compiler is :${CMAKE_CXX_COMPILER}")
message(STATUS "BOOST FOUND IS ${Boost_FOUND}")
message(STATUS "HDF5 FOUND IS ${HDF5_FOUND}")
set(INCLUDE_DIR /opt/homebrew/include)
#set(ONEAPI_DIR /opt/homebrew/include/oneapi)
#set(TBB_LIB /opt/homebrew/lib/libtbb.so)
#set(HDF5_LIB /opt/homebrew/lib/libhdf5.so)
include_directories (SYSTEM 
         #${MPI_INCLUDE_DIRS}
         #${MPI_CXX_INCLUDE_DIRS}
         ${HDF5_INCLUDE_DIRS}
         ${PROJECT_SOURCE_DIR}/include
         #${Boost_INCLUDR_DIRS}
         ${INCLUDE_DIR}
         #${ONEAPI_DIR}
         ${ROOT_INCLUDE_DIRS}
         #${EXTRA_INCLUDE_DIR}
                
         )

set (libraries ${libraries}
             ${ROOT_LIBRARIES}
         #${MPI_CXX_LIBRARIES}
         #${Boost_LIBRARIES}
         ${HDF5_LIBRARIES}
        #${TBB_LIB}
         ROOT::Core
         ROOT::RIO
         ROOT::Tree
         ROOT::Hist
         ROOT::ROOTNTuple
         )


REFLEX_GENERATE_DICTIONARY(G__dunedatamodel include/DUNETriggerData.h SELECTION ${PROJECT_SOURCE_DIR}/include/dunedatamodel.xml)# OPTIONS -D__GCCXML__)
add_library(dunedatamodel_dict SHARED G__dunedatamodel.cxx)
target_link_libraries(dunedatamodel_dict PUBLIC ${libraries})

add_executable(test_soa Test_SoA.cc)
target_link_libraries(test_soa PRIVATE ${libraries})

add_executable(test_trigger_rntuple
#               include/DUNETriggerData.cxx 
              test_trigger_rntuple.cc)
target_link_libraries(test_trigger_rntuple PRIVATE ${libraries} dunedatamodel_dict)

add_executable(test_trigger_tree
#               include/DUNETriggerData.cxx 
              test_trigger_tree.cc)
target_link_libraries(test_trigger_tree PRIVATE ${libraries} dunedatamodel_dict)

add_executable(test_trigger_tree_arrSoa
#               include/DUNETriggerData.cxx 
              test_trigger_tree_arrSoa.cc)
target_link_libraries(test_trigger_tree_arrSoa PRIVATE ${libraries} dunedatamodel_dict)

add_executable(test_trigger_rntuple_arrSoa
#               include/DUNETriggerData.cxx 
              test_trigger_rntuple_arrSoa.cc)
target_link_libraries(test_trigger_rntuple_arrSoa PRIVATE ${libraries} dunedatamodel_dict)
