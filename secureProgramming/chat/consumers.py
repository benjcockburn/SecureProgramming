from channels.generic.websocket import AsyncWebsocketConsumer
import json

class YourConsumer(AsyncWebsocketConsumer):
    async def connect(self):
        await self.accept()

    async def disconnect(self, close_code):
        pass

    async def receive(self, text_data):
        # Handle incoming WebSocket data here
        await self.send(text_data=json.dumps({
            'message': text_data
        }))
