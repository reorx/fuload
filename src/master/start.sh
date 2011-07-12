#!/bin/bash

uwsgi -s :9090 -w uws_app -p10 -d uws.error &
