project(libkar)

cmake_minimum_required(VERSION 2.8.0)

find_package(Qt4 REQUIRED)

set(QT_DONT_USE_QTGUI TRUE)

include(${QT_USE_FILE})

set(INCLUDE ${libkar_SOURCE_DIR}/include)
set(SRC ${libkar_SOURCE_DIR}/src)

include_directories(${INCLUDE})

file(GLOB INCLUDES ${INCLUDE}/*.h ${INCLUDE}/*.hpp)
file(GLOB SOURCES ${SRC}/*.c ${SRC}/*.cpp)

set(LIBRARY_OUTPUT_PATH ${libkar_SOURCE_DIR}/lib)
add_library(kar SHARED ${SOURCES} ${INCLUDES})
target_link_libraries(kar ${QT_LIBRARIES})

if(APPLE)
	add_definitions(-arch x86_64)
endif()

if(WIN32)
install(FILES ${INCLUDES} DESTINATION ${CMAKE_SOURCE_DIR}/../prefix/include/kar)
install(TARGETS kar
	ARCHIVE DESTINATION ${CMAKE_SOURCE_DIR}/../prefix/lib
	RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/../prefix/lib)
else()
install(FILES ${INCLUDES} DESTINATION include/kar)
install(TARGETS kar DESTINATION lib)
endif()

if(APPLE)
set(OSX_INSTALL_NAMES_SCRIPT "${libkar_SOURCE_DIR}/scripts/osx_install_names.sh")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -headerpad_max_install_names")
add_custom_target(osx_install_names ALL 
	COMMAND sh ${OSX_INSTALL_NAMES_SCRIPT} ${LIBRARY_OUTPUT_PATH}/libkar.dylib
		QtCore.framework/Versions/4/QtCore
	WORKING_DIRECTORY ${LIBRARY_OUTPUT_PATH}
	DEPENDS kar)
endif()

add_subdirectory(tools)