#include<Python.h>//前面所做的一切配置都是为了调用这个头文件和相关库
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
        printf("result is null");
    }

    pFunc= PyObject_GetAttrString(pModule, "fuload_handle_process");//这里是要调用的函数名
    //PyObject_CallFunction(pFunc,"{s:i,s:i}","abc", 123, "def", 456);//调用函数
    PyObject * t_dict = PyDict_New();
    PyDict_SetItemString(t_dict,"first",Py_BuildValue("s", "hello"));
    PyDict_SetItemString(t_dict,"second",Py_BuildValue("s", "world"));
    PyObject_CallFunctionObjArgs(pFunc,t_dict,NULL);

    pFunc= PyObject_GetAttrString(pModule, "fuload_handle_fini");//这里是要调用的函数名
    PyObject_CallFunction(pFunc, NULL);//调用函数

    Py_DECREF(pFunc);
    Py_DECREF(pModule);
    Py_Finalize();//调用Py_Finalize，这个根Py_Initialize相对应的。
    return 0;
}
