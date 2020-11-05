#include <logger.h>
#include <string>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdarg.h>
#include <sys/syscall.h> /* For SYS_xxx definitions */

    Logger *Logger::m_pLogger = NULL;
    int Logger::m_fd = fileno(stdout);
    std::string Logger::m_szLogFile = "";
    unsigned char Logger::m_ucLogLevel = 3;
    bool Logger::m_bLocked = false;

    Logger *Logger::getInstance()
    {
        if (!m_pLogger)
        {
            m_pLogger = new Logger();
            if (!m_pLogger)
            {
                printf("Failed to allocate memory for Logger!\n");
            }
        }
        return m_pLogger;
    }

    void Logger::vSetLogLevel(unsigned char ucLogLevel)
    {
        m_ucLogLevel = ucLogLevel;
    }

    void Logger::vSetLogFile(const std::string &szFileName)
    {
        if (szFileName != m_szLogFile)
        {
            if (m_fd != fileno(stdout))
            {
                close(m_fd);
            }

            if (!szFileName.empty())
            {
                m_fd = open(szFileName.c_str(), O_RDWR | O_CREAT | O_APPEND, S_IRWXU | S_IRGRP | S_IROTH);
                if (m_fd == -1)
                {
                    printf("Failed to open the file:<%s> due to error code:<%d> and error:<%s>\n", szFileName.c_str(), errno, strerror(errno));
                    m_szLogFile = "";
                    m_fd = fileno(stdout);
                }
                else
                {
                    m_szLogFile = szFileName;
                }
            }
            else
            {
                m_szLogFile = szFileName;
                m_fd = fileno(stdout);
            }
        }
    }

    bool Logger::bIsEnabled(unsigned char &ucLogLevel)
    {
        return (ucLogLevel & m_ucLogLevel);
    }

    bool Logger::bLock()
    {
        bool bRet = true;
        if (0 != flock(m_fd, LOCK_EX))
        {
            printf("Failed to lock the file with errno:<%d> and error:<%s>!\n", errno, strerror(errno));
            bRet = false;
        }
        else
        {
            m_bLocked = true;
        }

        return bRet;
    }

    bool Logger::bUnlock()
    {
        bool bRet = true;
        if (m_bLocked)
        {
            if (0 != flock(m_fd, LOCK_UN))
            {
                printf("Failed to unlock the file with errno:<%d> and error:<%s>!\n", errno, strerror(errno));
                bRet = false;
            }
            else
            {
                m_bLocked = false;
            }
        }
        else
        {
            m_bLocked = false;
        }

        return bRet;
    }

    void Logger::vPrintTag(unsigned char log_level)
    {
        switch (log_level)
        {
        case LOG_ERROR:
            dprintf(m_fd, "[ERR]");
            break;
        case LOG_WARN:
            dprintf(m_fd, "[WRN]");
            break;
        case LOG_INFO:
            dprintf(m_fd, "[INFO]");
            break;
        case LOG_DEBUG:
            dprintf(m_fd, "[DBG]");
            break;
        }
    }

    void Logger::vPrintTime()
    {
        time_t currTime = time(NULL);
        struct timeval tv;
        struct timezone tz;
        gettimeofday(&tv, &tz);
        struct tm *tmTime = gmtime(&currTime);
        dprintf(m_fd, "[%02d-%02d-%d %02d:%02d:%02d.%06ld]", tmTime->tm_mday, tmTime->tm_mon, tmTime->tm_year + 1900, tmTime->tm_hour, tmTime->tm_min, tmTime->tm_sec, tv.tv_usec);
    }

    void Logger::vPrintPID()
    {
        pid_t pid, tid;
        pid = getpid();
        tid = syscall(SYS_gettid);
        if (pid == tid)
        {
            dprintf(m_fd, "[PID=%d]", pid);
        }
        else
        {
            dprintf(m_fd, "[PID=%d][TID=%d]", pid, tid);
        }
    }

    void Logger::vPrintPreamble(unsigned char log_level)
    {
        vPrintTime();
        vPrintPID();
        vPrintTag(log_level);
    }

    int Logger::PrintLog(unsigned char log_level, const char *fileName, int lineNum, const char *format, ...)
    {
        int writeLen = 0;
        if (bIsEnabled(log_level))
        {
            if (bLock())
            {
                vPrintPreamble(log_level);
                dprintf(m_fd, "[%s:%d]", fileName, lineNum);
                va_list ap;
                va_start(ap, format);
                writeLen = vdprintf(m_fd, format, ap);
                va_end(ap);
            }
            bUnlock();
        }
        return writeLen;
    }
