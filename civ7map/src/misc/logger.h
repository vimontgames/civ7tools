#pragma once

#define LOG_INFO(...)         g_logger.Log(Level::Info, __VA_ARGS__)
#define LOG_WARNING(...)      g_logger.Log(Level::Warning, __VA_ARGS__) 
#define LOG_ERROR(...)        g_logger.Log(Level::Error, __VA_ARGS__) 

enum class Level
{
    Info = 0,
    Warning,
    Error
};

enum class LevelFlags
{
    Info = 1 << (u32)Level::Info,
    Warning = 1 << (u32)Level::Warning,
    Error = 1 << (u32)Level::Error
};

struct LogEntry
{
    Level level;
    std::string message;
};

class Logger
{
public:
    void Init();
    void Deinit();
    void Log(Level _level, const char * _format, ...);
    void Clear();

    void Lock();
    void Unlock();

    const int GetLogCount() const;
    const LogEntry & GetLog(int _index) const;

protected:
    void                    log(Level _level, const char * _msg);

private:
    std::vector<LogEntry>   m_entries;
};

extern Logger g_logger;