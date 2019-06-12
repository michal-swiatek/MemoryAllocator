#ifndef __LOG_HISTORY__
#define __LOG_HISTORY__

#include <ctime>
#include <chrono>
#include <utility>
#include <fstream>

#include <vector>
#include <unordered_map>

#include "ILogger.h"

struct LogData
{
    std::string timestamp;
    LOG_MessageType type;
    std::string message;

    LogData(const std::string& ts, LOG_MessageType type, const std::string& msg) : timestamp(ts), type(type), message(msg) { }
};

class LogHistory : public Observer<LOG_MessageType, std::string>
{
    using LogEntry = std::pair<std::string, std::string>;

private:
    std::vector<LogData> history;
    std::unordered_map<LOG_MessageType, std::vector<LogEntry>, SimpleHash<LOG_MessageType> > logsByType;

    void updateObserver(const Subject<LOG_MessageType, std::string>* subject) override
    {
        std::time_t currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::string logTime = std::ctime(&currentTime);
        logTime.erase(logTime.length() - 1);

        LOG_MessageType logType = subject->getEventType();

        history.emplace_back(logTime, logType, subject->getEventData());
        logsByType[logType].emplace_back(logTime, subject->getEventData());
    }

public:
    LogHistory() { }
    ~LogHistory() { }

    void saveToFile(const std::string& filePath)
    {
        std::ofstream file(filePath);

        if (!file.good())   return;

        for (auto& log : history) {
            file << log.timestamp << ", ";
            file << getMessageTypeString(log.type) << ": ";
            file << log.message << '\n';
        }

        file.close();
    }
};

#endif // __LOG_HISTORY__
