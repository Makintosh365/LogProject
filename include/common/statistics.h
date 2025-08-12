#pragma once

#include "common/log_level.h"
#include "common/message_format.h"
#include "common/utf8_utils.h"

#include <chrono>
#include <deque>
#include <map>
#include <string>

struct StatsCount {
  int totalMessages = 0;
  std::map<LogLevel, int> messagesByLevel;
  std::deque<std::chrono::system_clock::time_point> messageTimestamps;
};

struct StatsLength {
  size_t totalLength = 0;
  size_t max = 0;
  size_t min = SIZE_MAX;
  double average = 0.0;
};

void statisticsRecord(StatsCount &countStat, StatsLength &lengthStat,
                      const std::string &message, bool &dirty);

void statisticsOutput(StatsCount &countStat, StatsLength &lengthStat,
                      bool &dirty);
