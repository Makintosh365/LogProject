// вверху оставить #include'ы
// tests/add_test.cpp — в самом верху
#include "common/all_tests.h" // <-- именно этот путь, как у тебя в проекте
#include <iostream>
#include <string>

using Test = void (*)();
struct Entry {
  const char *name;
  Test fn;
};
static const Entry kTests[] = {
    {"toString_high", &test_log_level_toString_valid_high},
    {"toString_medium", &test_log_level_toString_valid_medium},
    {"toString_low", &test_log_level_toString_valid_low},
    {"toString_invalid", &test_log_level_toString_invalid_null},

    {"convertImportance_high", &test_log_level_convertImportance_valid_high},
    {"convertImportance_medium",
     &test_log_level_convertImportance_valid_medium},
    {"convertImportance_low", &test_log_level_convertImportance_valid_low},
    {"convertImportance_invalid",
     &test_log_level_convertImportance_invalid_null},

    {"parseHeader_valid_low", &test_message_format_parseHeader_valid_low},
    {"parseHeader_valid_medium", &test_message_format_parseHeader_valid_medium},
    {"parseHeader_valid_high", &test_message_format_parseHeader_valid_high},
    {"parseHeader_valid_unknown",
     &test_message_format_parseHeader_valid_unknown},
    {"parseHeader_valid_copy_message",
     &test_message_format_parseHeader_valid_copy_message},
    {"parseHeader_invalid_no_title",
     &test_message_format_parseHeader_invalid_no_title},
    {"parseHeader_invalid_level",
     &test_message_format_parseHeader_invalid_level},

    {"createMessage_valid_all_level",
     &test_message_format_createMessage_valid_all_level},
    {"createMessage_valid_no_level",
     &test_message_format_createMessage_valid_no_level},
    {"createMessage_valid_different_message",
     &test_message_format_createMessage_valid_different_message},

    {"statisticsRecord_valid_counts_by_level",
     &test_statistics_statisticsRecord_valid_counts_by_level},
    {"one_lengths_and_average",
     &test_statistics_statisticsRecord_valid_one_lengths_and_average},
    {"statisticsRecord_valid_more_lengths_and_average",
     &test_statistics_statisticsRecord_valid_more_lengths_and_average},
    {"statisticsRecord_valid_UTF_8_ASCII",
     &test_statistics_statisticsRecord_valid_UTF_8_ASCII},

    {"utf8_ascii_basic", &test_utf8_utils_ascii_basic},
    {"utf8_cyrillic", &test_utf8_utils_cyrillic},
    {"utf8_emojis", &test_utf8_utils_emojis},
    {"utf8_combining", &test_utf8_utils_combining},

    {"logger_file_single", &test_logger_file_sink_single_write},
    {"logger_file_level", &test_logger_file_sink_respects_level},
    {"logger_file_threads", &test_logger_file_sink_multithreaded},
};

int main(int argc, char **argv) {
  if (argc > 1) {
    std::string name = argv[1];
    for (const auto &e : kTests) {
      if (name == e.name) {
        e.fn();
        return 0;
      }
    }
    std::cerr << "Unknown test name: " << name << "\n";
    return 2;
  }

  std::cout << "[TESTS] runner up\n";
  test_log_level_toString_valid_high();
  test_log_level_toString_valid_medium();
  test_log_level_toString_valid_low();
  test_log_level_toString_invalid_null();

  test_log_level_convertImportance_valid_medium();
  test_log_level_convertImportance_valid_high();
  test_log_level_convertImportance_valid_low();
  test_log_level_convertImportance_invalid_null();
  test_log_level_convertImportance_invalid_space();

  test_message_format_parseHeader_valid_low();
  test_message_format_parseHeader_valid_medium();
  test_message_format_parseHeader_valid_high();
  test_message_format_parseHeader_valid_unknown();
  test_message_format_parseHeader_valid_copy_message();

  test_message_format_parseHeader_invalid_no_title();
  test_message_format_parseHeader_invalid_level();

  test_message_format_createMessage_valid_all_level();
  // Перегрузка по умолчанию без уровня
  test_message_format_createMessage_valid_no_level();
  // Сохранность текста
  test_message_format_createMessage_valid_different_message();

  test_statistics_statisticsRecord_valid_counts_by_level();
  test_statistics_statisticsRecord_valid_one_lengths_and_average();
  test_statistics_statisticsRecord_valid_more_lengths_and_average();
  test_statistics_statisticsRecord_valid_UTF_8_ASCII();

  test_utf8_utils_ascii_basic();
  test_utf8_utils_cyrillic();
  test_utf8_utils_emojis();
  test_utf8_utils_combining();

  test_logger_file_sink_single_write();
  test_logger_file_sink_respects_level();
  test_logger_file_sink_multithreaded();

  return 0; // пока просто успешно завершаемся
}
