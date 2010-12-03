from django.contrib import admin
from mysite.report.models import *

class StatDetailAdmin(admin.ModelAdmin):
    list_display = (
            'reportId','firstTime', 'secondTime',
            'allAvgNum','sucAvgNum','errAvgNum',
            'allAvgTime','sucAvgTime','errAvgTime',
            'sucRate','errRate'
            )
    list_filter = ('reportId',)
    ordering = ('reportId',)

admin.site.register(StatDetail,StatDetailAdmin)
