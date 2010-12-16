#include "fl_log.h"
#include<sys/file.h>


CFLLog* CFLLog::m_instance = NULL;

char gLogLevelName[LOG_LEVEL_MAXNUM][16] = {    
    "none",
    "trace",
    "debug",
    "warning",
    "info",
    "error",
    "fatal"
};


////////////////////////////////////////////////////////////////////////////////
// constructor & destructor
CFLLog::CFLLog()
{
    for (int i=0; i<LOG_LEVEL_MAXNUM; i++)
    {
        m_LogFileInfos[i].fd = -1;
        m_LogFileInfos[i].seq = 0;
    }
}
CFLLog::~CFLLog()
{
    for (int i=0; i<LOG_LEVEL_MAXNUM; i++)
    {
        if (m_LogFileInfos[i].fd > 0)
        {
            close(m_LogFileInfos[i].fd);
            m_LogFileInfos[i].fd = -1;
        }
    }

    if (m_instance != NULL)
    {
        delete(m_instance);
        m_instance = NULL;
    }   
}


////////////////////////////////////////////////////////////////////////////////
// instance
CFLLog* CFLLog::instance()
{
    if (m_instance == NULL)
    {
        m_instance = new CFLLog();
    }
    return m_instance;
}


////////////////////////////////////////////////////////////////////////////////
// main functions
int CFLLog::Init(LogLevel logLevel, const char* logDir, const char* logName, unsigned long logSize)
{
    if (logLevel < LM_ALL || logLevel > LM_NONE)
    {
        fprintf(stderr, "the logLevel error, logLevel: %d\n", logLevel);
        return -1;
    }
    m_LogLevel = logLevel;

    if (logSize > 0)
        m_LogSize = logSize;
    else
        m_LogSize = LOG_DEFAULT_SIZE;

    if (logDir != NULL)
    {
        strncpy(m_LogDir, logDir, sizeof(m_LogDir) - 1);
        m_LogDir[sizeof(m_LogDir) - 1] = '\0';

        if (access(m_LogDir, F_OK | X_OK | W_OK) != 0)
        {
            fprintf(stderr, "the logdir do not exist or do not have wx. dir: %s\n", logDir);
            return -1;
        }
    }
    else
    {
        memset(m_LogDir, 0, sizeof(m_LogDir));
    }

    if (logName != NULL)
    {
        strncpy(m_LogName, logName, sizeof(m_LogName) - 1);
        m_LogName[sizeof(m_LogName) - 1] = '\0';
    }
    else
    {
        memset(m_LogName, 0, sizeof(m_LogName));
    }

    m_LogBuf = (char *)mmap(0, 1024, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (m_LogBuf == MAP_FAILED)
    {
        fprintf(stderr, "mmap log buffer failed.\n");
        return -1;
    }

    return 0;
}


int CFLLog::WriteLog(LogLevel logLevel, const char* logFormat, ...)
{   
    if (logLevel < m_LogLevel)
        return 0;

    time_t now = time(NULL);
    struct tm tm;
    localtime_r(&now, &tm);

    va_list ap;
    int fp;

    ShiftLogFiles(logLevel, m_LogSize, LOG_DEFAULT_MAXNUM);
    fp = OpenLogFile(logLevel);
    if (fp == -1)
    {
        // 如果打开log文件失败, 从stderr输出
        va_start(ap, logFormat);
        vfprintf(stderr, logFormat, ap);
        va_end(ap);
        return 0;
    }

    int preLen, infoLen;
    preLen = snprintf(m_LogBuf,1024, "[%02d:%02d:%02d][%05d]", tm.tm_hour, tm.tm_min, tm.tm_sec, getpid());

    va_start(ap, logFormat);
    infoLen = vsnprintf(m_LogBuf + preLen,1024-1-preLen, logFormat, ap);
    va_end(ap);
    write(fp, m_LogBuf, preLen + infoLen);  

    return 0;
}

int CFLLog::OpenLogFile(LogLevel logLevel)
{
    if (m_LogFileInfos[logLevel].fd > -1)
    {
        return m_LogFileInfos[logLevel].fd;
    }

    if (m_LogDir == NULL || m_LogName == NULL)
    {
        return -1;
    }

    char LogFile[256];
    GetLogFileName(LogFile, 256,logLevel);

    int fp = open(LogFile, O_WRONLY|O_CREAT|O_APPEND, 0644);
    if (fp < 0)
    {
        fprintf(stderr, "open log file error, redirect to stderr. file: %s\n", LogFile);
        return -1;
    }

    int val = fcntl(fp, F_GETFD, 0);
    val |= FD_CLOEXEC;
    fcntl(fp, F_SETFD, val);    

    m_LogFileInfos[logLevel].fd = fp;

    return m_LogFileInfos[logLevel].fd;
}

int CFLLog::GetLogFileName(char* logFile, int iLen ,LogLevel logLevel)
{
    //寻找当前日期的日志文件最大序号
    for (int seq = m_LogFileInfos[logLevel].seq + 1; seq < LOG_DEFAULT_MAXNUM; seq++) 
    {
        snprintf(logFile,iLen,"%s/%s.%s.log.%d", m_LogDir, m_LogName, gLogLevelName[logLevel],seq);

        if (access(logFile, F_OK)) 
        {
            m_LogFileInfos[logLevel].seq = seq - 1;
            if (seq == 1)
            {
                snprintf(logFile,iLen, "%s/%s.%s.log", m_LogDir, m_LogName, gLogLevelName[logLevel]);
            }
            else
            {
                snprintf(logFile,iLen,"%s/%s.%s.log.%d", m_LogDir, m_LogName, gLogLevelName[logLevel],seq - 1); 
            }
            return 0;
        }
        else
        {
            continue;
        }
    }

    fprintf(stderr, "too many logfiles...");
    snprintf(logFile, 1024, "%s/%s.%s.log", "/tmp", m_LogName, gLogLevelName[logLevel]);

    return -1;
}

int CFLLog::ShiftLogFiles(LogLevel logLevel, unsigned long maxsize, unsigned short maxnum)
{
    if (m_LogFileInfos[logLevel].fd < 0)
    {
        return -1;
    }

    long length = lseek(m_LogFileInfos[logLevel].fd, 0, SEEK_END);
    if (length < -1)
    {
        return -1;
    }

    if (length < (long) maxsize)
    {
        return 0;
    }   

    close(m_LogFileInfos[logLevel].fd);
    m_LogFileInfos[logLevel].fd = -1;
    m_LogFileInfos[logLevel].seq++;

    return 0;
}
