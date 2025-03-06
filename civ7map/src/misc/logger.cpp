#include "logger.h"

Logger g_logger;

//--------------------------------------------------------------------------------------
void Logger::Log(Level _level, const char * _format, ...)
{
    const int bufferSize = 4096;
    char tempBuffer[bufferSize];

    if (_format)
    {
        tempBuffer[0] = 0;
        va_list args;
        va_start(args, _format);
        vsnprintf_s(tempBuffer, bufferSize - 1, _format, args);
        m_entries.emplace_back(LogEntry{ _level, tempBuffer });
        ::OutputDebugString(tempBuffer);
        ::OutputDebugString("\n");
        va_end(args);
    }
}

//--------------------------------------------------------------------------------------
void Logger::Clear()
{
    m_entries.clear();
}

//--------------------------------------------------------------------------------------
const int Logger::GetLogCount() const
{
    return (int)m_entries.size();
}

//--------------------------------------------------------------------------------------
const LogEntry & Logger::GetLog(int _index) const
{
    return m_entries[_index];
}