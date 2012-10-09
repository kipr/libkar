project(libkar)
cmake_minimum_required(VERSION 2.8.0)

find_package(Qt4 REQUIRED)

set(QT_DONT_USE_QTGUI TRUE)

include(${QT_USE_FILE})

set(LIBRARY_OUTPUT_PATH ${libkar_SOURCE_DIR}/lib)
add_library(kar SHARED kar.cpp)
target_link_libraries(kar ${QT_LIBRARIES})

install(FILES kar.hpp DESTINATION include)
install(TARGETS kar DESTINATION lib)