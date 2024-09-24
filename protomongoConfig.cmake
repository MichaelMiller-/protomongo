include(CMakeFindDependencyMacro)

# TODO: check for version number
find_dependency(bsoncxx)
find_dependency(mongocxx)
find_dependency(Protobuf)

include(${CMAKE_CURRENT_LIST_DIR}/protomongoTargets.cmake)