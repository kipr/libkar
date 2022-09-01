project(libkar)

cmake_minimum_required(VERSION 2.8.12)

find_package(Qt6 REQUIRED COMPONENTS Core)

set(INCLUDE ${libkar_SOURCE_DIR}/include)
set(SRC ${libkar_SOURCE_DIR}/src)

include_directories(${INCLUDE})

file(GLOB INCLUDES ${INCLUDE}/*.h ${INCLUDE}/*.hpp)
file(GLOB SOURCES ${SRC}/*.c ${SRC}/*.cpp)

set(LIBRARY_OUTPUT_PATH ${libkar_SOURCE_DIR}/lib)
add_library(kar SHARED ${SOURCES} ${INCLUDES})
target_link_libraries(kar Qt6::Core)

if(WIN32 AND CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/../prefix)
endif()

install(FILES ${INCLUDES} DESTINATION include/kar) 
install(TARGETS kar DESTINATION lib) 

if(APPLE)
set(OSX_INSTALL_NAMES_SCRIPT "${libkar_SOURCE_DIR}/scripts/osx_install_names.sh")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -headerpad_max_install_names")
add_custom_target(osx_install_names ALL 
	COMMAND sh ${OSX_INSTALL_NAMES_SCRIPT} ${LIBRARY_OUTPUT_PATH}/libkar.dylib
    ${_qt5Core_install_prefix}/lib
		QtCore.framework/Versions/5/QtCore
	WORKING_DIRECTORY ${LIBRARY_OUTPUT_PATH}
	DEPENDS kar)
endif()

add_subdirectory(tools)
