#ifndef _FL_LOG_H_
#define _FL_LOG_H_

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <unistd.h>

#include <sys/mman.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <assert.h>

////////////////////////////////////////////////////////////////////////////////
// 直接调用的宏
#define log_init(lvl, path, args...)    APILogInit(lvl, path, ##args) //log类初始化

#define LOG_DETAIL(lvl, fmt, args...)   APILogWrite(lvl, "[%s:%u][%s] " fmt "\n", __FILE__, __LINE__, __FUNCTION__, ##args)

#define trace_log(fmt, args...)     LOG_DETAIL(LM_TRACE, fmt, ##args)
#define debug_log(fmt, args...)     LOG_DETAIL(LM_DEBUG, fmt, ##args)
#define warn_log(fmt, args...)      LOG_DETAIL(LM_WARNING, fmt, ##args)
#define info_log(fmt, args...)      LOG_DETAIL(LM_INFO, fmt, ##args)
#define error_log(fmt, args...)     LOG_DETAIL(LM_ERROR, fmt, ##args)
#define fatal_log(fmt, args...)     LOG_DETAIL(LM_FATAL, fmt, ##args)

#define screen_info(fmt, args...)   fprintf(stdout, fmt, ##args); fprintf(stdout, "\n")


inline void screen_output(const char* fmt, ...)
{
    time_t now = time(NULL);
    struct tm tm;
    localtime_r(&now, &tm);
    char temp_time[64];
    strftime(temp_time,sizeof(temp_time)-1,"%Y-%m-%d %H:%M:%S",&tm);

    fprintf(stdout, "[%s][%05d]", temp_time, getpid());

    va_list ap;
    va_start(ap, fmt);
    vfprintf(stdout, fmt, ap);
    va_end(ap);
    fprintf(stdout, "\n");
}


////////////////////////////////////////////////////////////////////////////////
// CFLLog
////////////////////////////////////////////////////////////////////////////////

// log文件默认大小
#define LOG_DEFAULT_SIZE        200000000
// 同名log文件默认最大数量
#define LOG_DEFAULT_MAXNUM      1000
// log level的数量
#define LOG_LEVEL_MAXNUM        10

// 单条log的最长大小
#define LOG_MSG_SIZE            4096

typedef enum _Log_Level_
{
    LM_ALL          = 0,
    LM_TRACE        = 1,
    LM_DEBUG        = 2,
    LM_WARNING      = 3,
    LM_INFO         = 4,
    LM_ERROR        = 5,
    LM_FATAL        = 6,
    LM_NONE         = 10
}LogLevel;

typedef struct
{
    int fd;
    int seq;
}LogFileInfo;

class CFLLog
{
    public:
        CFLLog();
        ~CFLLog();

    public:
        int Init(LogLevel logLevel, const char* logDir, const char* logName, unsigned long logSize=LOG_DEFAULT_SIZE);
        int WriteLog(LogLevel logLevel, const char* logFormat, ...);
        int VWriteLog(LogLevel logLevel, const char* logFormat,va_list ap);

    private:
        int OpenLogFile();
        int GetLogFileName(char* logFile, int iLen);
        int ShiftLogFiles(LogLevel logLevel, unsigned long maxsize, unsigned short maxnum);

    private:
        char* m_LogBuf;
        LogLevel m_LogLevel;
        char m_LogDir[256];
        char m_LogName[64];
        unsigned long m_LogSize;

        LogFileInfo m_LogFileInfo;

    public:
        static CFLLog* m_instance;
        static CFLLog* instance();
};

extern int APILogInit(LogLevel logLevel, const char* logDir, const char* logName, unsigned long logSize);
extern int APILogWrite(LogLevel logLevel, const char* logFormat, ...);

#endif
