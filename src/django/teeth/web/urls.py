from django.conf.urls import url,include
from . import views

urlpatterns = [
	url(r'^$', views.index,name='index'),
	url(r'^load', views.load,name='load'),
	url(r'^delete/(\d+)/', views.delete,name='delete'),
]

