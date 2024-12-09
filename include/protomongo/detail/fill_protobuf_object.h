#pragma once

#include <bsoncxx/document/view.hpp>
#include <google/protobuf/message.h>

#include "fill_array.h"
#include "set_field_value.h"

namespace protomongo::detail
{
   template <typename T>
      requires std::is_base_of_v<google::protobuf::Message, T>
   auto fill_protobuf_object(T& obj, bsoncxx::document::view const& doc) -> void
   {
      decltype(auto) desc = obj.GetDescriptor();
      decltype(auto) count = desc->field_count();

      for (decltype(count) i = 0; i < count; ++i) {
         decltype(auto) field = desc->field(i);
         decltype(auto) elem = doc[field->name()];

         if (elem) {
            if (field->is_repeated()) {
               fill_array(obj, *field, elem.get_array().value);
            } else {
               set_field_value(obj, *field, elem);
            }
         }
      }
   }
} // namespace protomongo::detail
