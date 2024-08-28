from django.db import models

# Create your models here.
class Message(models.Model):
    sender =  models.CharField(max_length=255) #hash of the receiver

    body = models.TextField() # the message itself

    timestamp = models.DateTimeField(auto_now_add=True) # auto adds based on the time it was sent

    def __str__(self):
        return f"Message from {self.sender} - {self.body}"