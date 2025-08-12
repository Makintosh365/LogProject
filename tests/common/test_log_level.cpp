#include "all_tests.h"
#include "common/log_level.h"
#include <cassert>

void test_log_level_toString_valid_high() {
  // Arrange
  LogLevel level = LogLevel::high;
  std::string levelString = "high";
  std::string testLevelString;

  // Act
  testLevelString = toString(level);

  // Assert
  assert(testLevelString == levelString);
}
void test_log_level_toString_valid_medium() {
  // Arrange
  LogLevel level = LogLevel::medium;
  std::string levelString = "medium";
  std::string testLevelString;

  // Act
  testLevelString = toString(level);

  // Assert
  assert(testLevelString == levelString);
}
void test_log_level_toString_valid_low() {
  // Arrange
  LogLevel level = LogLevel::low;
  std::string levelString = "low";
  std::string testLevelString;

  // Act
  testLevelString = toString(level);

  // Assert
  assert(testLevelString == levelString);
}
void test_log_level_toString_invalid_null() {
  // Arrange
  LogLevel level = LogLevel::unknown;
  std::string levelString = "unknown";
  std::string testLevelString;

  // Act
  testLevelString = toString(level);

  // Assert
  assert(testLevelString == levelString);
}

void test_log_level_convertImportance_valid_medium() {
  // Arrange
  LogLevel level = LogLevel::medium;
  std::string levelString = "medium";
  LogLevel testLevel;

  // Act
  testLevel = convertImportance(levelString);

  // Assert
  assert(level == testLevel);
}
void test_log_level_convertImportance_valid_high() {
  // Arrange
  LogLevel level = LogLevel::high;
  std::string levelString = "high";
  LogLevel testLevel;

  // Act
  testLevel = convertImportance(levelString);

  // Assert
  assert(level == testLevel);
}
void test_log_level_convertImportance_valid_low() {
  // Arrange
  LogLevel level = LogLevel::low;
  std::string levelString = "low";
  LogLevel testLevel;

  // Act
  testLevel = convertImportance(levelString);

  // Assert
  assert(level == testLevel);
}
void test_log_level_convertImportance_invalid_null() {
  // Arrange
  LogLevel level = LogLevel::unknown;
  std::string levelString = "";
  LogLevel testLevel;

  // Act
  testLevel = convertImportance(levelString);

  // Assert

  assert(level == testLevel);
}
void test_log_level_convertImportance_invalid_space() {
  // Arrange
  LogLevel level = LogLevel::unknown;
  std::string levelString = " ";
  LogLevel testLevel;

  // Act
  testLevel = convertImportance(levelString);

  // Assert

  assert(level == testLevel);
}