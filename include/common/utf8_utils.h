#pragma once
#include <cstddef>
#include <string_view>

inline std::size_t utf8Length(std::string_view s) noexcept {
  std::size_t count = 0;
  for (unsigned char c : s) {
    if ((c & 0xC0) != 0x80)
      ++count;
  }
  return count;
}
