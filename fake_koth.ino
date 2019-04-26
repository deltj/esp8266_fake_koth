#include <Arduino.h>
#include <SPI.h>
#include <U8g2lib.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

//  Configure OLED screen
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ 16, /* clock=*/ 5, /* data=*/ 4);

//  Configure WiFi Access Point
const char *ssid = "WCTF_KingOfTheHill";
IPAddress address(172,16,10,1);
IPAddress subnet(255,255,255,0);

//  Configure web server
ESP8266WebServer server(80);

//  Submission counter
unsigned int count = 0;

void handleRequest() {
  server.send(200, "text/html", "<html><head><title>King of the Hill</title></head><body><br><br><style>body {background-color: #F6F4F3;}</style><br><center><h2 style=\"font-family:Courier New;\"><form action=\"/cgi-bin/score.php\" method=\"get\">Team Name: <input type=\"text\" name=\"team_name\"><br><br><input type=\"submit\" value=\"Submit Team Name\"></form></h2><p>Submit your team name to become King of the Hill!<br></center><br></p></body>");
}

void handleSubmission() {
  count++;
  
  String response = "<!DOCTYPE html><html><head><title>King of the Hill</title></head><body><style>body {background-color: #F6F4F3;}</style><center>The following team has scored:<h1 style=\"font-family:Courier New;\">TEAM</h1><br>Locking Access Point for 60 seconds</center></body></html>";
  String team = "";
  
  for(int i=0; i<server.args(); i++) {
    if(server.argName(i) == "team_name") {
      team = server.arg(i);    
    }
  }

  response.replace("TEAM", team);
  
  server.send(200, "text/html", response);
}

void setup() {
  u8g2.begin();

  server.on("/", handleRequest);
  server.on("/cgi-bin/score.php", handleSubmission);
  server.begin();
  
  WiFi.softAPConfig(address, address, subnet);
  WiFi.softAP(ssid, 0, 1);
}

void loop() {
  u8g2.setFont(u8g2_font_t0_11_mf);
  u8g2.firstPage();

  do {
    u8g2.setCursor(0, 20);
    u8g2.print(ssid);
  } while(u8g2.nextPage());

  server.handleClient();
  delay(1000);
}
