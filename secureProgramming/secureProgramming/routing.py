from django.urls import path
from chat.consumers import YourConsumer

websocket_urlpatterns = [
    path('', YourConsumer.as_asgi()),
]
