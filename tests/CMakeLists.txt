add_library(protos OBJECT)
find_package(Protobuf CONFIG REQUIRED)
target_link_libraries(protos PUBLIC protobuf::libprotobuf)
target_include_directories(protos PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

set(input_files
        movie.proto
        user.proto
)
protobuf_generate(
        TARGET protos
        LANGUAGE cpp
        PROTOS ${input_files}
)

##
##
function(test filename)
    set(target_name test.${filename})
    add_executable(${target_name})

    target_sources(${target_name} PRIVATE ${filename}.cpp)

    find_package(Catch2 CONFIG REQUIRED)

    target_link_libraries(${target_name} PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)
    target_link_libraries(${target_name} PRIVATE protos)
    target_link_libraries(${target_name} PRIVATE protomongo::protomongo)

    add_test(
            NAME ${target_name}
            COMMAND $<TARGET_FILE:${target_name}>)
endfunction()

## Unit tests via catch (alphabetical order)
test("create_document")
test("detail.append_fields")
test("detail.from_bson")
test("detail.get_field_value")
test("detail.set_field_value")
test("find_document")
test("find_documents")
