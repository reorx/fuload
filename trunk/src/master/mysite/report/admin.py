from django.contrib import admin
from mysite.report.models import *

class StatMainAdmin(admin.ModelAdmin):
    list_display = ('reportId','firstTime','secondTime',)
    list_filter = ('reportId',)
    ordering = ('reportId',)

class StatDetailAdmin(admin.ModelAdmin):
    list_display = ('reportId','firstTime','secondTime',)
    list_filter = ('reportId',)
    ordering = ('reportId',)

admin.site.register(StatMain,StatMainAdmin)
admin.site.register(StatDetail,StatDetailAdmin)
