#include<Python.h>//前面所做的一切配置都是为了调用这个头文件和相关库
int main()
{
    Py_Initialize();//使用python之前，要调用Py_Initialize();这个函数进行初始化
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('./')");
    PyObject * pModule = NULL;//声明变量
    PyObject * pFunc = NULL;// 声明变量
    pModule =PyImport_ImportModule("testpy");//这里是要调用的文件名
    if (!pModule) {
        printf("Cant open python file!\n");
        return -1;
    }
    pFunc= PyObject_GetAttrString(pModule, "hello");//这里是要调用的函数名
    PyEval_CallObject(pFunc, NULL);//调用函数
    Py_Finalize();//调用Py_Finalize，这个根Py_Initialize相对应的。
    return 0;
}
