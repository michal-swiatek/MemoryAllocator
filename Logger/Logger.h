#ifndef __LOGGER__
#define __LOGGER__

#include "Internal/ILogger.h"

#include "Internal/ConsoleLogger.h"
#include "Internal/FileLogger.h"
#include "Internal/LogHistory.h"

const bool LOG_ENABLED = true;
const bool LOG_DISABLED = false;

enum LOG_InitFlags
{
    LOG_INIT_NONE               = 0,
    LOG_INIT_LOG_HISTORY        = 1,
    LOG_INIT_CONSOLE_LOGGER     = 2,
    LOG_INIT_FILE_LOGGER        = 4,
    LOG_INIT_EVERYTHING         = 7
};

enum LOG_Subsystems
{
    LOG_SUBSYSTEM_NONE               = 0,
    LOG_SUBSYSTEM_LOG_HISTORY        = 1,
    LOG_SUBSYSTEM_CONSOLE_LOGGER     = 2,
    LOG_SUBSYSTEM_FILE_LOGGER        = 4,
    LOG_SUBSYSTEM_EVERYTHING         = 7
};

extern ILogger* logger;

using LOG_Subsystem = Observer<LOG_MessageType, std::string>;
using LOG_Subject = Subject<LOG_MessageType, std::string>;

bool LOG_Init(int flags);
bool LOG_Quit();

int  LOG_EnableSubsystems(int flags);
void LOG_DisableSubsystems(int flags);
int  LOG_ActiveSubsystems();

void LOG_EnableMessages(int flags);
void LOG_DisableMessages(int flags);
int  LOG_ActiveMessages();

void LOG_EnableSubsytemMessages(LOG_Subsystems sybsystem, int messageFlags);
void LOG_EnableCustomSubsystem(LOG_Subsystem* subsystem, int messageFlags);
void LOG_DisableSubsytemMessages(LOG_Subsystems sybsystem, int messageFlags);
void LOG_DisableCustomSubsystem(LOG_Subsystem* subsystem, int messageFlags);

void LOG_SaveHistoryToFile(const std::string& filePath);

void LOG_SetFileLoggerOutput(const std::string& filePath);

#endif // __LOGGER__
