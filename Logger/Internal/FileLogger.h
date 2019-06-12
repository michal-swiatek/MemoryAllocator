#ifndef __FILE_LOGGER__
#define __FILE_LOGGER__

#include <ctime>
#include <chrono>
#include <fstream>
#include <iostream>

#include "ILogger.h"

class FileLogger : public Observer<LOG_MessageType, std::string>
{
private:
    bool fileOpen;

    std::string currentFilePath;
    std::ofstream currentFile;

    void updateObserver(const Subject<LOG_MessageType, std::string>* subject) override
    {
        if (fileOpen) {
            std::time_t logTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            std::string time = std::ctime(&logTime);
            time.erase(time.length() - 1);

            currentFile << time << ", ";
            currentFile << getMessageTypeString(subject->getEventType()) << ": " << subject->getEventData() << '\n';
        }
    }

public:
    FileLogger(const std::string& filePath) : fileOpen(false), currentFilePath(filePath) { setLoggerOutput(currentFilePath); }
    ~FileLogger() { currentFile.close(); }

    void setLoggerOutput(const std::string& filePath)
    {
        if (currentFile.is_open())
            currentFile.close();

        currentFilePath = filePath;
        currentFile.open(filePath);

        if (!currentFile.good()) {
            std::cerr << "Unable to open file " << filePath << '\n';
            return;
        }

        fileOpen = true;
    }
};

#endif // __FILE_LOGGER__
