#pragma once

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/document/value.hpp>
#include <google/protobuf/message.h>

#include "detail/append_fields.h"

namespace protomongo
{
   //! \brief Creates a ```bsoncxx::document::value``` from any given Protobuf class.
   template <typename T>
      requires std::is_base_of_v<google::protobuf::Message, T>
   [[nodiscard]] auto create_document(T const& obj) -> bsoncxx::document::value
   {
      bsoncxx::builder::stream::document doc{};
      detail::append_fields(doc, obj);
      return doc << bsoncxx::builder::stream::finalize;
   }
} // namespace protomongo
