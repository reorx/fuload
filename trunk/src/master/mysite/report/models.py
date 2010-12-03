from datetime import datetime

from django.db import models

class StatDetail(models.Model):
    reportId = models.IntegerField()

    firstTime = models.DateTimeField()
    secondTime = models.DateTimeField()

    allAvgNum = models.IntegerField()
    sucAvgNum = models.IntegerField()
    errAvgNum = models.IntegerField()

    allAvgTime = models.IntegerField()
    sucAvgTime = models.IntegerField()
    errAvgTime = models.IntegerField()

    sucRate = models.FloatField()
    errRate = models.FloatField()

    reportInfo = models.TextField()

    class Meta:
        unique_together = (("reportId","firstTime","secondTime"))

    def __unicode__(self):
        return "[%s][%s][%u]" % (self.firstTime, self.secondTime, self.reportId)
