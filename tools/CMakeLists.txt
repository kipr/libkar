include_directories(${CMAKE_SOURCE_DIR})

add_executable(kissarchive kissarchive.cpp)
target_link_libraries(kissarchive kar)
target_link_libraries(kissarchive Qt6::Core)

install(TARGETS kissarchive RUNTIME DESTINATION bin)
