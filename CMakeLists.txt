cmake_minimum_required(VERSION 3.15)

project(LogProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)

#------------------------------------------------------------------------------
#-------- Библиотека логгера: static + shared
#------------------------------------------------------------------------------


add_library(logger_static STATIC
  logger/logger.cpp
  client/client.cpp
)

add_library(logger_shared SHARED
  logger/logger.cpp
  client/client.cpp
)

target_include_directories(logger_static PUBLIC
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}
)
target_include_directories(logger_shared PUBLIC
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}
)

set_target_properties(logger_static PROPERTIES POSITION_INDEPENDENT_CODE ON)

#------------------------------------------------------------------------------
#-------- Приложение для ввода сообщений 
#------------------------------------------------------------------------------
add_executable(app
  app/main.cpp
)
target_include_directories(app PRIVATE
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}
)
target_link_libraries(app PRIVATE
  logger_static
  Threads::Threads
)

#------------------------------------------------------------------------------
#-------- Сервер, сбор статистики
#------------------------------------------------------------------------------

add_executable(log_stats_server
  server/server.cpp
  server/statistics.cpp
)
target_include_directories(log_stats_server PRIVATE
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}
)
target_link_libraries(log_stats_server PRIVATE
  logger_static
  Threads::Threads
)


#------------------------------------------------------------------------------
#-------- Тесты
#------------------------------------------------------------------------------
add_executable(tests
  tests/add_test.cpp
  tests/common/test_log_level.cpp
  tests/common/test_message_format.cpp
  tests/common/test_statistics.cpp
  tests/common/test_logger.cpp
  tests/common/test_utf8_utils.cpp
  server/statistics.cpp
)
target_include_directories(tests PRIVATE
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}
)
target_link_libraries(tests PRIVATE logger_static Threads::Threads)

# ---------- CTest ----------
enable_testing()

# log_level
add_test(NAME test_log_level_toString_valid_high      COMMAND $<TARGET_FILE:tests> toString_high)
add_test(NAME test_log_level_toString_valid_medium    COMMAND $<TARGET_FILE:tests> toString_medium)
add_test(NAME test_log_level_toString_valid_low       COMMAND $<TARGET_FILE:tests> toString_low)
add_test(NAME test_log_level_toString_invalid_null    COMMAND $<TARGET_FILE:tests> toString_invalid)

add_test(NAME test_log_level_convertImportance_valid_high   COMMAND $<TARGET_FILE:tests> convertImportance_high)
add_test(NAME test_log_level_convertImportance_valid_medium COMMAND $<TARGET_FILE:tests> convertImportance_medium)
add_test(NAME test_log_level_convertImportance_valid_low    COMMAND $<TARGET_FILE:tests> convertImportance_low)
add_test(NAME test_log_level_convertImportance_invalid_null COMMAND $<TARGET_FILE:tests> convertImportance_invalid)

# message_format
add_test(NAME test_message_format_parseHeader_valid_low      COMMAND $<TARGET_FILE:tests> parseHeader_valid_low)
add_test(NAME test_message_format_parseHeader_valid_medium   COMMAND $<TARGET_FILE:tests> parseHeader_valid_medium)
add_test(NAME test_message_format_parseHeader_valid_high     COMMAND $<TARGET_FILE:tests> parseHeader_valid_high)
add_test(NAME test_message_format_parseHeader_valid_unknown  COMMAND $<TARGET_FILE:tests> parseHeader_valid_unknown)
add_test(NAME test_message_format_parseHeader_valid_copy_message  COMMAND $<TARGET_FILE:tests> parseHeader_valid_copy_message)
add_test(NAME test_message_format_parseHeader_invalid_no_title    COMMAND $<TARGET_FILE:tests> parseHeader_invalid_no_title)
add_test(NAME test_message_format_parseHeader_invalid_level       COMMAND $<TARGET_FILE:tests> parseHeader_invalid_level)

add_test(NAME test_message_format_createMessage_valid_all_level         COMMAND $<TARGET_FILE:tests> createMessage_valid_all_level)
add_test(NAME test_message_format_createMessage_valid_no_level          COMMAND $<TARGET_FILE:tests> createMessage_valid_no_level)
add_test(NAME test_message_format_createMessage_valid_different_message COMMAND $<TARGET_FILE:tests> createMessage_valid_different_message)

# statistics
add_test(NAME test_statistics_statisticsRecord_valid_counts_by_level         COMMAND $<TARGET_FILE:tests> statisticsRecord_valid_counts_by_level)
add_test(NAME test_statistics_statisticsRecord_valid_one_lengths_and_average COMMAND $<TARGET_FILE:tests> one_lengths_and_average)
add_test(NAME test_statistics_statisticsRecord_valid_more_lengths_and_average COMMAND $<TARGET_FILE:tests> statisticsRecord_valid_more_lengths_and_average)
add_test(NAME test_statistics_statisticsRecord_valid_UTF_8_ASCII             COMMAND $<TARGET_FILE:tests> statisticsRecord_valid_UTF_8_ASCII)

# utf8 (новые имена)
add_test(NAME test_utf8_utils_ascii_basic  COMMAND $<TARGET_FILE:tests> utf8_ascii_basic)
add_test(NAME test_utf8_utils_cyrillic     COMMAND $<TARGET_FILE:tests> utf8_cyrillic)
add_test(NAME test_utf8_utils_emojis       COMMAND $<TARGET_FILE:tests> utf8_emojis)
add_test(NAME test_utf8_utils_combining    COMMAND $<TARGET_FILE:tests> utf8_combining)

# logger (новые имена)
add_test(NAME test_logger_file_sink_single_write  COMMAND $<TARGET_FILE:tests> logger_file_single)
add_test(NAME test_logger_file_sink_respects_level COMMAND $<TARGET_FILE:tests> logger_file_level)
add_test(NAME test_logger_file_sink_multithreaded  COMMAND $<TARGET_FILE:tests> logger_file_threads)