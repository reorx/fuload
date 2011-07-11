#!/bin/bash
run_dir=`dirname $0`
cd $run_dir

kill -9 `cat django.pid`
