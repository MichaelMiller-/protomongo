#pragma once

#include <charconv>
#include <string_view>

namespace protomongo::detail
{
   template <typename T>
   [[nodiscard]] constexpr auto convert(std::string_view str)
   {
      T result{};
      auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), result);
      //! \todo handle error
      return result;
   }
} // namespace protomongo::detail
