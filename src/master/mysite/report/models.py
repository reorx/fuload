from datetime import datetime

from django.db import models
from comm_func import TimeToString

class StatMain(models.Model):
    reportId = models.IntegerField()

    beginTime = models.DateTimeField()
    endTime = models.DateTimeField()

    allReqNum = models.IntegerField()
    sucReqNum = models.IntegerField()
    errReqNum = models.IntegerField()

    sucAvgTime = models.IntegerField()
    errAvgTime = models.IntegerField()

    sucRate = models.FloatField()
    errRate = models.FloatField()

    reportInfo = models.TextField()

    class Meta:
        unique_together = (("reportId",))

    #def save(self, *args, **kwargs):
        #if self.beginTime is None:
            #self.beginTime = datetime.now()
        #self.endTime = datetime.now()
        #super(StatMain,self).save(*args,**kwargs)

    def __unicode__(self):
        return "[%s][%s][%u]" % (self.beginTime, self.endTime, self.reportId)

class StatDetail(models.Model):
    reportId = models.IntegerField()

    beginTime = models.DateTimeField()
    endTime = models.DateTimeField()

    allReqNum = models.IntegerField()
    sucReqNum = models.IntegerField()
    errReqNum = models.IntegerField()

    sucAvgTime = models.IntegerField()
    errAvgTime = models.IntegerField()

    sucRate = models.FloatField()
    errRate = models.FloatField()

    reportInfo = models.TextField()

    def __unicode__(self):
        return "[%s][%s][%u]" % (self.beginTime, self.endTime, self.reportId)
