#pragma once
#include "common/log_level.h"
#include "common/message_format.h"

#include <fstream>
#include <memory>
#include <mutex>
#include <string>
#include <string_view>
#include <vector>
//==============================================================================
// Logger.h
//==============================================================================
class Client;

struct ILogSink {
  virtual ~ILogSink() = default;
  virtual void write(std::string_view entry) = 0;
};

//------------------------------------------------------------------------------
//-------- FileSink — приёмник, пишущий в файл
//------------------------------------------------------------------------------
class FileSink : public ILogSink {
private:
  std::ofstream logFile_;
  mutable std::mutex mx_;

public:
  explicit FileSink(const std::string &filename);
  void write(std::string_view entry) override;
  ~FileSink() noexcept = default;
};

//------------------------------------------------------------------------------
//-------- SocketSink — приёмник, пишущий в сокет
//------------------------------------------------------------------------------
class SocketSink : public ILogSink {
private:
  Client *client_;
  mutable std::mutex mx_;

public:
  explicit SocketSink(Client *client);
  void write(std::string_view entry) override;
  ~SocketSink() noexcept = default;
};

//------------------------------------------------------------------------------
//-------- Logger — основной класс для логирования
//------------------------------------------------------------------------------
class Logger {

private:
  std::vector<std::shared_ptr<ILogSink>> sinks_;
  LogLevel level_;
  mutable std::mutex mx_;

public:
  Logger(std::string_view filename, LogLevel level);
  Logger(std::string_view filename);
  Logger(std::string_view filename, LogLevel level, Client &client);
  ~Logger() noexcept = default;

  // Добавление приёмников
  void addSink(std::shared_ptr<ILogSink> sink);
  void addFile(const std::string &filename);
  void addSocket(Client *client);

  // Запись сообщений
  void log(std::string_view message);
  void log(std::string_view message, LogLevel level);

  void setLevel(LogLevel level);
  LogLevel getLevel();
};