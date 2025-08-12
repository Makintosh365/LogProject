#pragma once
#include "common/log_level.h"
#include <ctime>
#include <regex>
#include <string>
#include <string_view>

// Разделитель между заголовком и текстом сообщения.
inline constexpr std::string_view kHeaderDelimiter = "] : ";

// Регулярное выражение для парсинга заголовка: [YYYY-MM-DD HH:MM:SS] [level]
inline const std::regex &headerRegex() {
  static const std::regex kPattern(
      R"(\[\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}\] \[(low|medium|high|unknown)\])",
      std::regex::ECMAScript);
  return kPattern;
}

// Извлекает уровень логирования и позицию начала текста из сообщения.
inline bool parseHeader(const std::string &message, LogLevel &outLevel,
                        size_t &outTextStart) {
  std::smatch m;
  if (!std::regex_search(message, m, headerRegex()) || m.size() < 2)
    return false;
  outLevel = convertImportance(m[1].str());
  const auto pos = message.find(kHeaderDelimiter);
  if (pos == std::string::npos)
    return false;
  outTextStart = pos + kHeaderDelimiter.size();
  return true;
}

inline std::string createMessage(std::string_view message, LogLevel level) {

  // Создаёт сообщение с временной меткой, уровнем и текстом.
  std::string finalMessage;
  std::time_t now = std::time(nullptr);
  std::tm tm_buf;

#ifdef _WIN32
  localtime_s(&tm_buf, &now); // Windows:
#else
  localtime_r(&now, &tm_buf); // Linux/macOS:
#endif

  char buf[20];
  std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm_buf);

  finalMessage.reserve(message.size() + 64);
  finalMessage += " [";
  finalMessage += buf;
  finalMessage += "] [";
  finalMessage += toString(level);
  finalMessage += "] : ";
  finalMessage.append(message.data(),
                      message.size()); // <-- string_view -> string
  finalMessage.push_back('\n');

  return finalMessage;
}

// Создаёт сообщение с уровнем unknown.
inline std::string createMessage(std::string_view message) {
  return createMessage(message, LogLevel::unknown);
}