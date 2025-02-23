#include <catch.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>

#include <protomongo/find_documents.h>

#include "movie.pb.h"
#include "theater.pb.h"

#include <string>

using namespace std::string_literals;

mongocxx::instance inst{};

TEST_CASE("Finds documents in a collection", "[protomongo]")
{
   char const* uri = std::getenv("PROTOMONGO_URI_TEST_DATABASE");
   REQUIRE(uri != nullptr);

   mongocxx::client conn{mongocxx::uri{uri}};

   auto db = conn.database("sample_mflix");
   auto movies = db.collection("movies");

   auto result = protomongo::find_documents<example::Movie>(movies, "year", 1920);

   REQUIRE(result.size() == 4);

   // sort result to ensure test stability
   std::sort(begin(result), end(result), [](auto const& lhs, auto const& rhs) { return lhs.year() < rhs.year(); });

   REQUIRE(result[0].title() == "One Week"s);
   REQUIRE(result[1].title() == "High and Dizzy"s);
   REQUIRE(result[2].title() == "The Saphead"s);
   REQUIRE(result[3].title() == "The Daughter of Dawn"s);
}

TEST_CASE("Finds all documents in a collection", "[protomongo]")
{
   char const* uri = std::getenv("PROTOMONGO_URI_TEST_DATABASE");
   REQUIRE(uri != nullptr);

   mongocxx::client conn{mongocxx::uri{uri}};

   auto db = conn.database("sample_mflix");
   auto movies = db.collection("theaters");

   auto result = protomongo::find_documents<example::Theater, std::list>(movies);

   REQUIRE(result.size() == 1564);
}
