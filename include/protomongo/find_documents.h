#pragma once

#include <google/protobuf/message.h>
#include <mongocxx/collection.hpp>

#include "detail/from_bson.h"

#include <string_view>
#include <vector>

namespace protomongo
{
   //! \brief Finds the documents in this collection which match the provided key-value pair.
   template <typename T>
      requires std::is_base_of_v<google::protobuf::Message, T>
   [[nodiscard]] auto find_documents(mongocxx::collection& collection, std::string_view key,
                                     auto const& value) -> std::vector<T>
   {
      using bsoncxx::builder::basic::kvp;
      auto cursor = collection.find(bsoncxx::builder::basic::make_document(kvp(key, value)));
      // cursor has no size, therefore we cannot pre-allocate memory, unless we use std::distance
      std::vector<T> result{};
      std::transform(cursor.begin(), cursor.end(), back_inserter(result),
                     [](auto const& doc) { return detail::from_bson<T>(doc); });
      return result;
   }
} // namespace protomongo
