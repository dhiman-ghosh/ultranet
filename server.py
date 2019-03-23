import os
import json
import logging
import cloudmqtt

from flask import Flask, render_template
from flask_ask import Ask, context

import alexa

API = '/api/v1'
PKG_DIR = os.path.dirname(os.path.realpath(__file__))

app = Flask(__name__, template_folder=PKG_DIR)
app.secret_key = 'wanderingcouple1194'
app.config['SESSION_TYPE'] = 'filesystem'
ask = Ask(app, API + '/alexa')

@app.before_first_request
def setup_logging():
  gunicorn_logger = logging.getLogger('gunicorn.error')
  app.logger.handlers = gunicorn_logger.handlers
  app.logger.setLevel(gunicorn_logger.level)

@app.route('/')
def index():
  arq_val = cloudmqtt.get_arq(5)
  return render_template('default.html', arq=arq_val,
                          title="Air Quality Index")


# --------------- API ------------------
@app.route(API + '/auth/<pin>', methods=['GET'])
def authorize(pin):
  #user = dbprofile.Profile()
  if True:#user.authorize(pin):
    return Response('{"status": "OK"}')
  else:
    return Response('{"status": "Unauthorized"}', status=401)


# --------------- Alexa ------------------

@ask.launch
def ask_welcome():
  ask_handler = alexa.Alexa(context)
  return ask_handler.welcome()

@ask.intent('FallbackIntent')
def ask_fallback():
  return ask_welcome()

@ask.intent('AirQuality')
def ask_access_key():
  ask_handler = alexa.Alexa(context)
  return ask_handler.get_arq() 

@ask.intent('AMAZON.FallbackIntent')
def default():
  ask_handler = alexa.Alexa(context)
  return ask_handler.default()

@ask.intent('AMAZON.YesIntent')
def default():
  ask_handler = alexa.Alexa(context)
  return ask_handler.default_yes()

@ask.intent('AMAZON.NoIntent')
def default():
  ask_handler = alexa.Alexa(context)
  return ask_handler.default_no()

if __name__ == "__main__":
  app.run(host='0.0.0.0', debug=True)
