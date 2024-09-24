cmake_minimum_required(VERSION 3.23)
project(protomongo VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)

## Header-only library
add_library(protomongo INTERFACE)
## Alias
add_library(protomongo::protomongo ALIAS protomongo)

target_include_directories(protomongo INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

find_package(bsoncxx CONFIG REQUIRED)
target_link_libraries(protomongo INTERFACE $<IF:$<TARGET_EXISTS:mongo::bsoncxx_static>,mongo::bsoncxx_static,mongo::bsoncxx_shared>)

find_package(mongocxx CONFIG REQUIRED)
target_link_libraries(protomongo INTERFACE $<IF:$<TARGET_EXISTS:mongo::mongocxx_static>,mongo::mongocxx_static,mongo::mongocxx_shared>)

find_package(Protobuf CONFIG REQUIRED)
target_link_libraries(protomongo INTERFACE protobuf::libprotobuf)

## Testing
##
include(CTest)
if (BUILD_TESTING) # option is created by CTest
    enable_testing()
    add_subdirectory(tests)
endif ()

## Installation + CMake config files
##
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(TARGETS protomongo
        EXPORT protomongoExportTarget
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT protomongoExportTarget
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
        NAMESPACE ${PROJECT_NAME}::
        FILE protomongoTargets.cmake
)

write_basic_package_version_file(
        protomongoConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
)

install(FILES
        protomongoConfig.cmake
        ${PROJECT_BINARY_DIR}/protomongoConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
