import inflect
import cloudmqtt
from flask_ask import statement, question

class Alexa:
  def __init__(self, context):
    self.id = context.System.user.userId.split('.')[3]

  def welcome(self):
    msg = """Welcome to Ultranet IoT Services.
          You can ask for any real time update about
          any service installed in Ultranet. For example,
          you can ask about the air quality."""
    return statement(msg)
    
  def get_arq(self):
    arq = cloudmqtt.get_arq()
    msg = "The current air quality in your surrounding is " + arq + " PPM."
    return statement(msg)

  def set_ledstrip_pattern(self, pattern):
    led = cloudmqtt.set_ledstrip_pattern(5, pattern)
    return statement("I have set the requested mood to " + pattern)

  def default(self):
    qs = "Sorry, did not get your query. Do you want me to tell you about the air quality?"
    return question(qs)

  def default_yes(self):
    qs = "Simply ask, about air quality."
    return question(qs)

  def default_no(self):
    return statement('Okay')