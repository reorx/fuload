#include "fl_log.h"
#include<sys/file.h>


CFLLog* CFLLog::m_instance = NULL;

char gLogLevelName[LOG_LEVEL_MAXNUM][16] = {    
    "NONE",
    "TRACE",
    "DEBUG",
    "WARNING",
    "INFO",
    "ERROR",
    "FATAL"
};
////////////////////////////////////////////////////////////////////////////////

int APILogInit(LogLevel logLevel, const char* logDir, const char* logName, unsigned long logSize=LOG_DEFAULT_SIZE)
{
    return CFLLog::instance()->Init(logLevel,logDir,logName,logSize);
}
int APILogWrite(LogLevel logLevel, const char* logFormat, ...)
{
    va_list ap;
    va_start(ap, logFormat);
    int ret = CFLLog::instance()->VWriteLog(logLevel,logFormat,ap);
    va_end(ap);

    return ret;
}

////////////////////////////////////////////////////////////////////////////////
// constructor & destructor
CFLLog::CFLLog()
{
    m_LogBuf = NULL;
    m_LogFileInfo.fd = -1;
    m_LogFileInfo.seq = 0;
}
CFLLog::~CFLLog()
{
    if (m_LogFileInfo.fd > 0)
    {
        close(m_LogFileInfo.fd);
        m_LogFileInfo.fd = -1;
    }

    if (m_instance != NULL)
    {
        delete(m_instance);
        m_instance = NULL;
    }   
    if (m_LogBuf)
    {
        delete m_LogBuf;
        m_LogBuf = NULL;
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

    m_LogBuf = new char[LOG_MSG_SIZE];

    return 0;
}


int CFLLog::WriteLog(LogLevel logLevel, const char* logFormat, ...)
{   
    va_list ap;
    va_start(ap, logFormat);
    int ret = VWriteLog(logLevel,logFormat,ap);
    va_end(ap);

    return ret;
}
int CFLLog::VWriteLog(LogLevel logLevel, const char* logFormat, va_list ap)
{   
    if (logLevel < m_LogLevel)
        return 0;

    time_t now = time(NULL);
    struct tm tm;
    localtime_r(&now, &tm);

    char temp_time[64];
    strftime(temp_time,sizeof(temp_time)-1,"%Y-%m-%d %H:%M:%S",&tm);

    int fp;

    ShiftLogFiles(logLevel, m_LogSize, LOG_DEFAULT_MAXNUM);
    fp = OpenLogFile();
    if (fp == -1)
    {
        // 如果打开log文件失败, 从stderr输出
        vfprintf(stderr, logFormat, ap);
        return 0;
    }

    int preLen, infoLen;
    preLen = snprintf(m_LogBuf,LOG_MSG_SIZE, "[%s][%s][%05d]", gLogLevelName[logLevel], temp_time, getpid());

    infoLen = vsnprintf(m_LogBuf + preLen,LOG_MSG_SIZE-preLen, logFormat, ap);
    write(fp, m_LogBuf, preLen + infoLen);  

    return 0;
}

int CFLLog::OpenLogFile()
{
    if (m_LogFileInfo.fd > -1)
    {
        return m_LogFileInfo.fd;
    }

    if (m_LogDir == NULL || m_LogName == NULL)
    {
        return -1;
    }

    char LogFile[256];
    GetLogFileName(LogFile, 256);

    int fp = open(LogFile, O_WRONLY|O_CREAT|O_APPEND, 0644);
    if (fp < 0)
    {
        fprintf(stderr, "open log file error, redirect to stderr. file: %s\n", LogFile);
        return -1;
    }

    int val = fcntl(fp, F_GETFD, 0);
    val |= FD_CLOEXEC;
    fcntl(fp, F_SETFD, val);    

    m_LogFileInfo.fd = fp;

    return m_LogFileInfo.fd;
}

int CFLLog::GetLogFileName(char* logFile, int iLen)
{
    //寻找当前日期的日志文件最大序号
    for (int seq = m_LogFileInfo.seq + 1; seq < LOG_DEFAULT_MAXNUM; seq++) 
    {
        snprintf(logFile,iLen,"%s/%s.log.%d", m_LogDir, m_LogName, seq);

        if (access(logFile, F_OK)) 
        {
            m_LogFileInfo.seq = seq - 1;
            if (seq == 1)
            {
                snprintf(logFile,iLen, "%s/%s.log", m_LogDir, m_LogName);
            }
            else
            {
                snprintf(logFile,iLen,"%s/%s.log.%d", m_LogDir, m_LogName, seq - 1); 
            }
            return 0;
        }
        else
        {
            continue;
        }
    }

    fprintf(stderr, "too many logfiles...");
    snprintf(logFile, 1024, "%s/%s.log", "/tmp", m_LogName);

    return -1;
}

int CFLLog::ShiftLogFiles(LogLevel logLevel, unsigned long maxsize, unsigned short maxnum)
{
    if (m_LogFileInfo.fd < 0)
    {
        return -1;
    }

    long length = lseek(m_LogFileInfo.fd, 0, SEEK_END);
    if (length < -1)
    {
        return -1;
    }

    if (length < (long) maxsize)
    {
        return 0;
    }   

    close(m_LogFileInfo.fd);
    m_LogFileInfo.fd = -1;
    m_LogFileInfo.seq++;

    return 0;
}
