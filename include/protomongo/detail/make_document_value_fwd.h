#pragma once

#include <bsoncxx/document/value.hpp>
#include <google/protobuf/message.h>

namespace protomongo::detail
{
   template <typename T>
      requires std::is_base_of_v<google::protobuf::Message, T>
   [[nodiscard]] auto make_document_value(T const& obj) -> bsoncxx::document::value;
}