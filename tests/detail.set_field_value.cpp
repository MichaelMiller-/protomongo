#include <catch.hpp>

#include <bsoncxx/builder/basic/document.hpp>
#include <protomongo/detail/set_field_value.h>
#include <protomongo/detail/fill_protobuf_object.h>

#include "user.pb.h"

#include <utility>

TEST_CASE("copy a value into any field", "[protomongo]")
{
   using bsoncxx::builder::basic::kvp;

   bsoncxx::builder::basic::document builder{};
   builder.append(kvp("id", std::int64_t{1234}), kvp("name", "foo bar"), kvp("cash", 3.14));

   bsoncxx::builder::basic::document address_builder{};
   address_builder.append(kvp("zip_code", std::int32_t{32830}), kvp("city", "Lake Buena Vista"),
                          kvp("street", "1375 E Buena Vista Dr"));

   builder.append(kvp("address", address_builder));

   decltype(auto) doc = builder.extract();

   example::User obj{};
   decltype(auto) desc = obj.GetDescriptor();

   SECTION("set user-id")
   {
      example::User result{};
      protomongo::detail::set_field_value(result, *desc->FindFieldByName("id"), doc["id"]);
      REQUIRE(result.id() == 1234);
   }
   SECTION("set user-name")
   {
      example::User result{};
      protomongo::detail::set_field_value(result, *desc->FindFieldByName("name"), doc["name"]);
      REQUIRE(result.name() == "foo bar");
   }
   SECTION("set user-cash")
   {
      example::User result{};
      protomongo::detail::set_field_value(result, *desc->FindFieldByName("cash"), doc["cash"]);
      REQUIRE(result.cash() == 3.14);
   }
   SECTION("nested address field")
   {
      example::User result{};
      protomongo::detail::set_field_value(result, *desc->FindFieldByName("address"), doc["address"]);

      REQUIRE(result.address().street() == "1375 E Buena Vista Dr");
      REQUIRE(result.address().city() == "Lake Buena Vista");
      REQUIRE(result.address().zip_code() == 32830);
   }
}