#include <catch.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>

#include <protomongo/find_document.h>

#include "movie.pb.h"

TEST_CASE("Finds a single document in a collection", "[protomongo]")
{
   char const* uri = std::getenv("PROTOMONGO_URI_TEST_DATABASE");
   REQUIRE(uri != nullptr);

   mongocxx::instance inst{};
   mongocxx::client conn{mongocxx::uri{uri}};

   auto db = conn.database("sample_mflix");
   auto movies = db.collection("movies");

   auto result = protomongo::find_document<example::Movie>(movies, "title", "The Great Train Robbery");

   REQUIRE(result.has_value() == true);
   REQUIRE(result->title() == "The Great Train Robbery");
   REQUIRE(result->runtime() == 11);
   REQUIRE(result->year() == 1903);
}
