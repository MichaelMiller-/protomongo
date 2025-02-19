#pragma once

#include <google/protobuf/message.h>
#include <mongocxx/collection.hpp>

#include "detail/make_protobuf_object.h"

#include <optional>
#include <string_view>

namespace protomongo
{
   //! \brief Finds a single document in this collection that match the provided key-value pair.
   template <typename T>
      requires std::is_base_of_v<google::protobuf::Message, T>
   [[nodiscard]] auto find_document(mongocxx::collection& collection, std::string const& key,
                                    auto&& value) -> std::optional<T>
   {
      using bsoncxx::builder::basic::kvp;
      using bsoncxx::builder::basic::make_document;

      auto doc = collection.find_one(make_document(kvp(key, std::forward<decltype(value)>(value))));
      if (doc) {
         return std::optional<T>{detail::make_protobuf_object<T>(*doc)};
      }
      return std::nullopt;
   }
} // namespace protomongo