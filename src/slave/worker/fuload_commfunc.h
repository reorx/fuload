/*=============================================================================
# Author: dantezhu - http://www.vimer.cn
# Email: zny2008@gmail.com
# FileName: comm_func.h
# Description: 
# Version: 1.0
# LastChange: 2010-11-28 18:13:22
# History: 
=============================================================================*/
#ifndef _FULOAD_COMMFUNC_H_
#define _FULOAD_COMMFUNC_H_
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>

#include "fuload_commdef.h"

using namespace std;

inline int SplitString(const string &srcStr,const string &splitStr,vector<string> &destVec)
{
    if(srcStr.size()==0)
    { 
        return 0;
    } 
    size_t oldPos,newPos;
    oldPos=0;
    newPos=0;
    string tempData;
    while(1)
    { 
        newPos=srcStr.find(splitStr,oldPos);
        if(newPos!=string::npos)
        { 
            tempData = srcStr.substr(oldPos,newPos-oldPos);
            destVec.push_back(tempData);
            oldPos=newPos+splitStr.size();
        } 
        else if(oldPos<=srcStr.size())
        { 
            tempData= srcStr.substr(oldPos);
            destVec.push_back(tempData);
            break;
        } 
        else
        { 
            break;
        } 
    } 
    return 0;
}
string replace_all(const string& src,const string& old_value,const string& new_value) 
{ 
    string str = src;
    while(true) { 
        string::size_type pos(0); 
        if( (pos=str.find(old_value))!=string::npos ) 
            str.replace(pos,old_value.length(),new_value); 
        else break; 
    } 
    return str; 
} 

string replace_all_distinct(const string& src,const string& old_value,const string& new_value) 
{ 
    string str = src;
    for(string::size_type pos(0); pos!=string::npos; pos+=new_value.length()) { 
        if( (pos=str.find(old_value,pos))!=string::npos ) 
            str.replace(pos,old_value.length(),new_value); 
        else break; 
    } 
    return str; 
} 
#endif
