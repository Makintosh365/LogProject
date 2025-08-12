#pragma once
#include <string>

//------------------------------------------------------------------------------
//------- Client — простой TCP-клиент для отправки сообщений на локальный сервер
//------------------------------------------------------------------------------

class Client {
private:
  int port_;
  int sock_;

public:
  Client(unsigned port);
  ~Client();

  bool connectClient(const char *host);
  bool sendMessage(std::string_view msg);

  void closeClient();
};
