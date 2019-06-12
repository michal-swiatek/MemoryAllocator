#include "../Logger.h"

ILogger* logger = nullptr;

namespace {
    ConsoleLogger* consoleLogger = nullptr;
    FileLogger* fileLogger = nullptr;
    LogHistory* logHistory = nullptr;

    int activeSubsystems = LOG_SUBSYSTEM_EVERYTHING;
    int activeMessages = LOG_MSG_ALL;
}

bool LOG_Init(int flags)
{
    if (!logger)
        logger = new ILogger();

    if (LOG_EnableSubsystems(flags) != flags)   return false;

    return true;
}

bool LOG_Quit()
{
    LOG_DisableSubsystems(LOG_SUBSYSTEM_EVERYTHING);

    if (logger)
        delete logger;

    return true;
}

int LOG_EnableSubsystems(int flags)
{
    int successFlag = activeSubsystems;

    //
    //      CONSOLE LOGGER
    //

    if (flags & LOG_SUBSYSTEM_CONSOLE_LOGGER && !consoleLogger) {
        consoleLogger = new ConsoleLogger();

        logger->attach(LOG_MSG_ERROR,   consoleLogger);
        logger->attach(LOG_MSG_WARNING, consoleLogger);
        logger->attach(LOG_MSG_INFO,    consoleLogger);
        logger->attach(LOG_MSG_GENERAL, consoleLogger);

        successFlag |= LOG_SUBSYSTEM_CONSOLE_LOGGER;
    }

    //
    //      FILE LOGGER
    //

    if (flags & LOG_SUBSYSTEM_FILE_LOGGER && !fileLogger) {
        fileLogger = new FileLogger("logErrors.txt");

        logger->attach(LOG_MSG_ERROR, fileLogger);

        successFlag |= LOG_SUBSYSTEM_FILE_LOGGER;
    }

    //
    //      LOG HISTORY
    //

    if (flags & LOG_SUBSYSTEM_LOG_HISTORY && !logHistory) {
        logHistory = new LogHistory();

        logger->attach(LOG_MSG_ERROR,   logHistory);
        logger->attach(LOG_MSG_WARNING, logHistory);
        logger->attach(LOG_MSG_INFO,    logHistory);
        logger->attach(LOG_MSG_GENERAL, logHistory);
        logger->attach(LOG_MSG_COMMAND, logHistory);

        successFlag |= LOG_SUBSYSTEM_LOG_HISTORY;
    }

    activeSubsystems = successFlag;

    return successFlag;
}

void LOG_DisableSubsystems(int flags)
{
    activeSubsystems ^= flags & activeSubsystems;

    //
    //      CONSOLE LOGGER
    //

    if (flags & LOG_INIT_CONSOLE_LOGGER && consoleLogger) {
        logger->detachFromAllEvents(consoleLogger);

        delete consoleLogger;
        consoleLogger = nullptr;
    }

    //
    //      FILE LOGGER
    //

    if (flags & LOG_INIT_FILE_LOGGER && fileLogger) {
        logger->detachFromAllEvents(fileLogger);

        delete fileLogger;
        fileLogger = nullptr;
    }

    //
    //      LOG HISTORY
    //

    if (flags & LOG_INIT_LOG_HISTORY && logHistory) {
        logger->detachFromAllEvents(logHistory);

        delete logHistory;
        logHistory = nullptr;
    }
}

int LOG_ActiveSubsystems()
{
    return activeSubsystems;
}

void LOG_EnableMessages(int flags)
{
    activeMessages |= flags;

    if (flags & LOG_MSG_ERROR)      logger->setMessageTypeStatus(LOG_MSG_ERROR, LOG_ENABLED);
    if (flags & LOG_MSG_WARNING)    logger->setMessageTypeStatus(LOG_MSG_WARNING, LOG_ENABLED);
    if (flags & LOG_MSG_INFO)       logger->setMessageTypeStatus(LOG_MSG_INFO, LOG_ENABLED);
    if (flags & LOG_MSG_GENERAL)    logger->setMessageTypeStatus(LOG_MSG_GENERAL, LOG_ENABLED);
    if (flags & LOG_MSG_COMMAND)    logger->setMessageTypeStatus(LOG_MSG_COMMAND, LOG_ENABLED);
}

void LOG_DisableMessages(int flags)
{
    activeMessages ^= activeMessages & flags;

    if (flags & LOG_MSG_ERROR)      logger->setMessageTypeStatus(LOG_MSG_ERROR, LOG_DISABLED);
    if (flags & LOG_MSG_WARNING)    logger->setMessageTypeStatus(LOG_MSG_WARNING, LOG_DISABLED);
    if (flags & LOG_MSG_INFO)       logger->setMessageTypeStatus(LOG_MSG_INFO, LOG_DISABLED);
    if (flags & LOG_MSG_GENERAL)    logger->setMessageTypeStatus(LOG_MSG_GENERAL, LOG_DISABLED);
    if (flags & LOG_MSG_COMMAND)    logger->setMessageTypeStatus(LOG_MSG_COMMAND, LOG_DISABLED);
}

int LOG_ActiveMessages()
{
    return activeMessages;
}

Observer<LOG_MessageType, std::string>* getObserverPointer(LOG_Subsystems subsystem)
{
    if (subsystem & LOG_SUBSYSTEM_CONSOLE_LOGGER)   return consoleLogger;
    if (subsystem & LOG_SUBSYSTEM_FILE_LOGGER)      return fileLogger;
    if (subsystem & LOG_SUBSYSTEM_LOG_HISTORY)      return logHistory;
}

void LOG_EnableSubsytemMessages(LOG_Subsystems subsystem, int messageFlags)
{
    if (messageFlags & LOG_MSG_ERROR)      logger->attach(LOG_MSG_ERROR, getObserverPointer(subsystem));
    if (messageFlags & LOG_MSG_WARNING)    logger->attach(LOG_MSG_WARNING, getObserverPointer(subsystem));
    if (messageFlags & LOG_MSG_INFO)       logger->attach(LOG_MSG_INFO, getObserverPointer(subsystem));
    if (messageFlags & LOG_MSG_GENERAL)    logger->attach(LOG_MSG_GENERAL, getObserverPointer(subsystem));
    if (messageFlags & LOG_MSG_COMMAND)    logger->attach(LOG_MSG_COMMAND, getObserverPointer(subsystem));
}

void LOG_EnableCustomSubsystem(LOG_Subsystem* subsystem, int messageFlags)
{
    if (messageFlags & LOG_MSG_ERROR)      logger->attach(LOG_MSG_ERROR, subsystem);
    if (messageFlags & LOG_MSG_WARNING)    logger->attach(LOG_MSG_WARNING, subsystem);
    if (messageFlags & LOG_MSG_INFO)       logger->attach(LOG_MSG_INFO, subsystem);
    if (messageFlags & LOG_MSG_GENERAL)    logger->attach(LOG_MSG_GENERAL, subsystem);
    if (messageFlags & LOG_MSG_COMMAND)    logger->attach(LOG_MSG_COMMAND, subsystem);
}

void LOG_DisableSubsytemMessages(LOG_Subsystems subsystem, int messageFlags)
{
    if (messageFlags & LOG_MSG_ERROR)      logger->detach(LOG_MSG_ERROR, getObserverPointer(subsystem));
    if (messageFlags & LOG_MSG_WARNING)    logger->detach(LOG_MSG_WARNING, getObserverPointer(subsystem));
    if (messageFlags & LOG_MSG_INFO)       logger->detach(LOG_MSG_INFO, getObserverPointer(subsystem));
    if (messageFlags & LOG_MSG_GENERAL)    logger->detach(LOG_MSG_GENERAL, getObserverPointer(subsystem));
    if (messageFlags & LOG_MSG_COMMAND)    logger->detach(LOG_MSG_COMMAND, getObserverPointer(subsystem));
}

void LOG_DisableCustomSubsystem(LOG_Subsystem* subsystem, int messageFlags)
{
    if (messageFlags & LOG_MSG_ERROR)      logger->detach(LOG_MSG_ERROR, subsystem);
    if (messageFlags & LOG_MSG_WARNING)    logger->detach(LOG_MSG_WARNING, subsystem);
    if (messageFlags & LOG_MSG_INFO)       logger->detach(LOG_MSG_INFO, subsystem);
    if (messageFlags & LOG_MSG_GENERAL)    logger->detach(LOG_MSG_GENERAL, subsystem);
    if (messageFlags & LOG_MSG_COMMAND)    logger->detach(LOG_MSG_COMMAND, subsystem);
}

void LOG_SaveHistoryToFile(const std::string& filePath)
{
    if (logHistory)
        logHistory->saveToFile(filePath);
}

void LOG_SetFileLoggerOutput(const std::string& filePath)
{
    if (fileLogger)
        fileLogger->setLoggerOutput(filePath);
}
