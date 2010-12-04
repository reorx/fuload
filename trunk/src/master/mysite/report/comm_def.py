#!/usr/bin/python
# -*- coding: utf-8 -*-

#五分钟
split_minutes = 5
report_keys = (
'allTimeMsStat',
'sucTimeMsStat',
'errTimeMsStat',
'allReqNum',
'sucReqNum',
'errReqNum',
'alltimemap',
'suctimemap',
'errtimemap',
'retmap'
)
loop_keys = (
'alltimemap',
'suctimemap',
'errtimemap',
'retmap'
)
rtype2attr_line = {
        'allavgnum':'allAvgNum',
        'sucavgnum':'sucAvgNum',
        'erravgnum':'errAvgNum',

        'allavgtime':'allAvgTime',
        'sucavgtime':'sucAvgTime',
        'erravgtime':'errAvgTime',
        
        'sucrate':'sucRate',
        'errrate':'errRate',
        }
