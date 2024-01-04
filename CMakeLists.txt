cmake_minimum_required(VERSION 3.20)

project(soa CXX C)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#find_package(MPI REQUIRED)
find_package(ROOT REQUIRED COMPONENTS RIO Net OPTIONAL_COMPONENTS)
#find_package(nlohmann_json)


set(CMAKE_SHARED_LIBRARY_SUFFIX ".so")
message(STATUS "C++ compiler is :${CMAKE_CXX_COMPILER}")
message(STATUS "BOOST FOUND IS ${Boost_FOUND}")
message(STATUS "HDF5 FOUND IS ${HDF5_FOUND}")
set(INCLUDE_DIR /opt/homebrew/include)

include_directories (SYSTEM 
         ${PROJECT_SOURCE_DIR}/include
         ${INCLUDE_DIR}
         ${ROOT_INCLUDE_DIRS}
                
         )

set (libraries ${libraries}
             ${ROOT_LIBRARIES}
            ROOT::Core
            ROOT::RIO
            ROOT::Tree
            ROOT::Hist
            ROOT::ROOTNTuple
         )


REFLEX_GENERATE_DICTIONARY(G__dunedatamodel include/DUNETriggerData.h SELECTION ${PROJECT_SOURCE_DIR}/include/dunedatamodel.xml)
add_library(G__dunedatamodelDict SHARED G__dunedatamodel.cxx)
target_link_libraries(G__dunedatamodelDict PUBLIC ${libraries})



add_executable(test_trigger_rntuple
              test_trigger_rntuple.cc)
target_link_libraries(test_trigger_rntuple PRIVATE ${libraries} G__dunedatamodelDict)

add_executable(test_trigger_tree
              test_trigger_tree.cc)
target_link_libraries(test_trigger_tree PRIVATE ${libraries} G__dunedatamodelDict)