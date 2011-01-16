#!/usr/bin/python
# -*- coding: utf-8 -*-

'''
#=============================================================================
#  Author:          dantezhu - http://www.vimer.cn
#  Email:           zny2008@gmail.com
#  FileName:        report_show_handler.py
#  Description:     数据展示的handler
#  Version:         1.0
#  LastChange:      2011-01-16 20:57:57
#  History:         
#=============================================================================
'''
import datetime

from comm_def import rtype2attr,split_minutes,rtype2attr,max_x_len,pie_colors,default_grid_width,default_persent_yx

class ReportShowBaseHandler(object):
    #数据xml文件
    _data_file = ''
    #swf文件路径
    _swf_file = ''
    #传入的clean_data
    _cd = None
    #生成的数据
    _data = None

    def __init__(self, cd):
        self._cd = cd

    #仅仅是把数据查找出来没有做进一步的处理
    def _get_report_objs(self, cd):
        if cd is None:
            return None
        from models import StatDetail
        objs = StatDetail.objects.filter(reportId=cd['reportid'])

        if 'clientip' in cd and cd['clientip'] is not None:
            objs = objs.filter(clientIp=cd['clientip'])

        if 'begintime' in cd and cd['begintime'] is not None:
            objs = objs.filter(firstTime__gte=cd['begintime'])

        if 'endtime' in cd and cd['endtime'] is not None:
            objs = objs.filter(firstTime__lt=cd['endtime'])

        objs.order_by('firstTime')

        return objs

    def get_data(self):
        '''
        子类不必实现
        '''
        self.make_data()
        return self._data

    def get_data_file(self):
        '''
        子类不必实现
        '''
        return self._data_file

    def get_swf_file(self):
        '''
        子类不必实现
        '''
        return self._swf_file


    # 从这里之后的方法，子类都是可以实现的
    def make_data():
        '''
        模版方法，子类必须实现这个方法来生成data
        '''
        return True

    def get_render_data(self):
        '''
        获取用来render的render_data
        '''
        render_data = {}
        render_data['data'] = self.get_data()
        return render_data


    def get_swf_width(self):
        '''
        子类可以自己实现返回的flash长度
        '''
        return default_grid_width

    def is_valid(self):
        return True

class ReportShowLineHandler(ReportShowBaseHandler):
    def __init__(self, cd):
        super(ReportShowLineHandler, self).__init__(cd)
        self._data_file = 'show/line_data.xml'
        self._swf_file = 'fcp-line-chart.swf'

    def make_data(self):
        if self._data is not None:
            return True
        from models import StatDetail

        objs = self._get_report_objs(self._cd)

        if objs is None or len(objs) == 0:
            self._data = []
            return True

        rtype = self._cd['rtype']

        begintime = objs[0].firstTime
        endtime = objs[len(objs)-1].firstTime

        data = []
        t = datetime.timedelta(minutes=split_minutes)
        d = begintime
        while d <= endtime:
            t_item= {}
            t_item['x']=d
            try:
                 obj = objs.get(firstTime=d)
            except StatDetail.DoesNotExist:
                t_item['y'] = None
            else:
                t_item['y'] = getattr(obj,rtype2attr[rtype]['attr'])
            data.append(t_item)
            d = d+t

        if self._cd['adjust'] == 0:
            data = self._compress_data_line(data,max_x_len)

        for item in data:
            if item['x'] is not None:
                item['x'] = item['x'].strftime('%Y-%m-%d %H:%M')
            if item['y'] is not None:
                item['y'] = rtype2attr[rtype]['accuracy'] % item['y']

        self._data = data
        return True

    def get_render_data(self):
        render_data = {}
        render_data['data'] = self.get_data()
        render_data['grid_width'] = self._cal_grid_width(len(render_data['data']))
        render_data['persent_yx'] = self._cal_persent_yx(len(render_data['data']))
        return render_data

    def get_swf_width(self):
        return self._cal_grid_width(len(self.get_data()))+110

    def _compress_data_line(self, data, max_len):
        '''
        压缩线性的数据
        '''
        while len(data) > max_len:
            t_data = []
            for i,item in enumerate(data):
                if i%2 == 0:
                    if i+1 < len(data):
                        first_y = item['y']
                        second_y = data[i+1]['y']
                        if first_y is not None and second_y is not None:
                            t_y = (float(first_y)+float(second_y)) / 2
                        else:
                            t_y = first_y if first_y is not None else second_y

                        t_item = {
                                'x':item['x'],
                                'y':t_y
                                }
                    else:
                        t_item = item
                    t_data.append(t_item)
            data = t_data
        return data

    def _cal_grid_width(self, len_data=0):
        if len_data <= max_x_len:
            return default_grid_width
        return default_grid_width * len_data / max_x_len

    def _cal_persent_yx(self, len_data=0):
        if len_data <= max_x_len:
            return default_persent_yx
        return default_persent_yx * len_data / max_x_len

    def is_valid(self):
        #这样就不用自己去做None判断了
        t_data = self.get_data()
        if len(t_data) < 2:
            return False
        else:
            return True

class ReportShowPieHandler(ReportShowBaseHandler):
    def __init__(self, cd):
        super(ReportShowPieHandler, self).__init__(cd)
        self._data_file = 'show/pie_data.xml'
        self._swf_file = 'fcp-pie-2d-charts.swf'

    def make_data(self):
        '''
        获取饼状的数据
        '''
        if self._data is not None:
            return True

        objs = self._get_report_objs(self._cd)

        if objs is None or len(objs) == 0:
            self._data = []
            return True
        rtype = self._cd['rtype']

        data_map = {}
        for obj in objs:
            report_info = eval(obj.reportInfo)
            t_map = report_info[rtype]
            if t_map is None:
                continue
            for k,v in t_map.items():
                if k in data_map:
                    data_map[k] += v
                else:
                    data_map[k] = v

        orig_data = []
        for k,v in data_map.items():
            t_item = {
                    'name':k,
                    'value':v
                    }
            orig_data.append(t_item)

        orig_data.sort(lambda x,y: cmp(y['value'], x['value']))   

        max_pie_typecount = len(pie_colors) - 1

        #截断出最大类别
        cut_data = orig_data[:max_pie_typecount]

        if len(orig_data) > max_pie_typecount:
            sum_d = 0
            for i in range(max_pie_typecount,len(orig_data)):
                sum_d += orig_data[i]['value']
            cut_data.append({'name':'else','value':sum_d})

        sum_value = 0

        for v in cut_data:
            sum_value+=v['value']

        #加上颜色
        accuracy = rtype2attr[rtype]['accuracy']
        for i,item in enumerate(cut_data):
            item['color'] = pie_colors[i]
            if float(sum_value) != 0:
                item['value'] = accuracy % (float(item['value']) * float(100) / float(sum_value))
            else:
                item['value'] = 0

        self._data = cut_data
        return True

    def is_valid(self):
        t_data = self.get_data()
        if len(t_data) < 1:
            return False
        else:
            return True

class ReportShowMultiLineHandler(ReportShowBaseHandler):
    def __init__(self, cd):
        super(ReportShowMultiLineHandler, self).__init__(cd)

def get_show_handler(cd):
    '''
    工厂函数，负责生成对应类型的对象
    '''
    rtype = cd['rtype']

    obj = eval(rtype2attr[rtype]['showhandler'])(cd)
    return obj
