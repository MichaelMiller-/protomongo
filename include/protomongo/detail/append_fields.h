#pragma once

#include <bsoncxx/builder/stream/document.hpp>

#include "get_field_value.h"

namespace protomongo::detail
{
   template <typename T>
      requires std::is_base_of_v<google::protobuf::Message, T>
   void append_fields(bsoncxx::builder::stream::document& doc, T const& obj)
   {
      auto desc = T::descriptor();
      auto cnt = desc->field_count();
      for (decltype(cnt) i = 0; i < cnt; ++i) {
         auto field = desc->field(i);
         std::visit([&](auto v) { doc << field->name() << v; }, get_field_value(obj, *field));
      }
   }
} // namespace protomongo::detail
