/*=============================================================================
#  Author:          dantezhu - http://www.vimer.cn
#  Email:           zny2008@gmail.com
#  FileName:        main.cpp
#  Description:     fuload so编写实例代码
#  Version:         1.0
#  LastChange:      2010-11-28 17:45:34
#  History:         
=============================================================================*/
#include <iostream>
#include <string>
#include <vector>
using namespace std;

/**
 * @brief   第一次进入so时，需要做的初始化工作，只会执行一次。
 *
 * @return  0               succ
 *          else            fail
 */
extern "C" int fuload_handle_init()
{
    return 0;
}

/**
 * @brief   业务逻辑，每次进入
 *
 * @param   vecParams       将输入数据按照空格拆分之后的vector
 * @param   strArgs         输入数据原样数据
 *
 * @return  0               succ
 *          else            返回值,会用来做统计
 */
extern "C" int fuload_handle_process(const vector<string>& vecParams,const string& strArgs)
{
    return 0;
}

/**
 * @brief   so结束时的收尾工作，只会调用一次。一般不用编写
 *
 * @return  0               succ
 *          else            fail
 */
extern "C" int fuload_handle_fini()
{
    return 0;
}
