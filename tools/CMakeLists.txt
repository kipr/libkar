include_directories(${CMAKE_SOURCE_DIR})

add_executable(kissarchive kissarchive.cpp)
target_link_libraries(kissarchive kar)
qt5_use_modules(kissarchive Core)

if(WIN32)
install(TARGETS kissarchive RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/../prefix/bin)
else()
install(TARGETS kissarchive RUNTIME DESTINATION bin)
endif()