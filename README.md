# ESP8266-WiFi-Marquee
ESP8266 WiFi Marquee w/ AP:  Has stand-alone full time AP mode for ad-hoc message management & SPIFFS support.
ESP8266 WiFi Marquee No AP:  Only spawns AP at first run for entering your WiFi network creds, then it will use your network.  Browse to IP address on your network to manage messages.  This one also has SPIFFS support.



I wanted to build a wireless sign that could be portable, and setup anywhere using any mobile device. Possible uses are for store promotions, yard/flea market sales, tailgating, parties, office use, and public events.

The design is based on an ESP8266 MCU which drives a 8x64 matrix display (basically two 8x32's daisy chained) and scrolls a message that is entered via web browser. To enter the message, you simply browse for the sign's wireless network AP, open a browser to 10.10.10.1 and then type your message into the web form, hit SUBMIT and your message scrolls. The Marquee runs on 5 volts DC @ approx. 2 amps.

At the moment, I could use some help modifying the code to store the submitted message into the ESP8266's flash memory so that if power is cycled, it will read the last stored message and continue scrolling it. Once that milestone is reached, I will mark this project finished.

For the ESP8266, I used the Wemos D1 Mini, but any 8266 chip should work. Comments are in the sourcecode on how to wire the display.
