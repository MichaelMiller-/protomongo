#pragma once

#include <bsoncxx/builder/basic/array.hpp>

#include "make_document_value_fwd.h"

namespace protomongo::detail
{
   template <typename T>
      requires std::is_base_of_v<google::protobuf::Message, T>
   [[nodiscard]] auto transform_array(T const& obj,
                                      google::protobuf::FieldDescriptor const& field) -> bsoncxx::array::value
   {
      decltype(auto) reflection = obj.GetReflection();
      decltype(auto) count = reflection->FieldSize(obj, &field);

      bsoncxx::builder::basic::array result{};

      for (decltype(count) i = 0; i < count; ++i) {
         switch (field.cpp_type()) {
         case google::protobuf::FieldDescriptor::CppType::CPPTYPE_INT32: {
            result.append(reflection->GetRepeatedInt32(obj, &field, i));
            break;
         }
         case google::protobuf::FieldDescriptor::CppType::CPPTYPE_INT64: {
            result.append(reflection->GetRepeatedInt64(obj, &field, i));
            break;
         }
         case google::protobuf::FieldDescriptor::CppType::CPPTYPE_UINT32: {
            // BSON does not support 'uint32_t'. see: https://bsonspec.org
            // -> therefore convert value to a string
            //! \todo design a customization point
            result.append(std::to_string(reflection->GetRepeatedUInt32(obj, &field, i)));
            break;
         }
         case google::protobuf::FieldDescriptor::CppType::CPPTYPE_UINT64: {
            // BSON does not support 'uint64_t'. see: https://bsonspec.org
            // -> therefore convert value to a string
            //! \todo design a customization point
            result.append(std::to_string(reflection->GetRepeatedUInt64(obj, &field, i)));
            break;
         }
         case google::protobuf::FieldDescriptor::CppType::CPPTYPE_DOUBLE: {
            result.append(reflection->GetRepeatedDouble(obj, &field, i));
            break;
         }
         case google::protobuf::FieldDescriptor::CppType::CPPTYPE_FLOAT: {
            // implicit cast to double bcause BSON does not support float
            result.append(reflection->GetRepeatedFloat(obj, &field, i));
            break;
         }
         case google::protobuf::FieldDescriptor::CppType::CPPTYPE_BOOL: {
            result.append(reflection->GetRepeatedBool(obj, &field, i));
            break;
         }
         case google::protobuf::FieldDescriptor::CppType::CPPTYPE_ENUM: {
            result.append(reflection->GetRepeatedEnumValue(obj, &field, i));
            break;
         }
         case google::protobuf::FieldDescriptor::CppType::CPPTYPE_STRING: {
            result.append(reflection->GetRepeatedString(obj, &field, i));
            break;
         }
         case google::protobuf::FieldDescriptor::CppType::CPPTYPE_MESSAGE: {
            result.append(make_document_value(reflection->GetRepeatedMessage(obj, &field, i)));
            break;
         }
         default:
            throw std::runtime_error{std::format("Invalid cpp-type: {}", field.cpp_type_name())};
         }
      }
      return result.extract();
   }
} // namespace protomongo::detail
