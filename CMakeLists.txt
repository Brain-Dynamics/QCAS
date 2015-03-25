project(QCAS)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)
mark_as_advanced(VERSION_MAJOR VERSION_MINOR VERSION_PATCH)
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

if(CMAKE_SYSTEM_NAME STREQUAL Linux)
  set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS}")
endif(CMAKE_SYSTEM_NAME STREQUAL Linux)

find_package(Qt4 REQUIRED)
aux_source_directory( src/ SRC_LIST)

file( GLOB_RECURSE INCLUDE_LIST include/*.h )
include_directories( include )

option(BUILD_SHARED "Build libraries shared." OFF)

if(BUILD_SHARED)
  set(LIBRARY_TYPE SHARED)
else(BUILD_SHARED)
  set(LIBRARY_TYPE STATIC)
endif(BUILD_SHARED)

add_library(${PROJECT_NAME} ${LIBRARY_TYPE} ${INCLUDE_LIST} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} Qt4::QtCore Qt4::QtNetwork)

add_executable(example example/example.cpp ${INCLUDE_LIST})
target_link_libraries(example ${PROJECT_NAME})

set_target_properties( ${PROJECT_NAME} example
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

install(TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION lib)

install(DIRECTORY include/ DESTINATION include/
        FILES_MATCHING PATTERN "*.h")


###### BUILD TESTS ######
option(BUILD_TESTS "Build the tests of the library" OFF)
if(BUILD_TESTS)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_INCLUDE_CURRENT_DIR ON)

    set(TEST_DATA_CAS_URL CACHE STRING "A cas server url for the tests")
    set(TEST_DATA_USERNAME CACHE STRING "A cas server username for the tests")
    set(TEST_DATA_PASSWORD CACHE STRING "A cas server password for the tests")
    set(TEST_DATA_SERVICE_URL CACHE STRING "A cas server service for the tests")

    if(TEST_DATA_CAS_URL STREQUAL "")
        message(STATUS "TEST_DATA_CAS_URL at ${TEST_DATA_CAS_URL}")
        message(FATAL_ERROR "Undefined TEST_DATA_CAS_URL env var.")
    endif()

    if(TEST_DATA_USERNAME STREQUAL "")
        message(STATUS "TEST_DATA_USERNAME at ${TEST_DATA_USERNAME}")
        message(FATAL_ERROR "Undefined TEST_DATA_USERNAME env var.")
    endif()

    if(TEST_DATA_PASSWORD STREQUAL "")
        message(STATUS "TEST_DATA_PASSWORD at ${TEST_DATA_PASSWORD}")
        message(FATAL_ERROR "Undefined TEST_DATA_PASSWORD env var.")
    endif()

    if(TEST_DATA_SERVICE_URL STREQUAL "")
        message(STATUS "TEST_DATA_SERVICE_URL at ${TEST_DATA_SERVICE_URL}")
        message(FATAL_ERROR "Undefined TEST_DATA_SERVICE_URL env var.")
    endif()

    # Set the configurations in the tests files
    configure_file(tests/QCASTest.cpp.in
                 ${PROJECT_BINARY_DIR}/tests/QCASTest.cpp  @ONLY IMMEDIATE)

    # Create the executables
    add_executable(QCASTest ${PROJECT_BINARY_DIR}/tests/QCASTest.cpp ${INCLUDE_LIST})
    target_link_libraries(QCASTest ${PROJECT_NAME} Qt4::QtTest)

    set_target_properties(QCASTest
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )

    # Add the test to the project
    enable_testing()
    add_test(testCAST "${CMAKE_BINARY_DIR}/bin/QCASTest" )
endif()



###### BUILD DOCUMENTATION ######
option(BUILD_DOCUMENTATION "Use Doxygen to create the HTML based API documentation" OFF)
if(BUILD_DOCUMENTATION)
  FIND_PACKAGE(Doxygen)
  if (NOT DOXYGEN_FOUND)
    message(FATAL_ERROR
      "Doxygen is needed to build the documentation. Please install it correctly")
  endif()
  #-- Configure the Template Doxyfile for our specific project
  configure_file(doc/Doxyfile.in
                 ${PROJECT_BINARY_DIR}/Doxyfile  @ONLY IMMEDIATE)
  #-- Add a custom target to run Doxygen when ever the project is built
  add_custom_target (Docs ALL
                     COMMAND ${DOXYGEN_EXECUTABLE} ${PROJECT_BINARY_DIR}/Doxyfile
                     SOURCES ${PROJECT_BINARY_DIR}/Doxyfile)
  # IF you do NOT want the documentation to be generated EVERY time you build the project
  # then leave out the 'ALL' keyword from the above command.

endif()
