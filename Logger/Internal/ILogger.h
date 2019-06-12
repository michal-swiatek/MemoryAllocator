#ifndef __LOGGER_INTERFACE__
#define __LOGGER_INTERFACE__

#include <sstream>
#include <utility>
#include <unordered_map>

#include "ObserverPattern.h"

//enum MessageType { MSG_ERROR = 0, MSG_WARNING, MSG_INFO, MSG_GENERAL, MSG_COMMAND, MSG_SIZE };
enum LOG_MessageType
{
    LOG_MSG_NONE        = 0,
    LOG_MSG_ERROR       = 1,
    LOG_MSG_WARNING     = 2,
    LOG_MSG_INFO        = 4,
    LOG_MSG_GENERAL     = 8,
    LOG_MSG_COMMAND     = 16,
    LOG_MSG_ALL         = 31
};

class ILogger : public Subject<LOG_MessageType, std::string>
{
private:
    std::unordered_map<LOG_MessageType, bool, SimpleHash<LOG_MessageType> > messageTypeStatus;

    //  Building messages
    std::stringstream sstr;

    //  Helper functions
    template <typename T>
    void buildMessage(const T& value)
    {
        sstr << value;
    }

    template <typename T, typename... Args>
    void buildMessage(const T& value, const Args&... args)
    {
        sstr << value;
        buildMessage(args...);
    }

public:
    ILogger()
    {
        messageTypeStatus[LOG_MSG_ERROR]    = true;
        messageTypeStatus[LOG_MSG_WARNING]  = true;
        messageTypeStatus[LOG_MSG_INFO]     = true;
        messageTypeStatus[LOG_MSG_GENERAL]  = true;
        messageTypeStatus[LOG_MSG_COMMAND]  = true;
    }

    ~ILogger() { }

    template <typename... Args>
    void log(LOG_MessageType msgType, const Args&... args)
    {
        if (messageTypeStatus[msgType])
        {
            sstr.str(std::string());                    //  Clearing buffer
            buildMessage(std::forward<const Args&>(args)...);  //  Generating new message

            //  Observer pattern
            setEventData(sstr.str());
            notify(msgType);
        }
    }

    void setMessageTypeStatus(LOG_MessageType msgType, bool status)
    {
        messageTypeStatus[msgType] = status;
    }
};

static std::string getMessageTypeString(LOG_MessageType msgType)
{
    switch (msgType)
    {
        case LOG_MSG_ERROR:     return "ERROR";
        case LOG_MSG_WARNING:   return "WARNING";
        case LOG_MSG_INFO:      return "INFO";
        case LOG_MSG_GENERAL:   return "GENERAL";
        case LOG_MSG_COMMAND:   return "COMMAND";
        default:                return "INVALID MESSAGE TYPE";
    }
}

#endif // __LOGGER_INTERFACE__
