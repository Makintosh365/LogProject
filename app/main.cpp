#include "client/client.h"
#include "common/log_level.h"
#include "logger/logger.h"

#include <arpa/inet.h>
#include <cstring>
#include <functional>
#include <iostream>
#include <limits>
#include <mutex>
#include <netinet/in.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

//------------------------------------------------------------------------------
//-------- Консольное приложение для отправки логов в файл
// Параметры запуска: <logfile> <level>
// ./app mylog.txt high
//------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    const char* host         = "127.0.0.1"; // адреса сервера куда отправлять логи
    std::string filename     = "default_log.txt";
    LogLevel    defaultLevel = LogLevel::medium;

    // -------- аргументы запуска --------
    if (argc >= 2)
    {
        LogLevel tmp = convertImportance(argv[1]);
        if (tmp != LogLevel::unknown)
        {
            // Пользователь указал только уровень
            defaultLevel = tmp;
        }
        else
        {
            // Пользователь указал имя файла
            filename = argv[1];
        }
    }
    if (argc >= 3)
    {
        LogLevel tmp = convertImportance(argv[2]);
        if (tmp == LogLevel::unknown)
        {
            std::cerr << "Ошибка: некорректный уровень по умолчанию: '" << argv[2]
                      << "'. Допустимые значения: 0|1|2 или low|medium|high.\n";
            return 2;
        }
        defaultLevel = tmp;
    }

    Logger logger(filename, defaultLevel);

    /*----------------------------------------------------------------------------*/

    std::string importance;

    std::string message;
    std::cout << "\nОтправлять сообщение по сокету Д/Н: ";
    std::string sendOverSocket;

    std::cin >> sendOverSocket;
    Client client(12345);
    if (sendOverSocket == "д" || sendOverSocket == "Д")
    {
        if (client.connectClient(host))
            logger.addSocket(&client);
        else
            std::cout << "Сокет для логирования не найден\n"
                      << "Запись будет только в файл " << filename << '\n';
    }
    while (true)
    {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "\nСообщение для журнала: ";
        std::getline(std::cin, message);

        std::cout << "Уровень важности (0-low, 1-medium, 2-high): ";
        std::cin >> importance;

        // Поток для записи в лог.
        std::thread t1([&logger, message, importance] { logger.log(message, convertImportance(importance)); });
        t1.detach();

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::string exitStr;
        std::cout << "Продолжить запись (Д/Н): ";
        std::cin >> exitStr;

        if (exitStr == "н" || exitStr == "Н")
        {
            std::cout << "Выход..." << std::endl;
            break;
        }
    }
}
