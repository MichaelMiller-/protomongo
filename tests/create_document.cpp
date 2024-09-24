#include <catch.hpp>

#include <bsoncxx/string/to_string.hpp>

#include <protomongo/create_document.h>

#include "user.pb.h"

TEST_CASE("Create a document from any given object", "[protomongo]")
{
   example::User obj{};
   obj.set_id(42);
   obj.set_name("John Doe");
   obj.set_cash(9.81);

   auto result = protomongo::create_document(obj);

   SECTION("test user-id")
   {
      auto elem = result["id"];
      REQUIRE(elem.get_int64() == 42);
   }
   SECTION("test user-name")
   {
      auto elem = result["name"];
      REQUIRE(bsoncxx::string::to_string(elem.get_string().value) == "John Doe");
   }
   SECTION("test user-cash")
   {
      auto elem = result["cash"];
      REQUIRE(elem.get_double() == 9.81);
   }
}
