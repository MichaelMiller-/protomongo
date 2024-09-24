#pragma once

#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>

#include <cstdint>
#include <format>
#include <string>
#include <variant>

namespace protomongo::detail
{
   template <typename T>
      requires std::is_base_of_v<google::protobuf::Message, T>
   auto get_field_value(T const& obj, google::protobuf::FieldDescriptor const& field)
      -> std::variant<std::int32_t, std::int64_t, double, std::string>
   {
      auto reflection = T::GetReflection();

      switch (field.cpp_type()) {
      case google::protobuf::FieldDescriptor::CppType::CPPTYPE_INT32: {
         return reflection->GetInt32(obj, &field);
      }
      case google::protobuf::FieldDescriptor::CppType::CPPTYPE_INT64: {
         return reflection->GetInt64(obj, &field);
      }
      //! \todo CPPTYPE_UINT32 = 3,    // TYPE_UINT32, TYPE_FIXED32
      //! \todo CPPTYPE_UINT64 = 4,    // TYPE_UINT64, TYPE_FIXED64
      case google::protobuf::FieldDescriptor::CppType::CPPTYPE_DOUBLE: {
         return reflection->GetDouble(obj, &field);
      }
      //! \todo CPPTYPE_FLOAT = 6,     // TYPE_FLOAT
      //! \todo CPPTYPE_BOOL = 7,      // TYPE_BOOL
      //! \todo CPPTYPE_ENUM = 8,      // TYPE_ENUM
      case google::protobuf::FieldDescriptor::CppType::CPPTYPE_STRING: {
         return reflection->GetString(obj, &field);
      }
      //! \todo CPPTYPE_MESSAGE = 10,  // TYPE_MESSAGE, TYPE_GROUP
      default:
         throw std::runtime_error{
            std::format("BSON does not support cpp-type: {} of field: {}", field.cpp_type_name(), field.name())};
      }
   }
} // namespace protomongo::detail