#!/usr/bin/python
# -*- coding: utf-8 -*-

#五分钟
split_minutes = 5
#X轴最大的长度
max_x_len = 96
#flash默认宽度
default_grid_width = 900
#flash y,x默认比例
default_persent_yx = 175
#上报数据时json的字段
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
#上报数据时，对这些字段要进行遍历累加
loop_keys = (
'alltimemap',
'suctimemap',
'errtimemap',
'retmap'
)
#form中原始展现的rtype顺序
rtype_original_keys = (
        'allavgnum',
        'sucavgnum',
        'erravgnum',

        'allavgtime',
        'sucavgtime',
        'erravgtime',

        'sucrate',
        'errrate',

        'alltimemap',
        'suctimemap',
        'errtimemap',
        'retmap',
        )
#展示数据时，前台传来的rtype与attr，name等的对应
rtype2attr = {
        'allavgnum':{
            'attr':'allAvgNum',
            'swftype':'line',
            'name':'总每秒调用量',
            'accuracy':'%.2f',
            },
        'sucavgnum':{
            'attr':'sucAvgNum',
            'swftype':'line',
            'name':'成功每秒调用量',
            'accuracy':'%.2f',
            },
        'erravgnum':{
            'attr':'errAvgNum',
            'swftype':'line',
            'name':'失败每秒调用量',
            'accuracy':'%.2f',
            },

        'allavgtime':{
            'attr':'allAvgTime',
            'swftype':'line',
            'name':'总平均响应时间(ms)',
            'accuracy':'%.2f',
            },
        'sucavgtime':{
            'attr':'sucAvgTime',
            'swftype':'line',
            'name':'成功平均响应时间(ms)',
            'accuracy':'%.2f',
            },
        'erravgtime':{
            'attr':'errAvgTime',
            'swftype':'line',
            'name':'失败平均响应时间(ms)',
            'accuracy':'%.2f',
            },
        
        'sucrate':{
            'attr':'sucRate',
            'swftype':'line',
            'name':'成功率(%)',
            'accuracy':'%.2f',
            },
        'errrate':{
            'attr':'errRate',
            'swftype':'line',
            'name':'失败率(%)',
            'accuracy':'%.2f',
            },

        'alltimemap':{
            'attr':'alltimemap',
            'swftype':'pie',
            'name':'总响应时间饼图(ms)',
            'accuracy':'%.2f',
            },
        'suctimemap':{
            'attr':'suctimemap',
            'swftype':'pie',
            'name':'成功响应时间饼图(ms)',
            'accuracy':'%.2f',
            },
        'errtimemap':{
            'attr':'errtimemap',
            'swftype':'pie',
            'name':'失败响应时间饼图(ms)',
            'accuracy':'%.2f',
            },

        'retmap':{
            'attr':'retmap',
            'swftype':'pie',
            'name':'返回码饼图',
            'accuracy':'%.2f',
            },
        }
