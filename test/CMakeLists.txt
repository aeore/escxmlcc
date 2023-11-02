CMAKE_MINIMUM_REQUIRED(VERSION 3.7)
project(test)

add_compile_options(-Wall -Wextra -Werror)

add_custom_target(cppunit_autogen
   COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/cppunit/autogen.sh)
      
include(ExternalProject)
ExternalProject_Add(cppunit
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/cppunit
    CONFIGURE_COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/cppunit/configure --prefix=<INSTALL_DIR>
    BUILD_COMMAND ${MAKE})
ExternalProject_Get_Property(cppunit install_dir)
add_dependencies(cppunit cppunit_autogen)

add_custom_target(generate)
   file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/test/src-gen)
   file(GLOB SCXMLS ${CMAKE_SOURCE_DIR}/test/manifests/*.scxml)
   foreach(SCXML ${SCXMLS})
      add_custom_command(
         TARGET generate PRE_BUILD
         COMMAND ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME} -i ${SCXML} -o ${CMAKE_BINARY_DIR}/test/src-gen/)
   endforeach()

add_executable(${PROJECT_NAME} src/main.cpp src/Sequence.cpp src/TestSet.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${install_dir}/include ${CMAKE_BINARY_DIR}/test)
target_link_libraries(${PROJECT_NAME} -L${install_dir}/lib cppunit.a)
add_dependencies(${PROJECT_NAME} ${CMAKE_PROJECT_NAME} cppunit generate)
