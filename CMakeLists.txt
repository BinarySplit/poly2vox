cmake_minimum_required(VERSION 2.6)
project(poly2vox)

add_executable(poly2vox src/main.cpp)

target_compile_options(poly2vox PUBLIC -std=c++11 -ggdb -o0)
target_include_directories(poly2vox PUBLIC src)

#dependencies
target_link_libraries(poly2vox PUBLIC
  libassimp.so
  # here you can add any library dependencies
)


set_target_properties( poly2vox
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
install(TARGETS poly2vox RUNTIME DESTINATION bin)
