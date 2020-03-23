from flask import Flask, render_template
from waitress import serve
from flask_pymongo import PyMongo
from datetime import datetime, time

app = Flask(__name__)

app.config["MONGO_URI"] = "mongodb://iot.jonnes.eng.br:27017/iot_dashboard"
mongo = PyMongo(app)

@app.route("/")
def home_page():
	return "Hello World! The server is up!"

@app.route("/dashboard")
def iot_dashboard():

	TOTAL_REGISTERS = 15
	
	temperatura_list = mongo.db.topic.find({"topic":"temperatura"}).limit(TOTAL_REGISTERS).sort([("_id", -1)])
	umidade_list = mongo.db.topic.find({"topic":"umidade"}).limit(TOTAL_REGISTERS).sort([("_id", -1)])
	
	temp_topics = list(temperatura_list)
	temp_topics.sort()
	
	moist_topics = list(umidade_list)
	moist_topics.sort()

	legend_temp= 'Temperature Sensor #1'
	horarios = [datetime.strptime(dt["datetime"], '%d/%m/%Y %H:%M:%S') for dt in temp_topics]
	temp_times = [time(hour=hr.hour, minute=hr.minute, second=hr.second) for hr in horarios]
	temperatures = [float(pld["payload"]) for pld in temp_topics]

	legend_moist = 'Moisture Sensor #1'
	horarios = [datetime.strptime(dt["datetime"], '%d/%m/%Y %H:%M:%S') for dt in moist_topics]
	moist_times = [time(hour=hr.hour, minute=hr.minute, second=hr.second) for hr in horarios]
	moistures = [float(pld["payload"]) for pld in moist_topics]
	
	return render_template("index.html", titulo="Jonnes' IoT Server", 
   		temp_title=("Last %d Temperature Measurements" % TOTAL_REGISTERS), temp_list=temp_topics, 
   		moist_title=("Last %d Moisture Measurements" % TOTAL_REGISTERS), moist_list=moist_topics,
   		temp_values=temperatures, temp_labels=temp_times, temp_legend=legend_temp, 
   		moist_values=moistures, moist_labels=moist_times, moist_legend=legend_moist)

if __name__ == "__main__":
	serve(app, host='0.0.0.0', port=80)