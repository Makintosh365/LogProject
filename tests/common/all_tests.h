#pragma once

void test_log_level_toString_valid_high();
void test_log_level_toString_valid_medium();
void test_log_level_toString_valid_low();
void test_log_level_toString_invalid_null();

void test_log_level_convertImportance_valid_medium();
void test_log_level_convertImportance_valid_high();
void test_log_level_convertImportance_valid_low();
void test_log_level_convertImportance_invalid_null();
void test_log_level_convertImportance_invalid_space();
// void test_message_format();
// void test_statistics();
// void test_utf8_utils();

void test_message_format_parseHeader_valid_low();
void test_message_format_parseHeader_valid_medium();
void test_message_format_parseHeader_valid_high();
void test_message_format_parseHeader_valid_unknown();
void test_message_format_parseHeader_valid_copy_message();

void test_message_format_parseHeader_invalid_no_title();
void test_message_format_parseHeader_invalid_level();

void test_message_format_createMessage_valid_all_level();
void test_message_format_createMessage_valid_no_level();
void test_message_format_createMessage_valid_different_message();

void test_statistics_statisticsRecord_valid_counts_by_level();
void test_statistics_statisticsRecord_valid_one_lengths_and_average();
void test_statistics_statisticsRecord_valid_more_lengths_and_average();
void test_statistics_statisticsRecord_valid_UTF_8_ASCII();
void test_statistics_statisticsRecord_valid_messages_hour();

// utf8
void test_utf8_utils_ascii_basic();
void test_utf8_utils_cyrillic();
void test_utf8_utils_emojis();
void test_utf8_utils_combining();

// logger
void test_logger_file_sink_single_write();
void test_logger_file_sink_respects_level();
void test_logger_file_sink_multithreaded();
