#pragma once

#include <bsoncxx/document/value.hpp>
#include <google/protobuf/message.h>

#include "detail/make_document_value.h"

namespace protomongo
{
   //! \brief Creates a ```bsoncxx::document::value``` from any given Protobuf class.
   template <typename T>
      requires std::is_base_of_v<google::protobuf::Message, T>
   [[nodiscard]] auto create_document(T const& obj) -> bsoncxx::document::value
   {
      return detail::make_document_value(obj);
   }
} // namespace protomongo
