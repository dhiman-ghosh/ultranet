import inflect
from flask_ask import statement, question

class Alexa:
  def __init__(self, context):
    self.id = context.System.user.userId.split('.')[3]

  def welcome(self):
    msg = """Welcome to Ultranet IoT Services.
          You can ask for any real time update about
          any service installed in Ultranet."""
    return statement(msg)

  def default(self):
    qs = "Sorry, did not get what is your inventory query. Do you want me to search some product in stock?"
    return question(qs)

  def default_yes(self):
    qs = "Simply ask, stock for your product. For example, stock for biscuits."
    return question(qs)

  def default_no(self):
    return statement('Okay')