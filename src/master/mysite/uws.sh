#!/bin/bash
#=============================================================================
#
#     FileName: uws.sh
#         Desc: for uwsgi start/stop/reload
#
#       Author: dantezhu
#        Email: zny2008@gmail.com
#     HomePage: http://www.vimer.cn
#
#      Created: 2011-07-24 21:15:23
#      Version: 0.0.1
#      History:
#               0.0.1 | dantezhu | 2011-07-24 21:15:23 | initialization
#
#=============================================================================

run_dir=`dirname $0`
cd $run_dir

#init val
host=
port=9090
script_name=uws_app
app_name=app
worker_num=2

touch_file=uws/uws.tc
pid_file=uws/uws.pid
log_file=log/uws.log

do_start() {
    #init dir or file
    mkdir -p uws
    mkdir -p log

    touch $touch_file
    export PYTHON_EGG_CACHE=/tmp/.python-eggs/

    #exec
    uwsgi -s $host:$port -w $script_name:$app_name -p $worker_num -d $log_file --pidfile $pid_file --touch-reload $touch_file -L &
}

do_stop() {
    kill -INT `cat $pid_file`
}

do_reload() {
    touch $touch_file
}

case "$1" in
    start)
        do_start
        ;;
    stop)
        do_stop
        ;;
    reload)
        do_reload
        ;;
    *)
        echo "Usage: $0 start|stop|reload" >&2
        exit 3
        ;;
esac
