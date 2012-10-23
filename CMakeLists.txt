project(libkar)
cmake_minimum_required(VERSION 2.8.0)

find_package(Qt4 REQUIRED)

set(QT_DONT_USE_QTGUI TRUE)
set(TESTS ${libkar_SOURCE_DIR}/tests)

include(${QT_USE_FILE})

set(LIBRARY_OUTPUT_PATH ${libkar_SOURCE_DIR}/lib)
add_library(kar SHARED kar.cpp)
target_link_libraries(kar ${QT_LIBRARIES})

install(FILES kar.hpp DESTINATION include)
install(TARGETS kar DESTINATION lib)

IF(APPLE)
SET(OSX_INSTALL_NAMES_SCRIPT "${libkar_SOURCE_DIR}/scripts/osx_install_names.sh")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -headerpad_max_install_names")
ADD_CUSTOM_TARGET(osx_install_names ALL 
	COMMAND sh ${OSX_INSTALL_NAMES_SCRIPT} ${LIBRARY_OUTPUT_PATH}/libkar.dylib
		QtCore.framework/Versions/4/QtCore
	WORKING_DIRECTORY ${LIBRARY_OUTPUT_PATH}
	DEPENDS kar)
ENDIF(APPLE)

add_subdirectory(tests)
add_subdirectory(tools)