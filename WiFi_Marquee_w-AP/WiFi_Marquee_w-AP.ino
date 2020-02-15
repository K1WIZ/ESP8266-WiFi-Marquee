/*
Project: Wifi controlled LED matrix display with AP Mode
NodeMCU pins    -> EasyMatrix pins
MOSI-D7-GPIO13  -> DIN
CLK-D5-GPIO14   -> Clk
GPIO0-D3        -> LOAD

Revision: Feb, 14 2020, John Rogers

*/
#include <string.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "./DNSServer.h"                  // Patched lib
#include <ESP8266WebServer.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>   // look in sketch directory and copy library to global library directory
#include "FS.h"
const byte        DNS_PORT = 53;          // Capture DNS requests on port 53
IPAddress         apIP(10, 10, 10, 1);    // Private network for server
DNSServer         dnsServer;              // Create the DNS object
  
// **************** SET UP AP ***********************   //   Marquee should be password protected to prevent unauthorized changes
/* Set these to your desired credentials. */
const char *ssid = "WiFi Marquee";
const char *password = "mymessage";

// ******************* String form to sent to the client-browser ************************************
String form =
  "<html><body>"
  "<center>"
  "<h1>WiFi Marquee</h1>"
  "<p>Please specify...</p>"
  "<form action='msg'>"
  "<table>"
  "<tr>"
  "<td>Message</td><td><input type='text' name='msg' autofocus></td>"
  "</tr>"
  "<tr>"
  "<td>Scrolling Speed</td><td><input type='text' name='scrSp' value='20'></td>"      // Set a reasonable scroll speed for easy quick reading.  Need to figure out how to save to EEPROM or SPIFFS to preserve setting
  "</tr>"
  "<tr>"
  "<td>Number of Runs</td><td><input type='text' name='maxRuns' value='0'></td>"
  "</tr>"
  "<tr>"
  "<td>Brightness</td><td><select name='brightness'><option value='12'>12</option>"   // Set for default bright setting, but user can override.  Need to figure out how to save to EEPROM or SPIFFS to preserve setting
  "<option value='-1'>auto</option>"
  "<option value='0'>0</option>"
  "<option value='1'>1</option>"
  "<option value='2'>2</option>"
  "<option value='3'>3</option>"
  "<option value='4'>4</option>"
  "<option value='5'>5</option>"
  "<option value='6'>6</option>"
  "<option value='7'>7</option>"
  "<option value='8'>8</option>"
  "<option value='9'>9</option>"
  "<option value='10'>10</option>"
  "<option value='11'>11</option>"
  "<option value='12'>12</option>"
  "<option value='13'>13</option>"
  "<option value='14'>14</option>"
  "<option value='15'>15</option>"
  "</select></td>" 
  "</tr>"
  "<tr>"
  "<td></td><td><input type='submit' value='Submit'></td>"
  "</tr>"
  "</table>"
  "</form>"
  "</center>"
  "</body></html>";

ESP8266WebServer server(80);  // HTTP server will listen at port 80
long period;
int offset=1,refresh=0;
int pinCS = 4; // Attach CS to this pin, DIN to MOSI and CLK to SCK (cf http://arduino.cc/en/Reference/SPI )
int numberOfHorizontalDisplays = 12;
int numberOfVerticalDisplays = 1;
String decodedMsg;
String msg;

Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);

int wait = 20; // In milliseconds

int spacer = 2;
int width = 5 + spacer; // The font width is 5 pixels
int formIntensity=13;
int maxRuns=-1;
int runs=0;

/*
  handles the messages coming from the webbrowser, restores a few special characters and 
  constructs the strings that can be sent to the oled display
*/

int intensity=0;

void handle_msg() {
                        
  matrix.fillScreen(LOW);
  server.send(200, "text/html", form);    // Send same page so they can send another msg
  refresh=0;
  runs=0;
  
  
  msg = server.arg("msg");
  String formI=server.arg("brightness");
  formIntensity=formI.toInt();
  String runsI=server.arg("maxRuns");
  maxRuns=runsI.toInt();
  String spdI=server.arg("scrSp");
  wait=spdI.toInt();
  Serial.println(wait);
  if(wait<5)
    wait=5;
  else if(wait>80)
    wait=80;
  decodedMsg = msg;
  // Restore special characters that are misformed to %char by the client browser
  
  decodedMsg.replace("+", " ");      
  decodedMsg.replace("%21", "!");  
  decodedMsg.replace("%22", "");  //  quote character doesnt seem to render - needs fixing
  decodedMsg.replace("%23", "#");
  decodedMsg.replace("%24", "$");
  decodedMsg.replace("%25", "%");  
  decodedMsg.replace("%26", "&");
  decodedMsg.replace("%27", "'"); //  doesnt seem to render - needs fixing
  decodedMsg.replace("%28", "(");
  decodedMsg.replace("%29", ")");
  decodedMsg.replace("%2A", "*");
  decodedMsg.replace("%2B", "+");  
  decodedMsg.replace("%2C", ",");  
  decodedMsg.replace("%2F", "/");   
  decodedMsg.replace("%3A", ":");    
  decodedMsg.replace("%3B", ";");  
  decodedMsg.replace("%3C", "<");  
  decodedMsg.replace("%3D", "=");  
  decodedMsg.replace("%3E", ">");
  decodedMsg.replace("%3F", "?");  
  decodedMsg.replace("%40", "@"); 

  decodedMsg.toUpperCase();   // convert the string to upper case for easy readability.

// Save decoded message to SPIFFS file for playback on power up.
  File f = SPIFFS.open("/msgf.txt", "w");
  if (!f) {
    Serial.println("File open failed!");
  } else {
    Serial.print("Entered Message was: ");
    Serial.print(decodedMsg);
  f.print(decodedMsg);
  }
  f.close();
}

void setup(void) {
matrix.setIntensity(12); // Use a value between 0 and 15 for brightness

// Adjust to your own needs
for(int loop=0;loop<numberOfHorizontalDisplays;++loop)
{
  matrix.setRotation(loop,3);
  matrix.setPosition(loop, numberOfHorizontalDisplays-1-loop, 0); // The first display is at <0, 7>
}

//ESP.wdtDisable();                               // used to debug, disable wachdog timer, 
  Serial.begin(115200);                           // full speed to monitor
  Serial.println();
  SPIFFS.begin();
  delay(3000);
  Serial.print("Configuring access point...");

  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  Serial.println(WiFi.softAP("WiFi Marquee") ? "Ready" : "Failed!");   // to prevent unauthorized message change, we should use password protected AP

  Serial.print("Soft-AP IP address = ");

  Serial.println(WiFi.softAPIP());

  // if DNSServer is started with "*" for domain name, it will reply with
  // provided IP to all DNS request
  dnsServer.start(DNS_PORT, "*", apIP);
  
  // Set up the endpoints for HTTP server,  Endpoints can be written as inline functions:
  // replay to all requests with same HTML
  server.onNotFound([]() {
    server.send(200, "text/html", form);
  });
  server.on("/msg", handle_msg);
  server.begin();
  
// ***************** INITIAL READY & Read stored message from SPIFFS ****************
    File fr = SPIFFS.open("/msgf.txt", "r");
    while(fr.available()) {
    String line = fr.readStringUntil('n');
 //   Serial.println(line);
    decodedMsg = line;
    fr.close();
  }
  Serial.println("WebServer ready!   "); 
  double sensorValue = analogRead(A0);
  //Serial.println(sensorValue);                     //  for debug
  intensity=(int)(((sensorValue/1024.0)*(sensorValue/1024.0))*10.0);
  matrix.setIntensity(intensity);   // 0 = low, 10 = high
}

void loop(void) {

if((maxRuns<1)||(runs<maxRuns))
{
  for ( int i = 0 ; i < width * decodedMsg.length() + matrix.width()  - spacer; i++ ) {
    dnsServer.processNextRequest();
    server.handleClient();   // checks for incoming messages
  double sensorValue = analogRead(A0);
  //Serial.println(sensorValue);                     // for debug   
  intensity=formIntensity<0?(int)(((sensorValue/1024.0)*(sensorValue/1024.0))*10.0):formIntensity;
  matrix.setIntensity(intensity);   // 0 = low, 10 = high

    if (refresh==1) i=0;
    refresh=0;
    matrix.fillScreen(LOW);

    int letter = i / width;
    int x = (matrix.width() - 1) - i % width;
    int y = (matrix.height() - 8) / 2; // center the text vertically
 
    while ( x + width - spacer >= 0 && letter >= 0 ) {
      if ( letter < decodedMsg.length() ) {
        matrix.drawChar(x, y, decodedMsg[letter], HIGH, LOW, 1);
      }

      letter--;
      x -= width;
    }

    matrix.write(); // Send bitmap to display

    delay(wait);
  }
  ++runs;
}
else
{
    dnsServer.processNextRequest();
    server.handleClient();   // checks for incoming messages
    delay(wait);
}
}
