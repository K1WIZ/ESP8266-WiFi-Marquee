import urllib, json
# DEFINE HOST TO QUERY
host = "10.0.1.10"
# GATHER THD DATA (TEMP,HUMIDITY,DEWPOINT)
url = "http://" + host + ":8080/json.htm?type=devices&rid=6"
response = urllib.urlopen(url)
data = json.loads(response.read())
thd = data["result"]
# GATHER WIND DATA (WIND,SPEED,DIRECTION)
url2 = "http://" + host + ":8080/json.htm?type=devices&rid=7"
response2 = urllib.urlopen(url2)
data2 = json.loads(response2.read())
windData = data2["result"]
# GET BAROMETER PREDICTION
url3 = "http://" + host + ":8080/json.htm?type=devices&rid=6"
response3 = urllib.urlopen(url3)
data3 = json.loads(response3.read())
baro = data3["result"]
# GET UV INDEX
url4 = "http://" + host + ":8080/json.htm?type=devices&rid=8"
response4 = urllib.urlopen(url4)
data4 = json.loads(response4.read())
uvIndex = data4["result"]
# GET CLOUD COVER
url5 = "http://" + host + ":8080/json.htm?type=devices&rid=12"
response5 = urllib.urlopen(url5)
data5 = json.loads(response5.read())
cloudCover = data5["result"]
# BUILD OUTPUT
output = "        Current Conditions:    Temp: " + str(thd[0]["Temp"]) + "F      DewPoint: " + str(thd[0]["DewPoint"]) + "F      Humidity: " + str(thd[0]["Humidity"]) + "%"
output += "       Feels Like Temp: " + str(windData[0]["Chill"]) + "F      Wind Speed: " + str(windData[0]["Speed"]) + " MPH      Gusting: " + str(windData[0]["Gust"]) + " MPH"
output += "       Wind Direction: " + str(windData[0]["DirectionStr"]) + "      Forecast: " + str(baro[0]["ForecastStr"])
output += "       UV Index: " + str(uvIndex[0]["UVI"]) + "        Cloud Cover: " + str(cloudCover[0]["Data"])
# WRITEOUT CONDITIONS TO BE PUSHED
file_object = open('/root/wx/conditions', 'w')
file_object.write(output)
