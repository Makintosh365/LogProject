//==============================================================================
// Logger.cpp
//==============================================================================
#include "logger.h"
#include "client/client.h"
#include "include/common/message_format.h"

#include <ctime>
#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>
#include <string_view>
#include <vector>

//----------------------------------------------------------------------------
//-------- Конструктор
//----------------------------------------------------------------------------
Logger::Logger(std::string_view filename, LogLevel level) : level_(level) {
  addFile(std::string(filename));
}
// Упрощённый конструктор: уровень — medium.
Logger::Logger(std::string_view filename)
    : Logger(std::string(filename), LogLevel::medium) {}

// Конструктор, который пишет в файл и сокет.
Logger::Logger(std::string_view filename, LogLevel level, Client &client)
    : level_(level) {
  addFile(std::string(filename));
  addSocket(&client);
}

//----------------------------------------------------------------------------
//-------- Добавление приёмника куда писать логи
//----------------------------------------------------------------------------
// Добавляет приёмник.
void Logger::addSink(std::shared_ptr<ILogSink> sink) {
  std::lock_guard<std::mutex> lock(mx_);
  sinks_.push_back(std::move(sink));
}

// Создаёт файловый приёмник и регистрирует его.
void Logger::addFile(const std::string &fileName) {
  addSink(std::make_shared<FileSink>(fileName));
}

// Создаёт сокетный приёмник и регистрирует его.
void Logger::addSocket(Client *client) {
  addSink(std::make_shared<SocketSink>(client));
}

//----------------------------------------------------------------------------
//-------- Меняет текущий уровень логирования.
//----------------------------------------------------------------------------

void Logger::setLevel(LogLevel level) { level_ = level; }

//----------------------------------------------------------------------------
//-------- Вывести минимальный уровень логирования.
//----------------------------------------------------------------------------

LogLevel Logger::getLevel() { return level_; }

//----------------------------------------------------------------------------
//-------- Запись в файл и сокет
//----------------------------------------------------------------------------

// Пишет сообщение с уровнем по умолчанию.
void Logger::log(std::string_view message) { log(message, level_); }

// Пишет сообщение с явным уровнем во все приёмники.
void Logger::log(std::string_view message, LogLevel level) {
  if (level < level_)
    return;
  std::lock_guard<std::mutex> lock(mx_);
  for (auto s : sinks_)
    s->write(createMessage(message, level));
}

//----------------------------------------------------------------------------
//-------- FileSink — запись в файл
//----------------------------------------------------------------------------

// Открывает файл в режиме добавления.
FileSink::FileSink(const std::string &filename)
    : logFile_(filename, std::ios::app) {
  if (!logFile_.is_open()) {
    std::cerr << "Не удалось открыть файл для логирования: " << filename
              << std::endl;
  }
};
// Записывает строку и сразу сбрасывает буфер.
void FileSink::write(std::string_view entry) {
  std::lock_guard<std::mutex> lock(mx_);
  logFile_ << std::string(entry) << std::flush;
};

//----------------------------------------------------------------------------
//-------- SocketSink — отправка в сокет
//----------------------------------------------------------------------------

// Хранит копию клиента
SocketSink::SocketSink(Client *client) : client_(client){};

void SocketSink::write(std::string_view entry) {
  std::lock_guard<std::mutex> lock(mx_);
  client_->sendMessage(std::string(entry)); // Клиент ожидает std::string.
};

//--------------------------------------------------------------------------//
//--------------------------------------------------------------------------//