#pragma once

#include <bsoncxx/document/view.hpp>
#include <google/protobuf/message.h>

namespace protomongo::detail
{
   template <typename T>
      requires std::is_base_of_v<google::protobuf::Message, T>
   auto fill_protobuf_object(T& obj, bsoncxx::document::view const& doc) -> void;
} // namespace protomongo::detail
