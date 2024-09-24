#include <catch.hpp>

#include <bsoncxx/builder/basic/document.hpp>

#include <protomongo/detail/from_bson.h>

#include "user.pb.h"

TEST_CASE("copy  bson::document::view into any given type", "[protomongo]")
{
   using bsoncxx::builder::basic::kvp;

   bsoncxx::builder::basic::document basic_builder{};
   basic_builder.append(kvp("id", std::int64_t{1234}));
   basic_builder.append(kvp("name", "foo bar"));
   basic_builder.append(kvp("cash", 3.14));

   auto result = protomongo::detail::from_bson<example::User>(basic_builder.extract().view());

   REQUIRE(result.id() == 1234);
   REQUIRE(result.name() == "foo bar");
   REQUIRE(result.cash() == 3.14);
}
