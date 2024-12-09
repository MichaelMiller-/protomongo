#pragma once

#include <bsoncxx/document/view.hpp>
#include <bsoncxx/string/to_string.hpp>
#include <google/protobuf/message.h>

#include "convert.h"
#include "fill_protobuf_object_fwd.h"

#include <format>

namespace protomongo::detail
{
   //! \todo write tests
   template <typename T>
      requires std::is_base_of_v<google::protobuf::Message, T>
   auto set_field_value(T& obj, google::protobuf::FieldDescriptor const& field,
                        bsoncxx::document::element const& elem) -> void
   {
      decltype(auto) reflection = obj.GetReflection();

      switch (field.cpp_type()) {
      case google::protobuf::FieldDescriptor::CppType::CPPTYPE_INT32: {
         reflection->SetInt32(&obj, &field, elem.get_int32());
         break;
      }
      case google::protobuf::FieldDescriptor::CppType::CPPTYPE_INT64: {
         reflection->SetInt64(&obj, &field, elem.get_int64());
         break;
      }
      case google::protobuf::FieldDescriptor::CppType::CPPTYPE_UINT32: {
         //! \todo design a customization point
         reflection->SetUInt32(&obj, &field, convert<std::uint32_t>(elem.get_string().value));
         break;
      }
      case google::protobuf::FieldDescriptor::CppType::CPPTYPE_UINT64: {
         //! \todo design a customization point
         reflection->SetUInt64(&obj, &field, convert<std::uint64_t>(elem.get_string().value));
         break;
      }
      case google::protobuf::FieldDescriptor::CppType::CPPTYPE_DOUBLE: {
         reflection->SetDouble(&obj, &field, elem.get_double());
         break;
      }
      case google::protobuf::FieldDescriptor::CppType::CPPTYPE_FLOAT: {
         reflection->SetFloat(&obj, &field, static_cast<float>(elem.get_double()));
         break;
      }
      case google::protobuf::FieldDescriptor::CppType::CPPTYPE_BOOL: {
         reflection->SetBool(&obj, &field, elem.get_bool());
         break;
      }
#if 0
      case google::protobuf::FieldDescriptor::CppType::CPPTYPE_ENUM: {
         // protobuf enumeration is implicit a int
         //! \todo: reflection->SetEnum(&obj, &field, elem.get_int32());
         break;
      }
#endif
      case google::protobuf::FieldDescriptor::CppType::CPPTYPE_STRING: {
         reflection->SetString(&obj, &field, bsoncxx::string::to_string(elem.get_string().value));
         break;
      }
      case google::protobuf::FieldDescriptor::CppType::CPPTYPE_MESSAGE: {
         decltype(auto) nested_obj = reflection->MutableMessage(&obj, &field);
         fill_protobuf_object(*nested_obj, elem.get_document());
         break;
      }
      default:
         throw std::runtime_error{
            std::format("Invalid cpp-type: {} of field: {}", field.cpp_type_name(), field.name())};
      }
   }
} // namespace protomongo::detail