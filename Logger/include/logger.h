#ifndef E_HORIZON_LOGGER_H
#define E_HORIZON_LOGGER_H
#include <string>

    class Logger
    {
    public:
        static Logger *getInstance();
        static void vSetLogLevel(unsigned char ucLogLevel);
        static void vSetLogFile(const std::string &szFileName);
        static int PrintLog(unsigned char log_level, const char *fileName, int lineNum, const char *format, ...);

    private:
        static Logger *m_pLogger;
        static std::string m_szLogFile;
        static unsigned char m_ucLogLevel;
        static int m_fd;
        static bool m_bLocked;

        static bool bIsEnabled(unsigned char &ucLogLevel);
        static void vPrintPreamble(unsigned char log_level);
        static void vPrintTag(unsigned char log_level);
        static void vPrintTime();
        static void vPrintPID();
        static bool bLock();
        static bool bUnlock();
    };
#define LOG_ERROR 1
#define LOG_WARN 2
#define LOG_INFO 4
#define LOG_DEBUG 8

#define SET_LOG_LEVEL(ucLogLevel) eHorizon::Logger::getInstance()->vSetLogLevel(ucLogLevel)
#define SET_LOG_FILE(ucLogFile) eHorizon::Logger::getInstance()->vSetLogFile(ucLogFile)

#define DEBUG_L1(format, ...) eHorizon::Logger::getInstance()->PrintLog(LOG_ERROR, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define DEBUG_L2(format, ...) eHorizon::Logger::getInstance()->PrintLog(LOG_WARN, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define DEBUG_L3(format, ...) eHorizon::Logger::getInstance()->PrintLog(LOG_INFO, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define DEBUG_L4(format, ...) eHorizon::Logger::getInstance()->PrintLog(LOG_DEBUG, __FILE__, __LINE__, format, ##__VA_ARGS__)
#endif
