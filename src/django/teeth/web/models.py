# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models

# Create your models here.
class Teeth(models.Model):
	name=models.CharField(max_length=255)
	lpp=models.FloatField(null=True)
	teeth=models.TextField()
	img=models.CharField(max_length=255)

	def __str__(self):
		return "teeth#%s" % self.id
