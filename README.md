# ESP8266-WiFi-Marquee

<!---
[![start with why](https://img.shields.io/badge/start%20with-why%3F-brightgreen.svg?style=flat)](http://www.ted.com/talks/simon_sinek_how_great_leaders_inspire_action)
--->
[![GitHub release](https://img.shields.io/github/release/elbosso/ESP8266-WiFi-Marquee/all.svg?maxAge=1)](https://GitHub.com/elbosso/ESP8266-WiFi-Marquee/releases/)
[![GitHub tag](https://img.shields.io/github/tag/elbosso/ESP8266-WiFi-Marquee.svg)](https://GitHub.com/elbosso/ESP8266-WiFi-Marquee/tags/)
[![GitHub license](https://img.shields.io/github/license/elbosso/ESP8266-WiFi-Marquee.svg)](https://github.com/elbosso/ESP8266-WiFi-Marquee/blob/master/LICENSE)
[![GitHub issues](https://img.shields.io/github/issues/elbosso/ESP8266-WiFi-Marquee.svg)](https://GitHub.com/elbosso/ESP8266-WiFi-Marquee/issues/)
[![GitHub issues-closed](https://img.shields.io/github/issues-closed/elbosso/ESP8266-WiFi-Marquee.svg)](https://GitHub.com/elbosso/ESP8266-WiFi-Marquee/issues?q=is%3Aissue+is%3Aclosed)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](https://github.com/elbosso/ESP8266-WiFi-Marquee/issues)
[![GitHub contributors](https://img.shields.io/github/contributors/elbosso/ESP8266-WiFi-Marquee.svg)](https://GitHub.com/elbosso/ESP8266-WiFi-Marquee/graphs/contributors/)
[![Github All Releases](https://img.shields.io/github/downloads/elbosso/ESP8266-WiFi-Marquee/total.svg)](https://github.com/elbosso/ESP8266-WiFi-Marquee)
[![Website elbosso.github.io](https://img.shields.io/website-up-down-green-red/https/elbosso.github.io.svg)](https://elbosso.github.io/)

ESP8266 WiFi Marquee w/ AP:  Has stand-alone full time AP mode for ad-hoc message management & SPIFFS support.
ESP8266 WiFi Marquee No AP:  Only spawns AP at first run for entering your WiFi network creds, then it will use your network.  Browse to IP address on your network to manage messages.  This one also has SPIFFS support.



I wanted to build a wireless sign that could be portable, and setup anywhere using any mobile device. Possible uses are for store promotions, yard/flea market sales, tailgating, parties, office use, and public events.

The design is based on an ESP8266 MCU which drives a 8x64 matrix display (basically two 8x32's daisy chained) and scrolls a message that is entered via web browser. To enter the message, you simply browse for the sign's wireless network AP, open a browser to 10.10.10.1 and then type your message into the web form, hit SUBMIT and your message scrolls. The Marquee runs on 5 volts DC @ approx. 2 amps.

The code is done so that the last entered message will be persistent across reboots/powercycles.  Your last entered message will continue scrolling until changed.

For the ESP8266, I used the Wemos D1 Mini, but any 8266 chip should work. Comments are in the sourcecode on how to wire the display.
