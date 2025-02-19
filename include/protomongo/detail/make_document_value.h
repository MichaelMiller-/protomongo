#pragma once

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/document/value.hpp>

#include <google/protobuf/message.h>

#include "get_field_value.h"
#include "transform_array.h"

namespace protomongo::detail
{
   template <typename T>
      requires std::is_base_of_v<google::protobuf::Message, T>
   [[nodiscard]] auto make_document_value(T const& obj) -> bsoncxx::document::value
   {
      decltype(auto) desc = obj.GetDescriptor();
      decltype(auto) count = desc->field_count();

      bsoncxx::builder::basic::document builder{};

      for (decltype(count) i = 0; i < count; ++i) {
         decltype(auto) field = desc->field(i);

         if (field->is_repeated()) {
            builder.append(bsoncxx::builder::basic::kvp(field->name(), transform_array(obj, *field)));
         } else {
            std::visit([&](auto v) { builder.append(bsoncxx::builder::basic::kvp(field->name(), std::move(v))); },
                       get_field_value(obj, *field));
         }
      }
      return builder.extract();
   }
} // namespace protomongo::detail