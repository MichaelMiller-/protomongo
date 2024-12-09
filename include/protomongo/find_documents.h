#pragma once

#include <google/protobuf/message.h>
#include <mongocxx/collection.hpp>

#include "detail/make_protobuf_object.h"

#include <string_view>
#include <vector>

namespace protomongo
{
   //! \brief Finds the documents in this collection which match the provided key-value pair.
   template <typename T>
      requires std::is_base_of_v<google::protobuf::Message, T>
   [[nodiscard]] auto find_documents(mongocxx::collection& collection, std::string_view key,
                                     auto&& value) -> std::vector<T>
   {
      using bsoncxx::builder::basic::kvp;
      using bsoncxx::builder::basic::make_document;

      auto cursor = collection.find(make_document(kvp(key, std::forward<decltype(value)>(value))));

      std::vector<T> result(std::distance(cursor.begin(), cursor.end()));
      std::transform(cursor.begin(), cursor.end(), result.begin(),
                     [](auto const& doc) { return detail::make_protobuf_object<T>(doc); });
      return result;
   }
} // namespace protomongo
