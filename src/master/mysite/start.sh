#!/bin/bash
run_dir=`dirname $0`
cd $run_dir

export PYTHON_EGG_CACHE=/tmp/.python-eggs/

uwsgi -s :9090 -w uws_app -p10 -d log/uws.error --pidfile uws.pid --touch-reload=uws.tc -L &

#uwsgi --module='django.core.handlers.wsgi:WSGIHandler()' --env DJANGO_SETTINGS_MODULE=mysite.settings -s :9090 -p 10 -d uws.log --touch-reload=uws.tc &
