# fuload
Automatically exported from code.google.com/p/fuload

一直以来，我都在考虑一个问题，怎么能保证在一个单机访问量上万的服务在上线之后一定是稳定的呢？测试，我们有单元测试，功能测试，但这是否够了呢？不够！一定尽量模拟正式环境的测试，否则一切都是没有办法保证的。 所以我写了fuload这个压力测试框架，并且把它开源，原因有几个：

1. 让所有人的做压力测试变得简单
2. 让尽量多的人，参与到开源项目里来

这个项目目前虽然已经能够正常的提供服务，但还需要尽量多的优化，所以很希望有朋友能够参加进来。另外，本博以后也会负责一些开源项目的开发和维护。一群素未蒙面的人一起完成一件有意义的事情，酷！

项目网址如下：http://code.google.com/p/fuload/

详细的说明文档如下：

一.这个框架能做什么？ 简单来说，fuload是为了给大型服务做压力测试或性能测试诞生的，你可以通过fuload来对你待上线或者已经上线的服务进行压力测试，并能通过详细的报表得出对你的服务的客观评价。

二.架构说明 整个框架实际包括两个部分:master和slave，master上运行用来统计的网站，slave上会调用用户自写的so并实现向master的上报（如图所示）。

1

上报的结果可以直接在master端查看,链接如下：http://{youhost}/report/show/（如图所示）:

1

1

三.使用说明

1. master部署
   依赖环境:linux + python + django
   a)部署master代码到选择的目录
   b)按照项目中httpd.conf修改apache配置
   c)通过python manage.py生成数据库
   d)访问http://{youhost}/report/show/

2. slave部署
   依赖环境:linux + python
   a)到worker目录下执行make all
   b)到module目录下执行make all
   c)到ctrl目录执行./start.sh
   d)在ctrl目录执行./fl_toolstat -f fl_statfile -l，可以看到当前调用量的统计

3. 用户自定义so的编写
   用户只需要实现3个接口即可：

```
/

@brief 第一次进入so时，需要做的初始化工作，只会执行一次。
@return 0 succ
else fail
extern "C" int fuload_handle_init()
/

@brief 业务逻辑，每次进入
@param mapParams 将输入数据按照空格拆分之后的vector
@return 0 succ
else 返回值,会用来做统计
extern "C" int fuload_handle_process(map<string,string>& mapParams)
/

@brief so结束时的收尾工作，只会调用一次。一般不用编写
@return 0 succ
else fail
extern "C" int fuload_handle_fini()
```

实际上重点只需要实现fuload_handle_process函数即可,mapParams中存储的是按照输入文件定义的数据。具体输入文件的配置请查看下一节《slave配置文件》。

4. slave配置文件
   slave的所有配置都在ctrl目录中的fl_slave_conf.py中，如下所示：
   
```
#启动的worker进程数目 WORKER_NUM = 2
#每个worker每秒速度限制(-1为不限制) LIMIT_SPEED = -1

#worker进程的输入文件 INPUT_FILE = "./fl_input.txt"

#worker进程需要加载的so SO_FILE = "../module/libmodule.so"

#worker进程的上报间隔(sec) REPORT_TIMESEC = 10

#上报的URL REPORT_URL = 'http://dante.qq.com/report/upload/1/'
```

上面的注释已经说的很清楚了，需要特别说明一下:

REPORT_URL = 'http://dante.qq.com/report/upload/1/' 中的1是reportid，无需申请，用户只要保证不和别人使用的reportid重复即可，在master上需要通过reportid作为统计查询的参数

INPUT_FILE = "./fl_input.txt" 是用户自定的输入文件，格式如下:

```
#这是注释

test[1 ]

name=dantezhu&string=woaininiuniu&laopo = niuniu

#这个也是注释

test2[2 ]

name=niuniu&string=woainidandan&laogong = dandan
```

其中test[1 ]中的1是权重的作用，即占整个请求的1/3。

`name=dantezhu&string=woaininiuniu&laopo = niuniu`

是具体的内容，框架会按照url的方式来解析，而用户自己的so只需要按照mapParams来取的数据即可。

OK，基本就是这样，目前的介绍还比较简单，等以后再补充~
