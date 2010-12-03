import time

from django.db import models
from comm_func import TimeToString

class StatMain(models.Model):
    reportId = models.IntegerField()

    beginTime = models.IntegerField()
    endTime = models.IntegerField()

    allReqNum = models.IntegerField()
    sucReqNum = models.IntegerField()
    errReqNum = models.IntegerField()

    sucRate = models.FloatField()
    errRate = models.FloatField()

    reportInfo = models.TextField()

    class Meta:
        unique_together = (("reportId",))

    def save(self, *args, **kwargs):
        if self.beginTime is None:
            self.beginTime = time.time()
        self.endTime = time.time()
        super(StatMain,self).save(*args,**kwargs)

    def __unicode__(self):
        return "[%s][%s][%u]" % (TimeToString(self.beginTime), TimeToString(self.endTime), self.reportId)

class StatDetail(models.Model):
    reportId = models.IntegerField()

    beginTime = models.IntegerField()
    endTime = models.IntegerField()

    allReqNum = models.IntegerField()
    sucReqNum = models.IntegerField()
    errReqNum = models.IntegerField()

    sucRate = models.FloatField()
    errRate = models.FloatField()

    reportInfo = models.TextField()

    def __unicode__(self):
        return "[%s][%s][%u]" % (TimeToString(self.beginTime), TimeToString(self.endTime), self.reportId)
