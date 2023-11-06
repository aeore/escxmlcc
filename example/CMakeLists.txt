CMAKE_MINIMUM_REQUIRED(VERSION 3.7)
project(example)

if(CMAKE_COMPILER_IS_GNUCXX)
   add_compile_options(-Wall -Wextra -Werror)
endif()

add_custom_target(generate_example)
   file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/example/src-gen)
   add_custom_command(
      TARGET generate_example PRE_BUILD
      COMMAND ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME} -i ${CMAKE_SOURCE_DIR}/example/example.scxml -o ${CMAKE_BINARY_DIR}/example/src-gen/)

add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR}/example)
add_dependencies(${PROJECT_NAME} ${CMAKE_PROJECT_NAME} generate_example) 
