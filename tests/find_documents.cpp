#include <catch.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>

#include <protomongo/find_documents.h>

#include "movie.pb.h"

TEST_CASE("Finds documents in a collection", "[protomongo]")
{
   char const* uri = std::getenv("PROTOMONGO_URI_TEST_DATABASE");
   REQUIRE(uri != nullptr);

   mongocxx::instance inst{};
   mongocxx::client conn{mongocxx::uri{uri}};

   auto db = conn.database("sample_mflix");
   auto movies = db.collection("movies");

   auto result = protomongo::find_documents<example::Movie>(movies, "year", 1920);

   REQUIRE(result.size() == 4);
}
