#pragma once

#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>

#include <cstdint>
#include <format>
#include <string>
#include <variant>

#include "make_document_value_fwd.h"

namespace protomongo::detail
{
   template <typename T>
      requires std::is_base_of_v<google::protobuf::Message, T>
   [[nodiscard]] auto get_field_value(T const& obj, google::protobuf::FieldDescriptor const& field)
      -> std::variant<std::int32_t, std::int64_t, double, std::string, bsoncxx::document::value>
   {
      decltype(auto) reflection = obj.GetReflection();

      switch (field.cpp_type()) {
      case google::protobuf::FieldDescriptor::CppType::CPPTYPE_INT32: {
         return reflection->GetInt32(obj, &field);
      }
      case google::protobuf::FieldDescriptor::CppType::CPPTYPE_INT64: {
         return reflection->GetInt64(obj, &field);
      }
      case google::protobuf::FieldDescriptor::CppType::CPPTYPE_UINT32: {
         // BSON does not support 'uint32_t'. see: https://bsonspec.org
         // -> therefore convert value to a string
         //! \todo design a customization point
         return std::to_string(reflection->GetUInt32(obj, &field));
      }
      case google::protobuf::FieldDescriptor::CppType::CPPTYPE_UINT64: {
         // BSON does not support 'uint32_t'. see: https://bsonspec.org
         // -> therefore convert value to a string
         //! \todo design a customization point
         return std::to_string(reflection->GetUInt64(obj, &field));
      }
      case google::protobuf::FieldDescriptor::CppType::CPPTYPE_DOUBLE: {
         return reflection->GetDouble(obj, &field);
      }
      case google::protobuf::FieldDescriptor::CppType::CPPTYPE_FLOAT: {
         // explicit cast to double, because BSON does not support float
         return static_cast<double>(reflection->GetFloat(obj, &field));
      }
      case google::protobuf::FieldDescriptor::CppType::CPPTYPE_BOOL: {
         // implicit conversion to int
         return reflection->GetBool(obj, &field);
      }
      case google::protobuf::FieldDescriptor::CppType::CPPTYPE_ENUM: {
         // returns a int
         return reflection->GetEnumValue(obj, &field);
      }
      case google::protobuf::FieldDescriptor::CppType::CPPTYPE_STRING: {
         return reflection->GetString(obj, &field);
      }
      case google::protobuf::FieldDescriptor::CppType::CPPTYPE_MESSAGE: {
         return make_document_value(reflection->GetMessage(obj, &field));
      }
      default:
         throw std::runtime_error{
            std::format("Invalid cpp-type: {} of field: {}", field.cpp_type_name(), field.name())};
      }
   }
} // namespace protomongo::detail