#include "Logger/Logger.h"

#include "MemoryAllocator/MemoryAllocator.h"

int main()
{
    LOG_Init(LOG_INIT_CONSOLE_LOGGER);

    LOG_Quit();

    return 0;
}
