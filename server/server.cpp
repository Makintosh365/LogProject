
#include "common/log_level.h"
#include "common/message_format.h"
#include "common/statistics.h"
#include "common/utf8_utils.h"

#include <arpa/inet.h>
#include <atomic>
#include <chrono>
#include <cstring>
#include <deque>
#include <iostream>
#include <map>
#include <regex>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

std::atomic<bool> running(true); // Переменная для отключения таймера

// Преобразует строку в число с проверкой диапазона.
bool parseUint(const char *s, unsigned min, unsigned max,
               unsigned &out) noexcept {
  if (!s || *s == '\0')
    return false;

  char *end = nullptr;
  errno = 0;
  unsigned long v = std::strtoul(s, &end, 10);

  // ошибки парсинга: нет цифр, переполнение, посторонние символы в конце
  if (errno != 0 || end == s || *end != '\0')
    return false;
  if (v < min || v > max)
    return false;

  out = static_cast<unsigned>(v);
  return true;
}

// Запускает таймер, который периодически выводит статистику.
void runStatsTimer(int timeoutSec, StatsCount &countStat,
                   StatsLength &lengthStat, bool &isDirty) {
  while (running) {
    std::this_thread::sleep_for(std::chrono::seconds(timeoutSec));
    if (!running)
      break;
    if (isDirty)
      statisticsOutput(countStat, lengthStat, isDirty);
  }
}
//------------------------------------------------------------------------------
//-------- TCP-сервер для приёма сообщений и подсчёта статистики
// Параметры запуска. <host> <port> <N> <Tsec>
// ./server_exec 127.0.0.1 12345 100 60
//------------------------------------------------------------------------------
int main(int argc, char *argv[]) {
  // Параметры запуска. <host> <port> <N> <Tsec>
  if (argc < 5) {
    std::cerr << "Usage: " << argv[0] << " <host> <port> <N> <Tsec>\n"
              << "  port: 1..65535\n"
              << "  N:    1..1000000000\n"
              << "  Tsec: 1..86400\n";
    return 1;
  }

  std::string host = argv[1];
  unsigned port, N, Tsec;

  if (!parseUint(argv[2], 1, 65535, port) ||
      !parseUint(argv[3], 1, 1000000000u, N) ||
      !parseUint(argv[4], 1, 86400u, Tsec)) {
    return 1;
  }

  const int BUFFER_SIZE = 1024;

  // Создаём серверный сокет
  int serverFd = socket(AF_INET, SOCK_STREAM, 0);
  if (serverFd < 0) {
    perror("socket");
    return 1;
  }

  sockaddr_in serverAddr{};
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(port);
  serverAddr.sin_addr.s_addr = INADDR_ANY;

  if (bind(serverFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
    perror("bind");
    close(serverFd);
    return 1;
  }

  if (listen(serverFd, 5) < 0) {
    perror("listen");
    close(serverFd);
    return 1;
  }

  std::cout << "Сервер слушает порт " << port << "...\n";

  // Принимаем одно подключение
  sockaddr_in clientAddr{};
  socklen_t clientLen = sizeof(clientAddr);
  int clientFd = accept(serverFd, (sockaddr *)&clientAddr, &clientLen);
  if (clientFd < 0) {
    perror("accept");
    close(serverFd);
    return 1;
  }

  std::cout << "Клиент подключен.\n";

  StatsCount countStat;
  StatsLength lengthStat;
  bool dirty = false; //

  // Запуск потока таймера для периодического вывода статистики
  std::thread timerThread(runStatsTimer, (int)Tsec, std::ref(countStat),
                          std::ref(lengthStat), std::ref(dirty));
  char buffer[BUFFER_SIZE];

  while (true) {

    memset(buffer, 0, BUFFER_SIZE);
    ssize_t received = recv(clientFd, buffer, BUFFER_SIZE - 1, 0);
    if (received <= 0) {
      std::cout << "Клиент отключился.\n";
      break;
    }

    std::string message(buffer);
    // Обновляем статистику
    statisticsRecord(countStat, lengthStat, message, dirty);
    std::cout << "\nПолучено: " << message;
    // Вывод после N сообщений
    if (countStat.totalMessages % N == 0)
      statisticsOutput(countStat, lengthStat, dirty);
  }

  running = false;
  timerThread.join();

  close(clientFd);
  close(serverFd);
  return 0;
}