#pragma once

#include <bsoncxx/array/view.hpp>
#include <bsoncxx/document/element.hpp>
#include <bsoncxx/string/to_string.hpp>
#include <google/protobuf/message.h>

#include "convert.h"
#include "fill_protobuf_object_fwd.h"

namespace protomongo::detail
{
   template <typename T>
      requires std::is_base_of_v<google::protobuf::Message, T>
   auto fill_array(T& obj, google::protobuf::FieldDescriptor const& field, bsoncxx::array::view const& array) -> void
   {
      decltype(auto) reflection = obj.GetReflection();

      for (auto const& entry : array) {
         switch (field.cpp_type()) {
         case google::protobuf::FieldDescriptor::CppType::CPPTYPE_INT32: {
            reflection->AddInt32(&obj, &field, entry.get_int32());
            break;
         }
         case google::protobuf::FieldDescriptor::CppType::CPPTYPE_INT64: {
            reflection->AddInt64(&obj, &field, entry.get_int64());
            break;
         }
         case google::protobuf::FieldDescriptor::CppType::CPPTYPE_UINT32: {
            //! \todo design a customization point
            reflection->AddUInt32(&obj, &field,
                                  convert<std::uint32_t>(static_cast<std::string_view>(entry.get_string().value)));
            break;
         }
         case google::protobuf::FieldDescriptor::CppType::CPPTYPE_UINT64: {
            //! \todo design a customization point
            reflection->AddUInt64(&obj, &field,
                                  convert<std::uint64_t>(static_cast<std::string_view>(entry.get_string().value)));
            break;
         }
         case google::protobuf::FieldDescriptor::CppType::CPPTYPE_DOUBLE: {
            reflection->AddDouble(&obj, &field, entry.get_double());
            break;
         }
         case google::protobuf::FieldDescriptor::CppType::CPPTYPE_FLOAT: {
            reflection->AddFloat(&obj, &field, static_cast<float>(entry.get_double()));
            break;
         }
         case google::protobuf::FieldDescriptor::CppType::CPPTYPE_BOOL: {
            reflection->AddBool(&obj, &field, entry.get_bool());
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
            reflection->AddString(&obj, &field, bsoncxx::string::to_string(entry.get_string().value));
            break;
         }
         case google::protobuf::FieldDescriptor::CppType::CPPTYPE_MESSAGE: {
            decltype(auto) item = reflection->AddMessage(&obj, &field);
            fill_protobuf_object(*item, entry.get_document());
            break;
         }
         default:
            throw std::runtime_error{
               std::format("Invalid cpp-type: {} of field: {}", field.cpp_type_name(), field.name())};
         }
      }
   }
} // namespace protomongo::detail
