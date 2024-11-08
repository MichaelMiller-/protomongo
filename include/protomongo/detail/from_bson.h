#pragma once

#include <bsoncxx/document/view.hpp>
#include <google/protobuf/message.h>

#include "set_field_value.h"

namespace protomongo::detail
{
   template <typename T>
      requires std::is_base_of_v<google::protobuf::Message, T>
   [[nodiscard]] auto from_bson(bsoncxx::document::view const& doc) -> T
   {
      decltype(auto) desc = T::descriptor();
      decltype(auto) cnt = desc->field_count();

      T result{};

      for (decltype(cnt) i = 0; i < cnt; ++i) {
         decltype(auto) field = desc->field(i);
         decltype(auto) elem = doc[field->name()];

         if (elem) {
            set_field_value(elem, result, *field);
         }
      }

      return result;
   }
} // namespace protomongo::detail
