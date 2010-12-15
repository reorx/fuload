/*=============================================================================
#  Author:          dantezhu - http://www.vimer.cn
#  Email:           zny2008@gmail.com
#  FileName:        main.cpp
#  Description:     专门为python so设计
#  Version:         1.0
#  LastChange:      2010-12-13 19:16:59
#  History:         
=============================================================================*/
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <python2.7/Python.h>
using namespace std;

#define PYMODULE_NAME   "fl_module"
#define PYFUNC_INIT     "fuload_handle_init"
#define PYFUNC_PROCESS  "fuload_handle_process"
#define PYFUNC_FINI     "fuload_handle_fini"

#define PYMODULE_PATH   "../py_module/"

PyObject * g_pModule = NULL;
PyObject * g_pInitFunc = NULL;
PyObject * g_pProcessFunc = NULL;
PyObject * g_pFiniFunc = NULL;

string log_python_exception()
{
    std::string strErrorMsg;
    if (!Py_IsInitialized())
    {
        strErrorMsg = "Python 运行环境没有初始化！";
        return strErrorMsg;
    }

    if (PyErr_Occurred() != NULL)
    {
        PyObject *type_obj, *value_obj, *traceback_obj;
        PyErr_Fetch(&type_obj, &value_obj, &traceback_obj);
        if (value_obj == NULL)
            return strErrorMsg;

        PyErr_NormalizeException(&type_obj, &value_obj, 0);
        if (PyString_Check(PyObject_Str(value_obj)))
        {
            strErrorMsg = PyString_AsString(PyObject_Str(value_obj));
        }

        if (traceback_obj != NULL)
        {
            strErrorMsg += "Traceback:";

            PyObject * pModuleName = PyString_FromString("traceback");
            PyObject * pTraceModule = PyImport_Import(pModuleName);
            Py_XDECREF(pModuleName);
            if (pTraceModule != NULL)
            {
                PyObject * pModuleDict = PyModule_GetDict(pTraceModule);
                if (pModuleDict != NULL)
                {
                    PyObject * pFunc = PyDict_GetItemString(pModuleDict,"format_exception");
                    if (pFunc != NULL)
                    {
                        PyObject * errList = PyObject_CallFunctionObjArgs(pFunc,type_obj,value_obj, traceback_obj,NULL);
                        if (errList != NULL)
                        {
                            int listSize = PyList_Size(errList);
                            for (int i=0;i < listSize;++i)
                            {
                                strErrorMsg += PyString_AsString(PyList_GetItem(errList,i));
                            }
                        }
                    }
                }
                Py_XDECREF(pTraceModule);
            }
        }
        Py_XDECREF(type_obj);
        Py_XDECREF(value_obj);
        Py_XDECREF(traceback_obj);
    }
    return strErrorMsg;
}

/**
 * @brief   清理python环境
 */
void clear_pyenv()
{
    Py_CLEAR(g_pModule);
    Py_CLEAR(g_pInitFunc);
    Py_CLEAR(g_pProcessFunc);
    Py_CLEAR(g_pFiniFunc);
    Py_Finalize();//调用Py_Finalize，这个根Py_Initialize相对应的。

    g_pModule = NULL;
    g_pInitFunc = NULL;
    g_pProcessFunc = NULL;
    g_pFiniFunc = NULL;
}


/**
 * @brief   第一次进入so时，需要做的初始化工作，只会执行一次。
 *
 * @return  0               succ
 *          else            fail
 */
extern "C" int fuload_handle_init()
{
    Py_Initialize();//使用python之前，要调用Py_Initialize();这个函数进行初始化
    if (!Py_IsInitialized())
    {
        printf("init error\n");
        return -1001;
    }
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('./')");
    PyRun_SimpleString("sys.path.append('"PYMODULE_PATH"')");

    g_pModule =PyImport_ImportModule(PYMODULE_NAME);//这里是要调用的文件名
    if (!g_pModule) {
        printf("Cant open python file!\n");
        printf("%s\n",log_python_exception().c_str());
        clear_pyenv();
        return -1002;
    }
    g_pInitFunc = PyObject_GetAttrString(g_pModule, PYFUNC_INIT);//这里是要调用的函数名
    g_pProcessFunc = PyObject_GetAttrString(g_pModule, PYFUNC_PROCESS);//这里是要调用的函数名
    g_pFiniFunc = PyObject_GetAttrString(g_pModule, PYFUNC_FINI);//这里是要调用的函数名
    if (!g_pInitFunc || !g_pProcessFunc || !g_pFiniFunc)
    {
        printf("func name not find\n");
        clear_pyenv();
        return -1003;
    }
    PyObject *objResult =  PyObject_CallFunction(g_pInitFunc, NULL);//调用函数
    if (!objResult)
    {
        clear_pyenv();
        return -1004;
    }
    int ret = PyInt_AsLong(objResult);

    return ret;
}

/**
 * @brief   业务逻辑，每次进入
 *
 * @param   mapParams       将输入数据按照url方式解析之后的key-value结构
 *
 * @return  0               succ
 *          else            返回值,会用来做统计
 */
extern "C" int fuload_handle_process(map<string,string>& mapParams)
{
    if (!g_pProcessFunc)
    {
        return -1001;
    }
    PyObject * t_dict = PyDict_New();
    for(map<string, string>::iterator it = mapParams.begin(); it != mapParams.end(); ++it)
    {
        PyDict_SetItemString(t_dict,it->first.c_str(),Py_BuildValue("s", it->second.c_str()));
    }
    PyObject *objResult = PyObject_CallFunctionObjArgs(g_pProcessFunc,t_dict,NULL);
    if (!objResult)
    {
        printf("%s\n",log_python_exception().c_str());
        return -1002;
    }
    int ret = PyInt_AsLong(objResult);

    return ret;
}

/**
 * @brief   so结束时的收尾工作，只会调用一次。一般不用编写
 *
 * @return  0               succ
 *          else            fail
 */
extern "C" int fuload_handle_fini()
{
    PyObject *objResult =  PyObject_CallFunction(g_pFiniFunc, NULL);//调用函数
    if (!objResult)
    {
        clear_pyenv();
        return -1004;
    }
    int ret = PyInt_AsLong(objResult);

    clear_pyenv();
    return ret;
}
#ifdef FL_MODULE_MAIN
int main(int argc, const char *argv[])
{
    map<string,string> mapParams;
    mapParams["first"]="1";
    mapParams["second"]="2";
    fuload_handle_init();
    fuload_handle_process(mapParams);
    fuload_handle_fini();
    return 0;
}
#endif
