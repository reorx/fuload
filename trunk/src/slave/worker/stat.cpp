/*=============================================================================
#  Author:          DanteZhu - http://www.vimer.cn
#  Email:           dantezhu@vip.qq.com
#  FileName:        stat.cpp
#  Description:     
#  Version:         1.0
#  LastChange:      2010-10-08 15:44:45
#  History:         
=============================================================================*/
#include "stat.h"
CStatInfo* CStatInfo::_ins = NULL;
CStatInfo::CStatInfo()
{
    _stat_buf = NULL;
    _stat_desc = NULL;
    _stat_num = 0;  
}

CStatInfo::~CStatInfo()
{
    if ( _stat_buf )
    {
        int length = _stat_num * sizeof(atomic_t);
        munmap((void*)_stat_buf, length);
        if (_ins != NULL)
        {
            delete this;
            _ins = NULL;
        }
    }
    _stat_buf = NULL;
    _stat_desc = NULL;
    _stat_num = 0;      
}

CStatInfo* CStatInfo::Ins()
{
    if (_ins == NULL)
        _ins = new CStatInfo();
    
    return _ins;
}

int CStatInfo::Init(const char* file_name,const char * const stat_desc[],int stat_num)
{
    int fd;
    int length = stat_num * sizeof(atomic_t);
    fd = open(file_name, O_RDWR | O_CREAT, 0644);
    if(fd < 0)  return -1;
    ftruncate(fd, length);
    _stat_buf = (atomic_t *) mmap(0, length, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if(_stat_buf == NULL)
    {
        close(fd);
        return -2;
    }
    close(fd);
    _stat_num = stat_num; 
    _stat_desc = stat_desc;

    return 0;
}

void CStatInfo::ResetStat()
{
    int length = _stat_num * sizeof(atomic_t);
    memset((char *)_stat_buf, 0, length);
    msync((void*)_stat_buf, length, MS_SYNC);
    
    return ;
}

void CStatInfo::AddCount(int index)
{
    atomic_inc(&_stat_buf[index]);
    
    return ;
}
int  CStatInfo::SetCount(int index,int value)
{
    atomic_set(&_stat_buf[index],value);
    
    return 0;
}
int CStatInfo::GetCount(int index)
{
    return atomic_read(&_stat_buf[index]);
}

void CStatInfo::ShowStatInfo(int num)
{
    if(num<0 || num>_stat_num)
    {
        num=_stat_num;
    }
    char buf[512];
    string tmp_buf;

    for (int i = 0; i < num; i ++)
    {
        buf[0] = '\0';
        snprintf(buf,sizeof(buf), "%-40s:  %-20d\n", 
                _stat_desc[i], 
                atomic_read(&_stat_buf[i]));
        tmp_buf += buf;
    }
    tmp_buf+="\n";
    printf("%s",tmp_buf.c_str());

    return ;
}
