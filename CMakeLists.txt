CMAKE_MINIMUM_REQUIRED(VERSION 3.7)
project(escxmlcc)

if(CMAKE_COMPILER_IS_GNUCXX)
   add_compile_options(-Wall -Wextra -Werror)
endif()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(Boost_NO_BOOST_CMAKE ON)
set(Boost_USE_STATIC_LIBS ON)  
set(Boost_USE_STATIC_RUNTIME ON)
find_package(Boost 1.45.0 COMPONENTS system filesystem program_options REQUIRED) 

add_executable(${PROJECT_NAME} ${Boost_INCLUDE_DIRS} src/main.cpp src/cpp_output.cpp src/scxml_parser.cpp src/version.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${Boost_LIBRARIES})
install(TARGETS ${PROJECT_NAME} DESTINATION bin)

option(WITH_TESTS "Build generator with tests" OFF)
if (WITH_TESTS)
   add_subdirectory(test)
endif()

option(WITH_EXAMPLES "Build generator with examples" OFF)
if (WITH_EXAMPLES)
   add_subdirectory(example)
endif()
