#pragma once
#include <string>
#include <string_view>

enum class LogLevel { unknown, low, medium, high };

// Преобразует уровень в строку.
inline std::string toString(LogLevel level) {
  switch (level) {
  case LogLevel::low:
    return "low";
  case LogLevel::medium:
    return "medium";
  case LogLevel::high:
    return "high";
  default:
    return "unknown";
  }
}

// Конвертирует строковое представление важности в LogLevel.
inline LogLevel convertImportance(std::string importance) {
  if (importance.empty())
    return LogLevel::unknown;

  switch (std::tolower(importance[0])) {
  case '0':
  case 'l':
    return LogLevel::low;

  case '1':
  case 'm':
    return LogLevel::medium;

  case '2':
  case 'h':
    return LogLevel::high;
  default:
    return LogLevel::unknown;
  }
}
