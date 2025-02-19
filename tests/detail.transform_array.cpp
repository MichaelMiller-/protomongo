#include <catch.hpp>

#include <bsoncxx/json.hpp>

#include <protomongo/detail/make_document_value.h>
#include <protomongo/detail/transform_array.h>

#include "user.pb.h"

using namespace std::string_view_literals;

TEST_CASE("Transform any given field of type array to a bson::document::value", "[protomongo]")
{
   example::User obj{};
   obj.mutable_tags()->Add("tag1");
   obj.mutable_tags()->Add("tag2");
   obj.mutable_tags()->Add("tag3");

   decltype(auto) desc = obj.GetDescriptor();
   REQUIRE(desc);

   decltype(auto) field = desc->FindFieldByName("tags");
   REQUIRE(field);

   auto result = protomongo::detail::transform_array(obj, *field);
   auto tags = result.view();
   auto expected = std::array{"tag1"sv, "tag2"sv, "tag3"sv};

   REQUIRE(std::equal(tags.begin(), tags.end(), expected.begin(), [](auto const& lhs, auto const& rhs) {
      return static_cast<std::string_view>(lhs.get_string().value) == rhs;
   }));
}
