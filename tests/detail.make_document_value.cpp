#include <catch.hpp>

#include <protomongo/detail/make_document_value.h>

#include "user.pb.h"

using namespace std::string_view_literals;

TEST_CASE("Make a bson::document from any given protobuf object", "[protomongo]")
{
   example::User obj{};
   obj.set_id(42);
   obj.set_name("John Doe");
   obj.set_cash(9.81);

   auto result = protomongo::detail::make_document_value(obj);

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
}
