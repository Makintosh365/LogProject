// tests/common/test_utf8_utils.cpp
#include "all_tests.h"
#include "common/utf8_utils.h"
#include <cassert>
#include <string>

static void expectLen(const std::string &s, std::size_t expected) {
  auto got = utf8Length(s);
  assert(got == expected);
}

void test_utf8_utils_ascii_basic() {
  expectLen("hello", 5); // ASCII
  expectLen("", 0);      // пустая строка
  expectLen("   ", 3);   // пробелы
}

void test_utf8_utils_cyrillic() {
  // «Привет» в UTF-8: 6 символов, но байтов больше
  expectLen(u8"Привет", 6);
  expectLen(u8"тест", 4);
}

void test_utf8_utils_emojis() {
  // Эмодзи — по одному юникод-символу каждый
  expectLen(u8"🙂", 1);
  expectLen(u8"🙂🙂🙂", 3);
  // Смешанный текст
  expectLen(std::string(u8"A🙂Б"), 3);
}

void test_utf8_utils_combining() {
  // Буква + комбинирующий диакритический знак: 1 базовый + 1 комбинирующий
  // Для нашей простой логики подсчёта лидирующих байтов получится 2
  expectLen(std::string("a\xCC\x81"), 2); // "á"
}
