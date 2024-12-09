#pragma once

#include <bsoncxx/document/view.hpp>
#include <bsoncxx/string/to_string.hpp>
#include <google/protobuf/message.h>

#include "fill_protobuf_object.h"

namespace protomongo::detail
{
   template <typename T>
      requires std::is_base_of_v<google::protobuf::Message, T>
   [[nodiscard]] auto make_protobuf_object(bsoncxx::document::view const& doc) -> T
   {
      T result{};
      fill_protobuf_object(result, doc);
      return result;
   }
} // namespace protomongo::detail
