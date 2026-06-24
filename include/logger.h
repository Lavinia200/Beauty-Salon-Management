
#ifndef OOP_LOGGER_H
#define OOP_LOGGER_H
#include <string>

class Logger {
private:
    Logger() = default;
public:
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    static void log(const std::string& mesaj) {
        std::cout << "[LOG SALON]: " << mesaj << std::endl;
    }
};
#endif //OOP_LOGGER_H