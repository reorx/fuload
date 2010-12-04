from django.conf.urls.defaults import *
import mysite.report.views

# Uncomment the next two lines to enable the admin:
from django.contrib import admin
admin.autodiscover()

urlpatterns = patterns('',
    # Example:
    #(r'^mysite/', include('mysite.foo.urls')),

    # Uncomment the admin/doc line below to enable admin documentation:
    # (r'^admin/doc/', include('django.contrib.admindocs.urls')),

    # Uncomment the next line to enable the admin:
    (r'^admin/', include(admin.site.urls)),

    (r'^report/upload/(\d+)/$', mysite.report.views.HandleReportUpload),
    (r'^report/data/avgtime/(\d+)/$', mysite.report.views.HttpReportDataAvgTime),


    (r'^report/show/avgtime/(\d+)/$', mysite.report.views.HttpReportShowAvgTime),
)
