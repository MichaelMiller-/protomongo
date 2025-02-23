#pragma once

#include <google/protobuf/message.h>
#include <mongocxx/collection.hpp>

#include "detail/make_protobuf_object.h"

#include <string_view>
#include <vector>

namespace protomongo
{
   //! \brief Finds the documents in this collection which match the provided key-value pair.
   template <typename T, template <typename...> typename ReturnType = std::vector>
      requires std::is_base_of_v<google::protobuf::Message, T>
   [[nodiscard]] auto find_documents(mongocxx::collection& collection, std::string const& key,
                                     auto&& value) -> ReturnType<T>
   {
      using bsoncxx::builder::basic::kvp;
      using bsoncxx::builder::basic::make_document;

      auto cursor = collection.find(make_document(kvp(key, std::forward<decltype(value)>(value))));
      //! \note: cursor can only be iterated once
      ReturnType<T> result{};
      std::transform(cursor.begin(), cursor.end(), std::back_inserter(result),
                     [](auto const& doc) { return detail::make_protobuf_object<T>(doc); });
      return result;
   }

   //! \brief Finds all documents in a collection
   template <typename T, template <typename...> typename ReturnType = std::vector>
      requires std::is_base_of_v<google::protobuf::Message, T>
   [[nodiscard]] auto find_documents(mongocxx::collection& collection) -> ReturnType<T>
   {
      using bsoncxx::builder::basic::kvp;
      using bsoncxx::builder::basic::make_document;

      decltype(auto) cursor = collection.find({});
      //! \note: cursor can only be iterated once
      ReturnType<T> result{};
      std::transform(cursor.begin(), cursor.end(), std::back_inserter(result),
                     [](auto const& doc) { return detail::make_protobuf_object<T>(doc); });
      return result;
   }
} // namespace protomongo
