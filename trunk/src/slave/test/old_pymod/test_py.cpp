#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include<Python.h>//前面所做的一切配置都是为了调用这个头文件和相关库
using namespace std;
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
int main()
{
    Py_Initialize();//使用python之前，要调用Py_Initialize();这个函数进行初始化
    if (!Py_IsInitialized())
    {
        printf("init error\n");
        return -1;
    }
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('./')");

    PyObject * pModule = NULL;//声明变量
    PyObject * pFunc = NULL;// 声明变量

    pModule =PyImport_ImportModule("fl_module");//这里是要调用的文件名
    if (!pModule) {
        printf("Cant open python file!\n");
        return -1;
    }
    pFunc= PyObject_GetAttrString(pModule, "fuload_handle_init");//这里是要调用的函数名
    PyObject *objResult =  PyObject_CallFunction(pFunc, NULL);//调用函数
    int ret = -1;
    if (objResult)
    {
        ret = PyInt_AsLong(objResult);
        printf("ret:%d\n",ret);
    }
    else
    {
        printf("1.result is null\n");
    }
    printf("process\n");

    pFunc= PyObject_GetAttrString(pModule, "fuload_handle_process");//这里是要调用的函数名
    //PyObject_CallFunction(pFunc,"{s:i,s:i}","abc", 123, "def", 456);//调用函数
    PyObject * t_dict = PyDict_New();
    PyDict_SetItemString(t_dict,"first",Py_BuildValue("s", "hello"));
    PyDict_SetItemString(t_dict,"second",Py_BuildValue("s", "world"));
    objResult = PyObject_CallFunctionObjArgs(pFunc,t_dict,NULL);
    if (!objResult)
    {
        printf("2.result is null");
        printf("%s\n",log_python_exception().c_str());
    }
    else
    {
        ret = PyInt_AsLong(objResult);
        printf("ret:%d\n",ret);
    }

    pFunc= PyObject_GetAttrString(pModule, "fuload_handle_fini");//这里是要调用的函数名
    PyObject_CallFunction(pFunc, NULL);//调用函数

    Py_DECREF(pFunc);
    Py_DECREF(pModule);
    Py_Finalize();//调用Py_Finalize，这个根Py_Initialize相对应的。
    return 0;
}
