#include <catch.hpp>

#include <protomongo/detail/get_field_value.h>
#include <protomongo/detail/make_document_value.h>

#include "user.pb.h"

TEST_CASE("extract a value of a field", "[protomongo]")
{
   example::User obj{};
   obj.set_id(42);
   obj.set_name("John Doe");
   obj.set_cash(9.81);

   auto desc = example::User::descriptor();

   SECTION("get user-id")
   {
      decltype(auto) field = desc->FindFieldByName("id");
      REQUIRE(field != nullptr);

      auto result = protomongo::detail::get_field_value(obj, *field);
      auto value = std::get<decltype(example::User{}.id())>(result);
      REQUIRE(value == 42);
   }
   SECTION("get user-name")
   {
      decltype(auto) field = desc->FindFieldByName("name");
      REQUIRE(field != nullptr);

      auto result = protomongo::detail::get_field_value(obj, *field);
      auto value = std::get<std::string>(result);
      REQUIRE(value == "John Doe");
   }
   SECTION("get user-cash")
   {
      decltype(auto) field = desc->FindFieldByName("cash");
      REQUIRE(field != nullptr);

      auto result = protomongo::detail::get_field_value(obj, *field);
      auto value = std::get<decltype(example::User{}.cash())>(result);
      REQUIRE(value == 9.81);
   }
}
