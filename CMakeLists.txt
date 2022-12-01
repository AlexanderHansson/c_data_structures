cmake_minimum_required (VERSION 3.24.2)
project(cds)

set (CDS_NAME cds)
set (CDS_MAJOR_VERSION 0)
set (CDS_MINOR_VERSION 0)
set (CDS_VERSION ${CDS_MAJOR_VERSION}.${CDS_MINOR_VERSION})

if (CDS_DEBUG)
message("--------------------")
message("COMPILING WITH DEBUG")
message("--------------------")
set (CMAKE_C_FLAGS "-O0 -g")
else ()
set (CMAKE_C_FLAGS "-O3")
endif()


file(GLOB CDS_SRC
    "src/*.c"
)

# *********** LIBCDS ************
add_library(${CDS_NAME} SHARED ${CDS_SRC})

# see https://cmake.org/cmake/help/latest/command/target_include_directories.html

# build interface, install interface
target_include_directories(${CDS_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}/include)

# linked libraries
# target_link_libraries(${CDS_NAME} PRIVATE )

# versioning
set_target_properties(${CDS_NAME} PROPERTIES VERSION ${CDS_VERSION} SOVERSION ${CDS_MAJOR_VERSION})

## INSTALL ##
install(DIRECTORY "${PROJECT_SOURCE_DIR}/include/cds" DESTINATION "include" COMPONENT dev)

install(
    TARGETS ${CDS_NAME}
    EXPORT ?
    RUNTIME DESTINATION "bin" COMPONENT
    LIBRARY DESTINATION "lib" COMPONENT
    ARCHIVE DESTINATION "lib" COMPONENT
    dev
)
