#ifndef __CONSOLE_LOGGER__
#define __CONSOLE_LOGGER__

#include <iostream>

#include "ILogger.h"

class ConsoleLogger : public Observer<LOG_MessageType, std::string>
{
public:
    ConsoleLogger() { }
    ~ConsoleLogger() { }

    void updateObserver(const Subject<LOG_MessageType, std::string>* subject) override
    {
        std::cerr << getMessageTypeString(subject->getEventType()) << ": " << subject->getEventData() << '\n';
    }
};

#endif // __CONSOLE_LOGGER__
