

# Create your views here.
from django.http import HttpResponse

from django.shortcuts import render, redirect
from django.template import loader


def chatPage(request, *args, **kwargs):
    if not request.user.is_authenticated:
        return redirect("login-user")
    context = {}
    
    return render(request, 'chatPage.html', context)