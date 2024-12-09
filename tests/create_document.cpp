#include <catch.hpp>

#include <protomongo/create_document.h>

#include "user.pb.h"

using namespace std::string_view_literals;

TEST_CASE("Create a document from any given object", "[protomongo]")
{
   example::User obj{};
   obj.set_id(42);
   obj.set_name("John Doe");
   obj.set_cash(9.81);

   obj.mutable_address()->set_street("1375 E Buena Vista Dr");
   obj.mutable_address()->set_city("Lake Buena Vista");
   obj.mutable_address()->set_state(example::State::FL);
   obj.mutable_address()->set_zip_code(32830);

   obj.mutable_tags()->Add("tag1");
   obj.mutable_tags()->Add("tag2");
   obj.mutable_tags()->Add("tag3");

   obj.mutable_personality()->mutable_motivations()->Add("High quality, strong coffee");
   obj.mutable_personality()->mutable_motivations()->Add("Quiet atmosphere for studying");
   obj.mutable_personality()->mutable_motivations()->Add("On or close to the college campus");

   obj.mutable_personality()->mutable_frustrations()->Add("Hates waiting too long for her coffee");
   obj.mutable_personality()->mutable_frustrations()->Add("Doesn’t want to be bothered by the staff while studying");
   obj.mutable_personality()->mutable_frustrations()->Add("Actually relies on strong coffee to help her concentrate later in the day");

   obj.mutable_personality()->mutable_goals()->Add("She needs information on the type of coffee a cafe serves, in order to determine the quality.");
   obj.mutable_personality()->mutable_goals()->Add("She wants to find the menu and the daily specials as quickly and easily as possible.");
   obj.mutable_personality()->mutable_goals()->Add("She wants to order with her smartphone, avoid the usual line-up, and experience minimal distractions.");

   auto result = protomongo::create_document(obj);

   SECTION("test user-id")
   {
      auto elem = result["id"];
      REQUIRE(elem.get_int64() == 42);
   }
   SECTION("test user-name")
   {
      auto elem = result["name"];
      REQUIRE(elem.get_string().value == "John Doe"sv);
   }
   SECTION("test user-cash")
   {
      auto elem = result["cash"];
      REQUIRE(elem.get_double() == 9.81);
   }
   SECTION("nested address")
   {
      auto elem = result["address"];

      REQUIRE(elem["street"].get_string().value == "1375 E Buena Vista Dr"sv);
      REQUIRE(elem["city"].get_string().value == "Lake Buena Vista"sv);
      REQUIRE(elem["state"].get_int32() == example::State::FL);
      REQUIRE(elem["zip_code"].get_int32() == 32830);
   }
   SECTION("array of tags")
   {
      auto tags = result["tags"].get_array().value;
      auto expected = std::array{"tag1"sv, "tag2"sv, "tag3"sv};

      REQUIRE(std::equal(tags.begin(), tags.end(), expected.begin(),
                         [](auto const& lhs, auto const& rhs) { return lhs.get_string() == rhs; }));
   }
}
