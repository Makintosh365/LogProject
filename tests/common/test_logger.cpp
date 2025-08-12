// tests/common/test_logger.cpp
#include "all_tests.h"
#include "common/log_level.h"
#include "common/message_format.h"
#include "logger/logger.h"

#include <cassert>
#include <cstdio>
#include <fstream>
#include <regex>
#include <string>
#include <thread>
#include <vector>

static std::string readAll(const std::string &path) {
  std::ifstream in(path);
  std::string data((std::istreambuf_iterator<char>(in)),
                   std::istreambuf_iterator<char>());
  return data;
}

void test_logger_file_sink_single_write() {
  const std::string path = "test_log_single.txt";
  std::remove(path.c_str());

  Logger logger(path, LogLevel::low);
  logger.log("hello", LogLevel::medium);

  auto out = readAll(path);
  // Проверяем заголовок и уровень
  LogLevel lvl{};
  size_t start{};
  assert(parseHeader(out, lvl, start));
  assert(lvl == LogLevel::medium);
  assert(out.substr(start) == "hello\n");

  std::remove(path.c_str());
}

void test_logger_file_sink_respects_level() {
  const std::string path = "test_log_level.txt";
  std::remove(path.c_str());

  Logger logger(path, LogLevel::medium); // порог medium
  logger.log("L", LogLevel::low);        // не должен попасть
  logger.log("M", LogLevel::medium);     // должен
  logger.log("H", LogLevel::high);       // должен

  auto out = readAll(path);
  // Должны встретиться только M и H
  size_t posL = out.find("] [low] : L\n");
  size_t posM = out.find("] [medium] : M\n");
  size_t posH = out.find("] [high] : H\n");

  assert(posL == std::string::npos);
  assert(posM != std::string::npos);
  assert(posH != std::string::npos);

  std::remove(path.c_str());
}

void test_logger_file_sink_multithreaded() {
  const std::string path = "test_log_threads.txt";
  std::remove(path.c_str());

  Logger logger(path, LogLevel::low);

  const int threads = 6;
  const int perThread = 50;
  std::vector<std::thread> ts;
  for (int t = 0; t < threads; ++t) {
    ts.emplace_back([&logger, t, perThread] {
      for (int i = 0; i < perThread; ++i) {
        logger.log("msg_" + std::to_string(t) + "_" + std::to_string(i),
                   LogLevel::high);
      }
    });
  }
  for (auto &th : ts)
    th.join();

  auto out = readAll(path);

  // Проверим что все сообщения присутствуют
  int found = 0;
  for (int t = 0; t < threads; ++t) {
    for (int i = 0; i < perThread; ++i) {
      auto needle = "] [high] : msg_" + std::to_string(t) + "_" +
                    std::to_string(i) + "\n";
      if (out.find(needle) != std::string::npos)
        ++found;
    }
  }
  assert(found == threads * perThread);

  std::remove(path.c_str());
}
