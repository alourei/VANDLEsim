cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(VANDLESim)

# GEANT4 core
find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

# CADMesh, form example
# find_package(cadmesh)
# include_directories(${CADMESH_INCLUDE_DIRS})

# root
find_package(ROOT)
message(${ROOT_INCLUDE_DIRS})
include_directories(${ROOT_INCLUDE_DIRS})



set(CMAKE_BUILD_TYPE release)

# User code
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

add_executable(VANDLESim VANDLESim.cc ${sources} ${headers})
target_link_libraries(VANDLESim ${Geant4_LIBRARIES})
#target_link_libraries(VANDLESim ${cadmesh_LIBRARIES})
target_link_libraries(VANDLESim ${ROOT_LIBRARIES})



install(TARGETS VANDLESim DESTINATION ${PROJECT_SOURCE_DIR})


