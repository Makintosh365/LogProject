#include "all_tests.h"
#include "common/statistics.h"
#include <cassert>

#include <chrono>
#include <deque>
#include <map>
#include <string>
// Счётчики уровней выросли
void test_statistics_statisticsRecord_valid_counts_by_level() {
  StatsCount countStat;
  StatsLength lengthStat;
  std::vector<std::string> messages = {
      "[2025-01-02 01:02:03] [low] : hello\n",
      "[2025-01-02 01:02:03] [medium] : hello\n",
      "[2025-01-02 01:02:03] [high] : hello\n",
      "[2025-01-02 01:02:03] [unknown] : hello\n"};
  std::vector<bool> dirtys{false, false, false, false, true};

  for (size_t i = 0; i < messages.size(); i++) {

    auto msg = messages[i];
    bool temp = dirtys[i];
    // Если сообщение валидно после вызова dirty == true.
    statisticsRecord(countStat, lengthStat, msg, temp);
    dirtys[i] = temp;
  }
  // Если невалидно сообщение dirty не меняется
  bool temp = dirtys[4];
  statisticsRecord(countStat, lengthStat, "hello", temp);
  dirtys[4] = temp;
  for (bool d : dirtys)
    assert(d);

  assert(countStat.totalMessages == messages.size());
  for (auto mbl : countStat.messagesByLevel) {
    assert(mbl.second == 1);
  }
}
// Обновление длин с одного сообщения
void test_statistics_statisticsRecord_valid_one_lengths_and_average() {
  StatsCount countStat;
  StatsLength lengthStat;
  std::string message = "[2025-01-02 01:02:03] [low] : hello\n";
  bool dirty;

  statisticsRecord(countStat, lengthStat, message, dirty);

  assert(countStat.totalMessages == 1);
  assert(lengthStat.min == lengthStat.max);
  assert(lengthStat.average == lengthStat.totalLength);
  assert(lengthStat.totalLength == 5);
}
// Обновление длин с двух сообщения
void test_statistics_statisticsRecord_valid_more_lengths_and_average() {
  StatsCount countStat;
  StatsLength lengthStat;
  std::vector<std::string> messages = {
      "[2025-01-02 01:02:03] [medium] : hellohello\n",
      "[2025-01-02 01:02:03] [medium] : hellohellohellohello\n"};
  bool dirty;

  for (auto m : messages) {
    statisticsRecord(countStat, lengthStat, m, dirty);
  }

  assert(countStat.totalMessages == 2);
  assert(lengthStat.average == 15);
  assert(lengthStat.max == 20);
  assert(lengthStat.min == 10);
  assert(lengthStat.totalLength == 30);
}
// UTF-8 и ASCII корректность длины
void test_statistics_statisticsRecord_valid_UTF_8_ASCII() {
  StatsCount countStat;
  StatsLength lengthStat;
  std::vector<std::string> messages = {
      "[2025-01-02 01:02:03] [medium] : hello\n",
      "[2025-01-02 01:02:03] [medium] : Привет\n"};
  bool dirty;

  for (auto m : messages) {
    statisticsRecord(countStat, lengthStat, m, dirty);
  }

  assert(countStat.totalMessages == 2);
  assert(lengthStat.average == 5.5);
  assert(lengthStat.max == 6);
  assert(lengthStat.min == 5);
  assert(lengthStat.totalLength == 11);
}
