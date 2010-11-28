/*=============================================================================
#  Author:          dantezhu - http://www.vimer.cn
#  Email:           zny2008@gmail.com
#  FileName:        weight_route.h
#  Description:     实现一个通用的按照权重分配路由的模版类
#  Version:         1.0
#  LastChange:      2010-11-28 20:19:03
#  History:         
=============================================================================*/
#ifndef _WEIGHT_ROUTE_H_
#define _WEIGHT_ROUTE_H_
#include <iostream>
#include <string>
#include <vector>
using namespace std;

template <typename T>
class CFLWeightRoute 
{
    public:
        CFLWeightRoute () {}
        virtual ~CFLWeightRoute () {}

        inline int add(T node,unsigned weight);
        inline int alloc(T& node);

    private:
        vector<int> m_vecWeightRoute;
        vector<T> m_vecNodes;
};
template <typename T>
int CFLWeightRoute<T>::add(T node,unsigned weight)
{
    m_vecNodes.push_back(node);
    m_vecWeightRoute.insert(m_vecWeightRoute.end(),weight,m_vecNodes.size()-1);
    return 0;
}
template <typename T>
int CFLWeightRoute<T>::alloc(T& node)
{
    if (m_vecWeightRoute.size()<=0)
    {
        return -1;
    }
    unsigned seed = time(NULL) + getpid();
    srand(seed);

    unsigned index = m_vecWeightRoute[rand() % m_vecWeightRoute.size()];
    if (m_vecNodes.size()<=index)
    {
        return -2;
    }
    node = m_vecNodes[index];
    return 0;
}

#endif
