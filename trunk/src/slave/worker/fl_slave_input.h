/*=============================================================================
#  Author:          dantezhu - http://www.vimer.cn
#  Email:           zny2008@gmail.com
#  FileName:        fl_slave_input.h
#  Description:     负责处理给slave_worker的input数据
#  Version:         1.0
#  LastChange:      2010-11-28 21:09:31
#  History:         
=============================================================================*/
#ifndef _FL_SLAVE_INPUT_H_
#define _FL_SLAVE_INPUT_H_
#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "fl_slave_func.h"
#include "fl_weight_route.h"
using namespace std;

typedef struct _stSWInput
{
    map<string,string> mapParams;
    string strInputLine;
}StSWInput;
class CFLSlaveInput 
{
    public:
        CFLSlaveInput (){}
        virtual ~CFLSlaveInput (){}

        /**
         * @brief   设置输入数据
         *
         * @param   strInputData    数据
         *
         * @return  0               succ
         *          else            fail
         */
        int SetInputData(const string& strInputData);
        /**
         * @brief   分配一个node
         *
         * @param   node            node
         *
         * @return  0               succ
         *          else            fail
         */
        int Alloc(StSWInput& node);

    private:
        /**
         * @brief   解析输入数据
         *
         * @param   strInputData    数据
         *
         * @return  0               succ
         *          else            fail
         */
        int parseInputData(const string& strInputData);
    private:
        string m_LastInputData;
        CFLWeightRoute<StSWInput> m_SWInputRoute;
};

#endif
