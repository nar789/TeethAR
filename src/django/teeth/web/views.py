# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.shortcuts import render,get_object_or_404
from django.http import HttpResponse
from .models import Teeth
import os

# Create your views here.

def delete(request,tid):
	t=Teeth.objects.get(id=tid)
	t.delete()
	return render(request,'web/save_complete.html',{'msg':'삭제가 완료됐습니다.','title':'삭제완료'})

def load(request):
	t=Teeth.objects.all()
	return render(request,"web/load.html",{'t':t})

def index(request):
	if request.method == "POST":
		tid=request.POST['id']
		tid=int(tid)
		name=request.POST['name']
		lpp=float(request.POST['lpp'])
		teeth=request.POST['teeth']
		img=request.POST['img']
		if tid == 0:
			t=Teeth(name=name,lpp=lpp,teeth=teeth,img=img)
			t.save()
		else :
			t=Teeth.objects.get(id=tid)
			t.name=name
			t.lpp=lpp
			t.teeth=teeth
			t.img=img
			t.save()

		return render(request,'web/save_complete.html',{'msg':'저장이 완료됐습니다.','title':'저장완료'})

	if 'id' in request.GET:
		tid=request.GET['id']
		t=get_object_or_404(Teeth,id=tid)
		return render(request,'web/index.html',{'t':t})

	PROJECT_ROOT = os.path.abspath(os.path.dirname(__file__))
	f=open(os.path.join(PROJECT_ROOT, 'data.dat'),'r')
	r=f.readlines()
	r=r[0][:-1]
	img='http://hume.co.kr/teethimg/'+r

	return render(request,'web/index.html',{'img':img})
