#pragma once

#include <bsoncxx/string/to_string.hpp>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>

#include <format>

namespace protomongo::detail
{
   template <typename T>
      requires std::is_base_of_v<google::protobuf::Message, T>
   auto set_field_value(bsoncxx::document::element const& elem, T& obj, google::protobuf::FieldDescriptor const& field)
   {
      auto reflection = T::GetReflection();

      switch (field.cpp_type()) {
      case google::protobuf::FieldDescriptor::CppType::CPPTYPE_INT32: {
         reflection->SetInt32(&obj, &field, elem.get_int32());
         break;
      }
      case google::protobuf::FieldDescriptor::CppType::CPPTYPE_INT64: {
         reflection->SetInt64(&obj, &field, elem.get_int64());
         break;
      }
      case google::protobuf::FieldDescriptor::CppType::CPPTYPE_DOUBLE: {
         reflection->SetDouble(&obj, &field, elem.get_double());
         break;
      }
      case google::protobuf::FieldDescriptor::CppType::CPPTYPE_STRING: {
         reflection->SetString(&obj, &field, bsoncxx::string::to_string(elem.get_string().value));
         break;
      }
      default:
         throw std::runtime_error{
            std::format("BSON does not support cpp-type: {} of field: {}", field.cpp_type_name(), field.name())};
      }
   }
} // namespace protomongo::detail