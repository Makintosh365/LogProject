#include "common/statistics.h"
#include <algorithm>
#include <iostream>

//------------------------------------------------------------------------------
//-------- Обновляет статистику сообщений по новому сообщению
//------------------------------------------------------------------------------
void statisticsRecord(StatsCount &countStat, StatsLength &lengthStat,
                      const std::string &message, bool &dirty) {

  LogLevel lvl{};
  size_t textStart = 0;
  if (!parseHeader(message, lvl, textStart))
    return;
  // Увеличиваем счётчик сообщений по уровню
  countStat.messagesByLevel[lvl]++;

  // Вычисляем длину текста (UTF-8)
  size_t contentStart = message.find("] : ");
  textStart = contentStart + 5;
  size_t textLength = utf8Length(message) - textStart;

  // Собираем длину всех сообщения для подсчета среднего значения
  lengthStat.totalLength += textLength;
  // Обновляем максимальную длину сообщения
  lengthStat.max = std::max(lengthStat.max, textLength);
  // Обновляем минимальную длину сообщения
  lengthStat.min = std::min(lengthStat.min, textLength);

  // Обновляем общее количество сообщений
  countStat.totalMessages++;

  // Средняя длина
  lengthStat.average = static_cast<double>(lengthStat.totalLength) /
                       static_cast<double>(countStat.totalMessages);

  // Добавляем время отправки последнего сообщения
  auto now = std::chrono::system_clock::now();
  countStat.messageTimestamps.push_back(now);
  // Удаляем сообщения которые старше 1 часа
  const auto cutoff = now - std::chrono::hours(1);
  while (!countStat.messageTimestamps.empty() &&
         countStat.messageTimestamps.front() <= cutoff) {
    countStat.messageTimestamps.pop_front();
  }
  dirty = true; // Изменение было
}
//------------------------------------------------------------------------------
//-------- Вывод статистики в консоль
//------------------------------------------------------------------------------
void statisticsOutput(StatsCount &countStat, StatsLength &lengthStat,
                      bool &dirty) {
  if (!dirty) // Данные не изменились с последнего отображения статистики
    return;

  std::cout << "\nПодсчет статистик кол-ва сообщений:";

  std::cout << "\nСообщений всего : " << countStat.totalMessages;

  std::cout << "\nCообщений по уровню важности : \n";
  for (const auto &l : countStat.messagesByLevel) {
    std::cout << toString(l.first) << ": " << l.second << " | ";
  }

  std::cout << "\nCообщений за последний чаc : "
            << countStat.messageTimestamps.size();

  std::cout << "\nПодсчет статистик длин сообщений:";
  std::cout << "\nМаксимум : " << lengthStat.max
            << " | Минимум : " << lengthStat.min
            << " | Средня : " << lengthStat.average << "\n";

  dirty = false;
}