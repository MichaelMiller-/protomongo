#include <catch.hpp>

#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/builder/basic/document.hpp>

#include <protomongo/detail/fill_array.h>
#include <protomongo/detail/fill_protobuf_object.h>

#include "user.pb.h"

TEST_CASE("Copy a bson::document::view into any given protobuf type", "[protomongo]")
{
   using bsoncxx::builder::basic::kvp;
   using namespace std::string_view_literals;

   bsoncxx::builder::basic::array tags{};
   tags.append("tag1");
   tags.append("tag2");
   tags.append("tag3");

   bsoncxx::builder::basic::document builder{};
   builder.append(kvp("tags", tags));

   example::User result{};

   decltype(auto) desc = result.GetDescriptor();
   REQUIRE(desc);

   decltype(auto) field = desc->FindFieldByName("tags");
   REQUIRE(field);

   protomongo::detail::fill_array(result, *field, tags.view());

   auto expected_tags = std::array{"tag1"sv, "tag2"sv, "tag3"sv};
   REQUIRE(result.tags().size() == expected_tags.size());
   REQUIRE(std::equal(result.tags().begin(), result.tags().end(), begin(expected_tags)));
}
