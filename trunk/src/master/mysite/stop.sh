#!/bin/bash
run_dir=`dirname $0`
cd $run_dir

#如果在rc.local加入之后不执行，那么就要考虑把这句打开了。。
#export PYTHON_EGG_CACHE=/tmp/.python-eggs/

#python manage.py runfcgi daemonize=true pidfile=`pwd`/django.pid host=127.0.0.1 port=9001 maxrequests=1 &
python manage.py runfcgi daemonize=true pidfile=`pwd`/django.pid socket=/home/dantezhu/nginx/sbin/django.sock maxrequests=1 &

#如果在windows下，则只能用端口+线程模式
#python manage.py runfcgi daemonize=false pidfile=./django.pid host=127.0.0.1 port=9001 method=threaded
