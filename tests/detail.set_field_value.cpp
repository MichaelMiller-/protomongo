#include <catch.hpp>

#include <bsoncxx/builder/basic/document.hpp>
#include <protomongo/detail/set_field_value.h>

#include "user.pb.h"

TEST_CASE("copy a value into any field", "[protomongo]")
{
   using bsoncxx::builder::basic::kvp;

   bsoncxx::builder::basic::document basic_builder{};
   basic_builder.append(kvp("id", std::int64_t{1234}));
   basic_builder.append(kvp("name", "foo bar"));
   basic_builder.append(kvp("cash", 3.14));

   auto doc = basic_builder.extract();
   auto desc = example::User::descriptor();

   SECTION("set user-id")
   {
      example::User result{};
      protomongo::detail::set_field_value(doc["id"], result, *desc->FindFieldByName("id"));
      REQUIRE(result.id() == 1234);
   }
   SECTION("set user-name")
   {
      example::User result{};
      protomongo::detail::set_field_value(doc["name"], result, *desc->FindFieldByName("name"));
      REQUIRE(result.name() == "foo bar");
   }
   SECTION("set user-cash")
   {
      example::User result{};
      protomongo::detail::set_field_value(doc["cash"], result, *desc->FindFieldByName("cash"));
      REQUIRE(result.cash() == 3.14);
   }
}
