#include "all_tests.h"
#include "common/log_level.h"
#include "common/message_format.h"
#include <cassert>
#include <vector>
// Все уровни
void test_message_format_parseHeader_valid_low() {
  // Arrange
  std::string message = "[2025-01-02 01:02:03] [low] : hello\n";
  LogLevel levelMessage;
  size_t messageoutTextStart;
  bool check;
  // Act
  check = parseHeader(message, levelMessage, messageoutTextStart);
  // Assert
  assert(check);
  assert(levelMessage == LogLevel::low);
  assert(message.substr(messageoutTextStart) == "hello\n");
}
void test_message_format_parseHeader_valid_medium() {
  // Arrange
  std::string message = "[2025-01-02 01:02:03] [medium] : hello\n";
  LogLevel levelMessage;
  size_t messageoutTextStart;
  bool check;
  // Act
  check = parseHeader(message, levelMessage, messageoutTextStart);
  // Assert
  assert(check);
  assert(levelMessage == LogLevel::medium);
  assert(message.substr(messageoutTextStart) == "hello\n");
}
void test_message_format_parseHeader_valid_high() {
  // Arrange
  std::string message = "[2025-01-02 01:02:03] [high] : hello\n";
  LogLevel levelMessage;
  size_t messageoutTextStart;
  bool check;
  // Act
  check = parseHeader(message, levelMessage, messageoutTextStart);
  // Assert
  assert(check);
  assert(levelMessage == LogLevel::high);
  assert(message.substr(messageoutTextStart) == "hello\n");
}
void test_message_format_parseHeader_valid_unknown() {
  // Arrange
  std::string message = "[2025-01-02 01:02:03] [unknown] : hello\n";
  LogLevel levelMessage;
  size_t messageoutTextStart;
  bool check;
  // Act
  check = parseHeader(message, levelMessage, messageoutTextStart);
  // Assert
  assert(check);
  assert(levelMessage == LogLevel::unknown);
  assert(message.substr(messageoutTextStart) == "hello\n");
}

// Два заголовка в строке
void test_message_format_parseHeader_valid_copy_message() {
  // Arrange
  std::string message = "[2025-01-02 01:02:03] [low] : hello [2025-01-02 "
                        "01:02:03] [medium] : hello\n";
  LogLevel levelMessage;
  size_t messageoutTextStart;
  bool check;
  // Act
  check = parseHeader(message, levelMessage, messageoutTextStart);
  // Assert
  assert(check);
  assert(levelMessage == LogLevel::low);
  assert(message.substr(messageoutTextStart) == "hello [2025-01-02 "
                                                "01:02:03] [medium] : hello\n");
}
// Нет заголовка
void test_message_format_parseHeader_invalid_no_title() {
  // Arrange
  std::string message = "hello\n";
  LogLevel levelMessage;
  size_t messageoutTextStart;
  bool check;
  // Act
  check = parseHeader(message, levelMessage, messageoutTextStart);
  // Assert
  assert(!check);
}

// Неверный уровень
void test_message_format_parseHeader_invalid_level() {
  // Arrange
  std::string message = "[2025-01-02 03:04:05] [HIGH] : hello\n";
  LogLevel levelMessage;
  size_t messageoutTextStart;
  bool check;
  // Act
  check = !parseHeader(message, levelMessage, messageoutTextStart);
  // Assert
  assert(check);
}
// тест для каждого уровня
void test_message_format_createMessage_valid_all_level() {
  // Arrange
  std::string message = "hello";
  std::vector<LogLevel> levelsMessages{LogLevel::low, LogLevel::medium,
                                       LogLevel::high, LogLevel::unknown};
  std::string messageCreate;
  LogLevel levelCreate;
  size_t messageoutTextStart;

  std::vector<bool> check;
  // Act
  for (auto l : levelsMessages) {
    messageCreate = createMessage(message, l);
    check.push_back(
        parseHeader(messageCreate, levelCreate, messageoutTextStart));
  }
  // Assert
  for (auto c : check)
    assert(c);
}
// Перегрузка по умолчанию без уровня
void test_message_format_createMessage_valid_no_level() {
  // Arrange
  std::string message = "hello";
  std::string messageCreate;
  LogLevel levelCreate;
  size_t messageoutTextStart;

  bool check;
  // Act

  messageCreate = createMessage(message);
  check = (parseHeader(messageCreate, levelCreate, messageoutTextStart));

  // Assert
  assert(check);
  assert(levelCreate == LogLevel::unknown);
  assert(messageCreate.substr(messageoutTextStart) == "hello\n");
}
// Сохранность текста
void test_message_format_createMessage_valid_different_message() {
  // Arrange
  std::vector<std::string> messages = {"a b c", " leading", "trailing ",
                                       "привет мир", ""};
  std::vector<std::string> created;
  created.reserve(messages.size());

  const LogLevel level = LogLevel::medium;

  // Act
  for (const auto &msg : messages) {
    created.push_back(createMessage(msg, level));
  }

  // Assert
  for (std::size_t i = 0; i < messages.size(); ++i) {
    LogLevel parsed = LogLevel::unknown;
    std::size_t start = 0;
    bool ok = parseHeader(created[i], parsed, start);
    assert(ok); // заголовок должен распознаться
    assert(parsed == level); // уровень в заголовке совпадает с заданным
    auto tail = created[i].substr(start);
    assert(tail == messages[i] || tail == messages[i] + "\n");
  }
}