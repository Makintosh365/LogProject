#include "client.h"
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

//------------------------------------------------------------------------------
//------- Client — простой TCP-клиент для отправки сообщений на локальный сервер
//------------------------------------------------------------------------------

// Создаёт сокет и сохраняет порт для будущего подключения.
Client::Client(unsigned portClient) : port_(portClient) {
  sock_ = socket(AF_INET, SOCK_STREAM, 0);
  if (sock_ < 0) {
    perror("socket");
  }
}

// Подключается к 127.0.0.1:port_. Возвращает true при успехе.
bool Client::connectClient(const char *host) {
  sockaddr_in serverAddr{};
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(port_);
  inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

  if (connect(sock_, (sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
    perror("connect");
    close(sock_);
    sock_ = -1;
    return false;
  }

  return true;
}
// Отправляет сообщение. Если сокет не открыт — возвращает false.
bool Client::sendMessage(std::string_view msg) {
  if (sock_ < 0)
    return false;
  return send(sock_, msg.data(), msg.size(), 0) >= 0;
}
// Закрывает сокет, если он был открыт.
void Client::closeClient() {
  if (sock_ >= 0) {
    close(sock_);
    sock_ = -1;
  }
}

Client::~Client() { closeClient(); }