#include <functional>
#include <FS.h>
#include "WebIface.h"
#include "picker.html.h"


bool WebIface::setup()
{
  using namespace std::placeholders;
  WiFi.softAP("WS2812B");

  server.serveStatic("/img", SPIFFS, "/img");
  server.serveStatic("/js", SPIFFS, "/js");
  server.serveStatic("/css", SPIFFS, "/css");
  server.serveStatic("/", SPIFFS, "/picker.html");
  //server.on("/", std::bind(&WebIface::handleRoot, this));
  server.on("/setColour1", std::bind(&WebIface::setColour1, this));
  server.on("/setColour2", std::bind(&WebIface::setColour2, this));
  server.on("/setTimedelay", std::bind(&WebIface::setDelay_msec, this));
  server.on("/setdTheta", std::bind(&WebIface::setdTheta, this));
  server.on("/setHarmonicFactor", std::bind(&WebIface::setHarmonicFactor, this));
  server.on("/setAnimationMode", std::bind(&WebIface::setAnimationMode, this));

  server.begin();
  return true;
}

bool WebIface::handleClients()
{
  server.handleClient();

  return true;
}

void WebIface::handleRoot()
{
  String s = MAIN_page; //Read HTML contents
  server.send(200, "text/html", s); //Send web page
}

void WebIface::setColour1()
{
  String s_r = server.arg("r");
  String s_g = server.arg("g");
  String s_b = server.arg("b");
  String s_a = server.arg("a");
  Serial.println(s_r + " / " + s_g + " / " + s_b + " / " + s_a);
  rgb1 = rgb8(atoi(s_r.c_str()), atoi(s_g.c_str()), atoi(s_b.c_str()));

  send200();
}

void WebIface::setColour2()
{
  String s_r = server.arg("r");
  String s_g = server.arg("g");
  String s_b = server.arg("b");
  String s_a = server.arg("a");
  Serial.println(s_r + " / " + s_g + " / " + s_b + " / " + s_a);
  rgb2 = rgb8(atoi(s_r.c_str()), atoi(s_g.c_str()), atoi(s_b.c_str()));

  send200();
}

void WebIface::setDelay_msec()
{
  Serial.println(server.arg("n"));
  delay_msec = atoi(server.arg("n").c_str());
  send200();
}

void WebIface::setdTheta()
{
  Serial.println(server.arg("n"));
  dTheta = atoi(server.arg("n").c_str());
  send200();
}

void WebIface::setHarmonicFactor()
{
  Serial.println(server.arg("n"));
  harmonicfactor = atoi(server.arg("n").c_str());
  send200();
}

void WebIface::setAnimationMode()
{
  Serial.println(server.arg("mode"));
  if(server.arg("mode").equalsIgnoreCase("rainbow"))
  {
    animation_mode = RAINBOW;
  }
  else if(server.arg("mode").equalsIgnoreCase("two_colours"))
  {
    animation_mode = TWO_COLOURS;
  }
  else if(server.arg("mode").equalsIgnoreCase("imu"))
  {
    animation_mode = IMU;
  }
  send200();
}

void WebIface::send200()
{
  server.send(200, "text/plain");
}
