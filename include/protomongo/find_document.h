#pragma once

#include <google/protobuf/message.h>
#include <mongocxx/collection.hpp>

#include "detail/from_bson.h"

#include <optional>
#include <string_view>

namespace protomongo
{
   //! \brief Finds a single document in this collection that match the provided key-value pair.
   template <typename T>
      requires std::is_base_of_v<google::protobuf::Message, T>
   [[nodiscard]] auto find_document(mongocxx::collection& collection, std::string_view key,
                                    auto const& value) -> std::optional<T>
   {
      using bsoncxx::builder::basic::kvp;
      auto doc = collection.find_one(bsoncxx::builder::basic::make_document(kvp(key, value)));
      if (doc) {
         return std::optional<T>{detail::from_bson<T>(*doc)};
      }
      return std::nullopt;
   }
} // namespace protomongo