from datetime import datetime

from django.db import models

class StatMain(models.Model):
    reportId = models.IntegerField()

    firstTime = models.DateTimeField()
    secondTime = models.DateTimeField()

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
        #if self.firstTime is None:
            #self.firstTime = datetime.now()
        #self.secondTime = datetime.now()
        #super(StatMain,self).save(*args,**kwargs)

    def __unicode__(self):
        return "[%s][%s][%u]" % (self.firstTime, self.secondTime, self.reportId)

class StatDetail(models.Model):
    reportId = models.IntegerField()

    firstTime = models.DateTimeField()
    secondTime = models.DateTimeField()

    allReqNum = models.IntegerField()
    sucReqNum = models.IntegerField()
    errReqNum = models.IntegerField()

    sucAvgTime = models.IntegerField()
    errAvgTime = models.IntegerField()

    sucRate = models.FloatField()
    errRate = models.FloatField()

    reportInfo = models.TextField()

    def __unicode__(self):
        return "[%s][%s][%u]" % (self.firstTime, self.secondTime, self.reportId)
