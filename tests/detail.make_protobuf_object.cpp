#include <catch.hpp>

#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/builder/basic/document.hpp>

#include <protomongo/detail/make_protobuf_object.h>

#include "user.pb.h"

TEST_CASE("Copy a bson::document::view into any given protobuf type", "[protomongo]")
{
   using bsoncxx::builder::basic::kvp;
   using namespace std::string_view_literals;

   bsoncxx::builder::basic::document address{};
   address.append(kvp("zip_code", std::int32_t{32830}), kvp("city", "Lake Buena Vista"),
                  kvp("street", "1375 E Buena Vista Dr"));

   bsoncxx::builder::basic::array tags{};
   tags.append("tag1");
   tags.append("tag2");
   tags.append("tag3");

   bsoncxx::builder::basic::array motivations{};
   motivations.append("High quality, strong coffee");
   motivations.append("Quiet atmosphere for studying");
   motivations.append("On or close to the college campus");

   bsoncxx::builder::basic::array frustrations{};
   frustrations.append("Hates waiting too long for her coffee");
   frustrations.append("Doesn’t want to be bothered by the staff while studying");
   frustrations.append("Actually relies on strong coffee to help her concentrate later in the day");

   bsoncxx::builder::basic::array goals{};
   goals.append("She needs information on the type of coffee a cafe serves, in order to determine the quality.");
   goals.append("She wants to find the menu and the daily specials as quickly and easily as possible.");
   goals.append(
      "She wants to order with her smartphone, avoid the usual line-up, and experience minimal distractions.");

   bsoncxx::builder::basic::document personality_builder{};
   personality_builder.append(kvp("motivations", motivations));
   personality_builder.append(kvp("frustrations", frustrations));
   personality_builder.append(kvp("goals", goals));

   bsoncxx::builder::basic::document linkedin{};
   linkedin.append(kvp("name", "LikedIn"), kvp("rating", std::int32_t{5}));

   bsoncxx::builder::basic::document facebook{};
   facebook.append(kvp("name", "Facebook"), kvp("rating", std::int32_t{1}));

   bsoncxx::builder::basic::array social_media{};
   social_media.append(linkedin);
   social_media.append(facebook);

   bsoncxx::builder::basic::document builder{};
   builder.append(kvp("id", std::int64_t{1234}), kvp("name", "foo bar"), kvp("cash", 3.14));
   builder.append(kvp("address", address));
   builder.append(kvp("tags", tags));
   builder.append(kvp("personality", personality_builder));
   builder.append(kvp("social_media", social_media));

   auto result = protomongo::detail::make_protobuf_object<example::User>(builder.extract().view());

   REQUIRE(result.id() == 1234);
   REQUIRE(result.name() == "foo bar");
   REQUIRE(result.cash() == 3.14);

   REQUIRE(result.address().street() == "1375 E Buena Vista Dr");
   REQUIRE(result.address().city() == "Lake Buena Vista");
   REQUIRE(result.address().zip_code() == 32830);

   auto expected_tags = std::array{"tag1"sv, "tag2"sv, "tag3"sv};
   REQUIRE(result.tags().size() == expected_tags.size());
   REQUIRE(std::equal(result.tags().begin(), result.tags().end(), begin(expected_tags)));

   auto expected_goals = std::array{
      "She needs information on the type of coffee a cafe serves, in order to determine the quality."sv,
      "She wants to find the menu and the daily specials as quickly and easily as possible."sv,
      "She wants to order with her smartphone, avoid the usual line-up, and experience minimal distractions."sv};
   REQUIRE(result.personality().goals().size() == expected_goals.size());
   REQUIRE(std::equal(result.personality().goals().begin(), result.personality().goals().end(), begin(expected_goals)));

   REQUIRE(result.social_media().size() == 2);
   REQUIRE(result.social_media()[0].name() == "LikedIn");
   REQUIRE(result.social_media()[0].rating() == 5);
   REQUIRE(result.social_media()[1].name() == "Facebook");
   REQUIRE(result.social_media()[1].rating() == 1);
}
